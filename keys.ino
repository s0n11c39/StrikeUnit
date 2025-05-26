// Prüft, ob eine bestimmte Taste aktuell gedrückt ist (HOLD, PRESS)
boolean isPressed(char key) {
  char currentKey = keypad.getKey();
  return (currentKey == key);
}

// Vergleicht das eingegebene Passwort (enteredText) mit dem gespeicherten Passwort
boolean comparePassword() {
  for (int i = 0; i < 8; i++) {
    if (enteredText[i] != password[i]) return false;
  }
  return true;
}

// Hilfsfunktion für die Anzeige beim Code-Setzen (verhindert Dopplung)
void printMaskedCode(int i, char var) {
  if (i != 0) {
    lcd.setCursor(i - 1, 1);
    lcd.print("*");
    lcd.print(var);
  } else {
    lcd.print(var);
  }
  tone(tonepin, 2400, 30); // Soundfeedback
}

// Fragt 8-stelligen Code vom Nutzer ab (in enteredText gespeichert)
void setCode() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) {
    while (1) {
      var = getNumber();
      if (var != 'x') {
        enteredText[i] = var;
        printMaskedCode(i, var);
        break;
      }
    }
  }
}

// Variante von setCode() mit 5 Sekunden Timeout je Ziffer
void setCodeTime() {
  timeCalcVar = millis();
  for (int i = 0; i < 8; i++) {
    while (1) {
      if (5000 + timeCalcVar - millis() <= 100) {
        enteredText[i] = 'x';
        break;
      }
      lcd.setCursor(11, 0);
      printTimeDom(5000 + timeCalcVar - millis(), false);
      var = getNumber();
      if (var != 'x') {
        enteredText[i] = var;
        printMaskedCode(i, var);
        break;
      }
    }
  }
}

// Setzt das gespeicherte Passwort (password[8]) per Nutzereingabe neu
void setPass() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) { 
    while (1) {
      var = getNumber();
      if (var != 'x') {
        password[i] = var;
        printMaskedCode(i, var);
        break;
      }
    }  
  }
}

// Dialog zum Setzen eines neuen Passworts (Eingabe + Wiederholung)
void setNewPass() {
  while (1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    printProgmemString(ENTER_NEW_PASS);
    setPass();

    lcd.clear();
    lcd.setCursor(0, 0);
    printProgmemString(RETYPE_PASS);
    setCode();

    if (comparePassword()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      printProgmemString(PASSWORD_SET_OK);
      delay(2000);
      break; 
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      printProgmemString(PASSWORD_ERROR);
      if (soundEnable) tone(tonepin, errorTone, 200);
      delay(2000);
    }
  }
}

// Gibt eingegebenes Zeichen zurück, wenn Zahl; sonst 'x'
char getNumber() {
  var = keypad.getKey();
  if (var) {
    switch (var) {
      case 'a': case 'b': case 'c': case 'd':
      case '*': case '#':
        return 'x';
      default:
        return var; // Ziffer
    }
  }
  return 'x';
}

// Liest eine Ziffer ein (blockiert bis Tastendruck), gibt als Byte (0–9) zurück, sonst 11 bei ungültig
byte getRealNumber() {
  var = keypad.waitForKey();
  if (var) {
    switch (var) {
      case '1': return 1;
      case '2': return 2;
      case '3': return 3;
      case '4': return 4;
      case '5': return 5;
      case '6': return 6;
      case '7': return 7;
      case '8': return 8;
      case '9': return 9;
      case '0': return 0;
      default:  return 11;
    }
  }
  return 11;
}
