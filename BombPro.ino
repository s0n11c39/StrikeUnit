#include <Keypad.h>               // Keypad-Tastatur-Matrix
#include <Wire.h>                 // I2C-Kommunikation
#include <LiquidCrystal_I2C.h>    // I2C-LCD-Display
#include "lang.h"                 // Sprachtexte auslagern


LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD-Display Initialisierung (Adresse 0x27, 20 Zeichen, 4 Zeilen)

// Keypad-Matrix (4 Zeilen, 4 Spalten)
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
 {'1','2','3','a'},
 {'4','5','6','b'},
 {'7','8','9','c'},
 {'*','0','#','d'}
};
// Pinzuordnung für Zeilen und Spalten des Keypads
byte rowPins[ROWS] = {5,4,3,2};
byte colPins[COLS] = {9,8,7,6};

// Keypad-Objekt initialisieren
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Diverse globale Variablen für Passworteingabe, Zeit, Status
char enteredText[8];
byte time[4];
byte refresh=0; // Flag für Anzeige-Refresh
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

// Logische Tastenzuordnung für Menü-Navigation
char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok/Bestätigen  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   //Noch nicht verwendet / not implemented

// LED-Pinzuweisungen
const int REDLED = 11;
const int GREENLED = 10;
//const int BLUELED = 12;

// MOSFET (Sirene-Ausgang) Einstellungen
boolean mosfetEnable = false;
const int mosfet = A0;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFET_TIME = 5000;

// MOSFET für Pyro-Ausgang
boolean mosfetpyroEnable = false;
const int mosfetpyro = A1;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFETPYRO_TIME = 5000;

// Zeit-Einstellungen (Minuten/Sekunden)
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;

// Diverse Status-Flags
boolean endGame = false;
boolean start = true;
boolean defuseando; // Defuse-Vorgang läuft (wird noch umbenannt!)
boolean cancelando; // Cancel-Vorgang läuft (wird noch umbenannt!)

// Cut The Wire-Mode: Richtiger Draht und Pinzuordnung
int correctWire = 4; // Nummer des richtigen Drahts (1-4)
int wirePins[4] = {A2, A3, A4, A5}; // Zuordnung zu den Pins


// Soundeinstellungen und Tone-Pins
boolean soundEnable = true;
int tonepin = 12 ; // Piezo-Lautsprecher an Pin 12
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;

// Zeitvariablen für Ablauf/Spielmodi
unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime; // Startzeit für Domination-Zone
byte team=0; // 0=neutral, 1=grün, 2=rot



// Spielmodi als Enum (zentrale Steuerung)
enum GameMode {
  NONE,
  SEARCH_DESTROY,
  SABOTAGE,
  DOMINATION,
  CUT_THE_WIRE
};

GameMode currentGameMode = NONE; // Standard: Kein Modus

// Initialisierung: Wird beim Starten des Arduino einmalig ausgeführt
void setup() {
  Serial.begin(9600);     // Serielle Schnittstelle für Debug
  initLCD();              // LCD initialisieren & Begrüßungsbildschirm
  initKeypad();           // Keypad einrichten
  initHardware();         // LEDs, MOSFETs, Draht-Pins initialisieren
  initCustomChars();      // Eigene Symbole für LCD laden
  delay(5000);            // 5 Sekunden Startbildschirm
}

// Hauptschleife: Menü aufrufen (endet nie)
void loop(){
  menuPrincipal();
}

// Keypad-Event-Handler: Reagiert auf gedrückte/gehaltene Tasten
void keypadEvent(char key) {
  KeyState state = keypad.getState();

  if (key == 'd') defuseando = (state == HOLD); // Taste D = Defuse
  if (key == 'c') cancelando = (state == HOLD); // Taste C = Abbrechen
}

// Anzeige bei entschärfter Bombe/Erfolg
void disarmedSplash() {
  endGame = false;
  showEndSplash(true);
}

// Anzeige bei Explosion/Fehlschlag
void explodeSplash() {
  endGame = false;
  showEndSplash(false);
}

