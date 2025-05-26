// ======================================================
// Zeitanzeige für "Cut the Wire"-Modus mit SpeedFactor
// ======================================================
void printTimeWire(unsigned long minutos, unsigned long aTiempo, float speedFactor) {
  // Berechne verbleibende Spielzeit unter Berücksichtigung des Speed-Faktors
  unsigned long totalMillis = (minutos + 1) * 60000;
  unsigned long scaledTime = (unsigned long)(aTiempo * speedFactor);
  unsigned long remaining = (scaledTime < totalMillis) ? (totalMillis - scaledTime) : 0;

  unsigned int mins = (remaining / 60000);
  unsigned int secs = (remaining / 1000) % 60;

  // "Fake"-Millisekunden für animierte Anzeige (nur optischer Effekt)
  unsigned int fakeMillis = 999 - (millis() % 1000);

  // Zeit formatieren & ausgeben: MM:SS:ms
  if (mins < 10) lcd.print("0");
  lcd.print(mins);
  lcd.print(":");

  if (secs < 10) lcd.print("0");
  lcd.print(secs);
  lcd.print(":");

  if (fakeMillis < 100) lcd.print("0");
  if (fakeMillis < 10) lcd.print("0");
  lcd.print(fakeMillis);
}

// ======================================================
// CUT THE WIRE Hauptlogik
// ======================================================
void wire() {
  bool wireCut[4] = { false, false, false, false }; // Merker für bereits durchtrennte Drähte
  cls();
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);

  // Setup-Variablen
  int minutos = GAMEMINUTES - 1;
  unsigned long iTime = millis();     // Spielstartzeit
  unsigned long aTime;
  float speedFactor = 1.0;            // Startgeschwindigkeit (kann sich erhöhen)
  int timeCalcVar = 0;
  bool bombArmed = false;
  int wrongWiresCut = 0;              // Zähler für falsch durchtrennte Drähte

  // === PRE-ARM Phase: Warte bis Bombe scharf gemacht wird ===
  while (!bombArmed && !passwordEnable) {
    keypad.getKey();  // Löst keypadEvent() aus (defuseando etc.)
    aTime = millis() - iTime;

    // Zeit & Hinweis anzeigen
    lcd.setCursor(3, 0);
    printProgmemString(GAME_TIME_TOP);
    lcd.setCursor(3, 1);
    printTime(minutos, aTime);

    // Blink- und Soundlogik (grüne LED)
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 50)
      digitalWrite(GREENLED, HIGH);
    if (timeCalcVar >= 90 && timeCalcVar <= 130)
      digitalWrite(GREENLED, LOW);

    // Scharfmachen durch Taste D (defuseando)
    if (defuseando) {
      lcd.clear();
      digitalWrite(GREENLED, LOW);
      lcd.setCursor(2, 0);
      printProgmemString(ARMING_BOMB);
      lcd.setCursor(0, 1);

      unsigned int percent = 0;
      unsigned long xTime = millis();

      while (defuseando) {
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 40) {
          digitalWrite(REDLED, HIGH);
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 520) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds = millis() - xTime;
        percent = seconds / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        if (percent >= 100) {
          digitalWrite(GREENLED, LOW);
          bombArmed = true;
          defuseando = false;
          break;
        }
      }

      cls();
      digitalWrite(REDLED, LOW);
    }

    // Zeit abgelaufen, ohne dass Bombe scharf gemacht wurde
    if (minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) {
      explodeSplash();
    }

    delay(100); // Kurze Pause, stabilisiert Anzeige
  }

  // === PHASE 2: Bombe ist scharf ===
  cls();
  lcd.setCursor(0, 0);
  printProgmemString(BOMB_ARMED);
  delay(1000);
  cls();

  // === Countdown-Phase: Spieler müssen den richtigen Draht durchtrennen ===
  iTime = millis(); // Neue Startzeit für Countdown
  while (1) {
    keypad.getKey();
    aTime = millis() - iTime;
    unsigned long totalCountdown = (minutos + 1) * 60000;
    unsigned long adjustedTime = (unsigned long)(aTime * speedFactor);

    // Hinweistext + Restzeit anzeigen
    lcd.setCursor(0, 0);
    lcd.print(F("Find & cut wire!  "));
    lcd.setCursor(3, 1);
    printTimeWire(minutos, aTime, speedFactor);

    // LED/Sound-Warnungen (rote LED, Piezo)
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 40) {
      digitalWrite(REDLED, HIGH);
      if (soundEnable) tone(tonepin, tonoActivada, 50);
    }
    if (timeCalcVar >= 180 && timeCalcVar <= 220) {
      digitalWrite(REDLED, LOW);
    }

    if (timeCalcVar >= 245 && timeCalcVar <= 255 && minutos - adjustedTime / 60000 < 2 && soundEnable)
      tone(tonepin, tonoActivada, 50);
    if (timeCalcVar >= 495 && timeCalcVar <= 510 && minutos - adjustedTime / 60000 < 4 && soundEnable)
      tone(tonepin, tonoActivada, 50);
    if (timeCalcVar >= 745 && timeCalcVar <= 760 && minutos - adjustedTime / 60000 < 2 && soundEnable)
      tone(tonepin, tonoActivada, 50);

    // Ablaufzeit erreicht (Bombe explodiert)
    if (adjustedTime >= totalCountdown) {
      explodeSplash();
    }

    // === Drahtprüfung: Wurde ein Draht durchtrennt? ===
    for (int i = 0; i < 4; i++) {
      // Nur reagieren, wenn Draht gerade erstmals von "intakt" auf "durchtrennt" wechselt
      if (!wireCut[i] && digitalRead(wirePins[i]) == HIGH) {
        wireCut[i] = true; // Draht als bearbeitet markieren

        if (i + 1 == correctWire) {
          // Richtiger Draht: Sieg/Entschärft
          for (int b = 0; b < 6; b++) {
            digitalWrite(REDLED, HIGH);
            digitalWrite(GREENLED, HIGH);
            delay(100);
            digitalWrite(REDLED, LOW);
            digitalWrite(GREENLED, LOW);
            delay(100);
          }
          disarmedSplash();
        } else {
          // Falscher Draht: Spiel wird schneller
          wrongWiresCut++;
          // SpeedFactor je nach Zahl der falschen Schnitte erhöhen
          if (wrongWiresCut == 1) speedFactor = 1.5;
          else if (wrongWiresCut == 2) speedFactor = 2.0;
          else if (wrongWiresCut >= 3) speedFactor = 3.0;
          // Feedback: LED blinkt kurz
          digitalWrite(REDLED, HIGH); delay(200); digitalWrite(REDLED, LOW);
        }
      }
    }

    delay(1); // sehr kurz, für schnellen ms-Effekt!
  }
}
