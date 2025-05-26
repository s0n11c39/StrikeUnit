// =============================
// SABOTAGE MODUS (Hauptfunktion)
// =============================
void sabotage(){
  cls();                        // Display löschen
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);   
  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;  // Spielzeit (Minuten-1)

  // Startzeit einmalig setzen, falls Spiel frisch gestartet
  if(start){
  iTime = millis(); // Startzeit merken 
  start = false;
  }
  
  unsigned long aTime;

  // === Haupt-Spielschleife ===
  while(1){

    // LED Blink-Logik (Grün blinkt periodisch)
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 50)
    {
      digitalWrite(GREENLED, HIGH);  
    }
    if(timeCalcVar >= 90 && timeCalcVar <= 130)
    {    
      digitalWrite(GREENLED, LOW);  
    }

    // Zeit & Info anzeigen
    lcd.setCursor(3,0);
    printProgmemString(GAME_TIME);
    aTime=millis()- iTime;
    lcd.setCursor(3,1);
    printTime(minutos, aTime);

    // === SPIELENDE? Zeit abgelaufen ===
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      lcd.clear();
      while(1){
        printProgmemString(GAME_OVER);
        lcd.setCursor(0,1);
        printProgmemString(DEFENDERS_WIN);  

        // Alarm-Sound
        for(int i = 1000; i>200; i--){
          if(soundEnable)tone(tonepin,i);
          delay(5);
        }
        noTone(tonepin);
        delay(5000);
        cls();
        menuPrincipal();  // Zurück ins Hauptmenü
      }
    }

    // === Passwort-Variante: Taste D gedrückt + Passwort aktiv ===
    if('d' == keypad.getKey() && passwordEnable){
      lcd.clear();
      lcd.setCursor(2,0);      
      printProgmemString(ARMING_BOMB);
      delay(1000);  // Kurze Pause

      lcd.clear();
      lcd.setCursor(0, 0);
      printProgmemString(ENTER_CODE);

      setCode();  // Passwort eingeben

      if(comparePassword()){
        destroySabotage();  // Bombe scharf machen
      }        
      lcd.clear();
      lcd.setCursor(2,0);
      printProgmemString(CODE_ERROR);
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    // === Normaler Modus: Bombe scharf machen (Taste D halten, kein Passwort) ===
    while(defuseando && !passwordEnable)
    {
      keypad.getKey();
      cls();
      digitalWrite(GREENLED, LOW);
      lcd.clear();
      lcd.setCursor(2,0);
      printProgmemString(ARMING_BOMB);
      lcd.setCursor(0,1);

      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)

      {
        keypad.getKey();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 40)
        {
          digitalWrite(REDLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 520)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(REDLED, LOW);
        }
        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          destroySabotage();  // Bombe scharf!
        }
      }
      cls();
      digitalWrite(REDLED, LOW);  
    }
  }
}

// ======================================
// ZWEITE PHASE: Bombe ist scharf (Countdown)
// ======================================
void destroySabotage(){
  lcd.clear();
  lcd.setCursor(3,0);
  printProgmemString(BOMB_ARMED);
  delay(1000);

  int minutos=BOMBMINUTES-1;    // Countdown für Bombe (Minuten-1)
  unsigned long iTime=millis(); // Startzeit der Phase
  unsigned long aTime;
  int largoTono = 50;

  // === Countdown-Schleife ===
  while(1){

     // Explosionsphase: Spiel-Ende durch Fehlschlag
    if(endGame){
      explodeSplash();
    }

    // Rote LED blinkt als Warnung
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      digitalWrite(REDLED, HIGH);  
      if(soundEnable)tone(tonepin,tonoActivada,largoTono);
    }
    if(timeCalcVar >= 180 && timeCalcVar <= 220){
      digitalWrite(REDLED, LOW);  
    }
    // Weitere Sound-Warnungen je nach verbleibender Zeit
    timeCalcVar=(millis()- iTime)%1000;
    aTime= millis()- iTime;
    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if( minutos-aTime/60000==0 && 59-((aTime/1000)%60) < 10)largoTono = 300;

    lcd.setCursor(1,0);
    printProgmemString(DETONATION_IN);  
    lcd.setCursor(3,1);

    // === SPIELENDE: Zeit abgelaufen (Bombe explodiert) ===
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      explodeSplash();
    }

    printTime(minutos, aTime);  // Countdown auf Display

    // === DISARMING (mit Passwort) ===
    if('d' == keypad.getKey() && passwordEnable){
      cls();
      digitalWrite(REDLED, LOW);  
      digitalWrite(GREENLED, HIGH); 
      printProgmemString(DISARMING);
      delay(1000);

      lcd.clear();
      lcd.setCursor(0,0);
      printProgmemString(ENTER_CODE);

      setCode();  // Passwort eingeben

      if(comparePassword()){
        sabotage(); // Wieder zurück in die erste Phase
      }        
      lcd.clear();
      lcd.setCursor(2,0);
      printProgmemString(CODE_ERROR);
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    // === DISARMING (ohne Passwort) ===
    if(defuseando && !passwordEnable)
    {
      lcd.clear();
      digitalWrite(REDLED, LOW);
      lcd.setCursor(3,0);
      printProgmemString(DISARM);
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis();
      while(defuseando)
      {
        keypad.getKey();
        // Prüfen, ob Zeit während Entschärfung abläuft
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar=(millis()- xTime)%1000;
        if(timeCalcVar>= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(GREENLED, LOW);  
        }
        unsigned long seconds=(millis()- xTime);
        percent= seconds/(ACTIVATESECONDS*10);
        drawPorcent(percent);  

        // Entschärfung erfolgreich: Zurück in erste Phase
        if(percent >= 100)
        {
          sabotage();   
        }
      }
      digitalWrite(REDLED, LOW); 
      digitalWrite(GREENLED, LOW);
      cls(); 
    }
  }
}