// Zentrale Funktion für Spielende (Sieg/Niederlage)
void showEndSplash(bool defused) {
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  cls();
  delay(100);

  if (defused) {
    // Sieg-Anzeige je nach Spielmodus
    if (currentGameMode == SEARCH_DESTROY || currentGameMode == SABOTAGE) {
      lcd.setCursor(2, 0); printProgmemString(BOMB_DISARMED);
      lcd.setCursor(3, 1); printProgmemString(GOODS_WIN);
      digitalWrite(GREENLED, HIGH);
      delay(5000);
      digitalWrite(GREENLED, LOW);
    }
    else if (currentGameMode == CUT_THE_WIRE) {
      lcd.setCursor(2, 0); printProgmemString(BOMB_DISARMED);
      lcd.setCursor(4, 1); printProgmemString(GOODS_WIN);
      digitalWrite(GREENLED, HIGH);
      delay(5000);
      digitalWrite(GREENLED, LOW);
    }
  } else {

    // Niederlage-Anzeige
    if (currentGameMode == CUT_THE_WIRE) {
      lcd.setCursor(1, 0); printProgmemString(GAME_OVER);
      lcd.setCursor(3, 1); printProgmemString(TERRORISTS_WIN);
    } else {
      lcd.setCursor(1, 0); printProgmemString(TERRORISTS_WIN);
      lcd.setCursor(4, 1); printProgmemString(GAME_OVER);
    }
    // Piezo-Soundeffekt
    for (int i = 200; i > 0; i--) {
      tone(tonepin, i);
    }
    noTone(tonepin);

    // Auslösung MOSFET/Pyro (falls aktiviert)
    if (mosfetpyroEnable && mosfetEnable) {
      activateMosfetpyro();
      activateMosfet();
    }

    delay(5000);
  }

  // Play Again/Weiter
  cls();
  printProgmemString(PLAY_AGAIN);
  lcd.setCursor(0, 1);
  printProgmemString(YES_NO);

  while (1) {
    var = keypad.waitForKey();
    if (var == 'a') {
      tone(tonepin, 2400, 30);
      if (currentGameMode == SEARCH_DESTROY) { startGameCount(); search(); }
      if (currentGameMode == SABOTAGE) { currentGameMode = SABOTAGE; startGameCount(); start = true; sabotage(); }
      if (currentGameMode == CUT_THE_WIRE) { currentGameMode = CUT_THE_WIRE; startGameCount(); wire(); }
      break;
    }
    if (var == 'b') {
      tone(tonepin, 2400, 30);
      menuPrincipal();
      break;
    }
  }
}

// LCD initialisieren und Begrüßungsbildschirm anzeigen
void initLCD() {
  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(6, 0); printProgmemString(START_TITLE);
  lcd.setCursor(1, 1); printProgmemString(START_SUBTITLE);
  lcd.setCursor(5, 2); printProgmemString(START_AUTHOR);
  lcd.setCursor(17, 3); printProgmemString(START_VERSION);

  tone(tonepin, 2400, 30); // Startsound
}

// Keypad initialisieren (Entprellen, HoldTime, Eventlistener)
void initKeypad() {
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
}

// Hardware initialisieren: LEDs, MOSFETs, wirePins als INPUT_PULLUP
void initHardware() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(mosfet, OUTPUT);
  pinMode(mosfetpyro, OUTPUT);

  // Cut The Wire: Draht-Pins als INPUT_PULLUP
  for (int i = 0; i < 4; i++) {
    pinMode(wirePins[i], INPUT_PULLUP);
  }
}

// Eigene Symbole/Grafiken für das LCD anlegen
void initCustomChars() {
  byte bar1[8] = { B10000,B10000,B10000,B10000,B10000,B10000,B10000,B10000 };
  byte bar2[8] = { B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11000 };
  byte bar3[8] = { B11100,B11100,B11100,B11100,B11100,B11100,B11100,B11100 };
  byte bar4[8] = { B11110,B11110,B11110,B11110,B11110,B11110,B11110,B11110 };
  byte bar5[8] = { B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111 };
  byte up[8]   = { B00000,B00100,B01110,B11111,B11111,B00000,B00000,B00000 };
  byte down[8] = { B00000,B00000,B11111,B11111,B01110,B00100,B00000,B00000 };

  lcd.createChar(0, bar1);
  lcd.createChar(1, bar2);
  lcd.createChar(2, bar3);
  lcd.createChar(3, bar4);
  lcd.createChar(4, bar5);
  lcd.createChar(5, up);
  lcd.createChar(6, down);
}
