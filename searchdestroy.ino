// =============================
// SEARCH & DESTROY MODUS (Hauptfunktion)
// =============================
void search(){
  cls();                          // Display löschen
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);   
  int minutos = GAMEMINUTES-1;    // Spielzeit (Minuten-1)
  unsigned long iTime=millis();   // Startzeit merken
  unsigned long aTime;

  // === Haupt-Spielschleife ===
  while(1){

    // Wenn Spiel vorbei, Zeige Endbildschirm und gehe ins Menü zurück
    if(endGame){
      endSplash();
    }

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

    lcd.setCursor(3,0);
    printProgmemString(GAME_TIME_TOP);
    aTime=millis()- iTime;
    lcd.setCursor(3,1);
    printTime(minutos, aTime);

    // === SPIELENDE? Zeit abgelaufen ===
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      endSplash();
    }
    
    // === Bombe scharf machen (mit Passwort) ===
    if('d' == keypad.getKey() && passwordEnable){
      lcd.clear();
      lcd.setCursor(2, 0);
      printProgmemString(ARMING_BOMB);
      delay(1000);//a little delay to think in the password
      lcd.clear();
      lcd.setCursor(0, 0);
      printProgmemString(ENTER_CODE);

      setCodeTime();  // Zeitbegrenzte Passworteingabe

      if(comparePassword()){
        destroy();  // Bombe ist scharf, Countdown startet
      }        
      lcd.clear();
      lcd.setCursor(3,0);
      printProgmemString(CODE_ERROR);
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }
    
    // === Bombe scharf machen (ohne Passwort) ===
    while(defuseando && !passwordEnable)
    {
      digitalWrite(GREENLED, LOW);
      lcd.clear();
      lcd.setCursor(2,0);
      printProgmemString(ARMING_BOMB);
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis()
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
          destroy();  // Bombe ist scharf, Countdown startet
        }
      }
      cls();
      digitalWrite(REDLED, LOW);  

    }
  }
}

// ========================================
// PHASE 2: Die Bombe ist scharf (Countdown)
// ========================================
void destroy(){
  lcd.clear();
  lcd.setCursor(3,0);
  printProgmemString(BOMB_ARMED);
  delay(1000);
  int minutos=BOMBMINUTES-1;
  unsigned long iTime=millis();
  unsigned long aTime;
  int largoTono = 50;

  // === Countdown-Schleife ===
  while(1){

    // Wenn Entschärfen fehlschlägt oder Zeit abläuft, Spielende
    if(endGame){
      explodeSplash();
    }

    // Rote LED blinkt (Bomben-Countdown)
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      digitalWrite(REDLED, HIGH);  
      if(soundEnable)tone(tonepin,tonoActivada,largoTono);
    }
    if(timeCalcVar >= 180 && timeCalcVar <= 220){
      digitalWrite(REDLED, LOW);  
    }
    
    // Zusätzliche Soundwarnungen, je nach Restzeit 
    aTime= millis()- iTime;
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if( minutos-aTime/60000==0 && 59-((aTime/1000)%60) < 10)largoTono = 300;

    lcd.setCursor(1,0);
    printProgmemString(DETONATION_IN);

    lcd.setCursor(3,1);

    // === SPIELENDE: Zeit abgelaufen ===
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      explodeSplash();
    }
  
    printTime(minutos, aTime);  // Countdown anzeigen

    // === Entschärfung mit Passwort ===
    if('d' == keypad.getKey() && passwordEnable){

      lcd.clear();
      lcd.setCursor(1,0);
      printProgmemString(DISARMING);
      delay(1000);//a little delay to think in the password

      lcd.clear();
      lcd.setCursor(0,0);
      printProgmemString(ENTER_CODE);

      setCodeTime();

      if(comparePassword()){
        disarmedSplash(); // Entschärft, Team gewinnt
      }        
      lcd.clear();
      lcd.setCursor(3,0);
      printProgmemString(CODE_ERROR);
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    // === Entschärfung ohne Passwort ===
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

        // Bombe entschärft: Team gewinnt
        if(percent >= 100)
        {
          disarmedSplash();   
        }
      }
      digitalWrite(REDLED, LOW); 
      digitalWrite(GREENLED, LOW);
      cls();
    }
  }
}

// ========================================
// SPIELENDE: Siegeranzeige + Zurück ins Menü
// ========================================
void endSplash(){
  lcd.clear();
  while(1){
    printProgmemString(GAME_OVER);
    lcd.setCursor(0,1);
    printProgmemString(DEFENDERS_WIN); 

    for(int i = 1000; i>200; i--){
      if(soundEnable)tone(tonepin,i);
      delay(5);
    }
    noTone(tonepin);
    delay(5000);
    cls();
    menuPrincipal();
  } 

}

