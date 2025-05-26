// Fortschrittsbalken für Prozentanzeige auf dem LCD zeichnen (0–100%)
void drawPorcent(byte porcent) {
  int filledBlocks = (8 * porcent) / 10;  // Umrechnung Prozent auf 0–8 Blöcke (max. 8)

  lcd.setCursor(0, 1);

  // Volle 5er-Blöcke ausgeben
  while (filledBlocks >= 5) {
    lcd.write(4); // Volles Blocksymbol
    filledBlocks -= 5;
  }
  // Restliche Teilblöcke (0–4)
  if (filledBlocks > 0) {
    lcd.write(filledBlocks - 1); // CustomChars 0–3 für Teilbalken
  }
}

// Display löschen und Cursor auf 0,0 setzen
void cls() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

// Spielzeitanzeige HH:MM:SS:ms (mit Restzeit)
void printTime(unsigned long minutos, unsigned long aTiempo) {
  unsigned long remainingMins = minutos - aTiempo / 60000;
  unsigned long hours = remainingMins / 60;
  unsigned long mins  = remainingMins % 60;
  unsigned long secs  = 59 - (aTiempo / 1000) % 60;
  unsigned long ms    = 999 - (millis() % 1000);

  if (hours > 0) {
    lcd.print((hours < 10) ? "0" : ""); lcd.print(hours); lcd.print(":");
  }
  lcd.print((mins < 10) ? "0" : ""); lcd.print(mins); lcd.print(":");
  lcd.print((secs < 10) ? "0" : ""); lcd.print(secs); lcd.print(":");
  lcd.print(ms);
}

// Zeitformat MM:SS:ms (Domination-Modus, optional Millisekunden)
void printTimeDom(unsigned long aTiempo, boolean showMillis) {
  unsigned long mins = aTiempo / 60000;
  unsigned long secs = (aTiempo / 1000) % 60;
  unsigned long ms   = 999 - millis() % 1000;

  lcd.print((mins < 10) ? "0" : ""); lcd.print(mins); lcd.print(":");
  lcd.print((secs < 10) ? "0" : ""); lcd.print(secs);
  if (showMillis) {
    lcd.print(":"); lcd.print(ms);
  }
}

// Start-Counter und Vorbereitungsanzeige vor Spielstart
void startGameCount() {
  cls();
  lcd.setCursor(3, 0);
  printProgmemString(GAME_READY);
  lcd.setCursor(2, 2);
  printProgmemString(PRESS_ANY_BUTTON);
  keypad.waitForKey();  // Warten auf Tasten-Druck

  cls();
  lcd.setCursor(3, 0);
  printProgmemString(GAME_STARTING);
  for (int i = 5; i > 0; i--) { // Countdown von 5
    lcd.setCursor(8, 2);
    tone(tonepin, 2000, 100);
    printProgmemString(COUNTDOWN_IN);
    lcd.print(i);
    delay(1000);
  }
  cls();
}

// Pfeile für Menünavigation anzeigen
void checkArrows(byte i, byte maxx) {
  if (i == 0) {
    lcd.setCursor(15, 1); lcd.write(6); // Pfeil nach unten
  }
  if (i == maxx) {  
    lcd.setCursor(15, 0); lcd.write(5); // Pfeil nach oben
  }
  if (i > 0 && i < maxx) {
    lcd.setCursor(15, 1); lcd.write(6);
    lcd.setCursor(15, 0); lcd.write(5);  
  }
}

// MOSFET-Ausgänge für Pyro
void activateMosfetpyro() {
  digitalWrite(mosfetpyro, HIGH);
  delay(MOSFETPYRO_TIME);
  digitalWrite(mosfetpyro, LOW);
}

// MOSFET-Ausgang für Sirene
void activateMosfet() {
  digitalWrite(mosfet, HIGH);
  delay(MOSFET_TIME);
  digitalWrite(mosfet, LOW);
}
