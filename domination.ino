// Hauptfunktion für den Domination-Spielmodus
void domination(){

  // === Setup / Initialisierung ===
  int minutos = GAMEMINUTES-1;    // Gesamtdauer (Minuten, -1 weil 0-basiert)
  boolean showGameTime=true;      // Umschalter für Anzeige (GameTime <-> Zonenzeit)
  unsigned long a;                // Hilfsvariable für Umschaltung
  unsigned long iTime=millis();   // Startzeit des Spiels
  unsigned long aTime;            // Aktuelle verstrichene Zeit
 
  // Spiel-Status zurücksetzen
  team=0;                         // 0 = neutral, 1 = grün, 2 = rot
  iZoneTime=0;                    // Startzeit der aktuellen Zone
  aTime=0;                        // Spielzeit-Reset
  redTime=0;                      // Zeit, die das rote Team die Zone kontrolliert hat
  greenTime=0;                    // Zeit, die das grüne Team die Zone kontrolliert hat

  int largoTono = 50;             // Tonlänge für Soundfeedback
  a=millis();

  // === Haupt-Spielschleife ===
  while(1)  // Endlosschleife für das laufende Spiel
  {
    // Spielende prüfen
    if(endGame){
      gameOver(); // Zeige Spielende-Logik & Siegeranzeige
    }
    
    keypad.getKey();  // Keypad abfragen (damit Events wie defuseando/cancelando erkannt werden)
    aTime=millis()- iTime; // Aktuelle Spielzeit berechnen

    // === LED Blink-Logik je nach Team-Status ===
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team==1)digitalWrite(GREENLED, HIGH);  
      if(team==2)digitalWrite(REDLED, HIGH);  
    }
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      if(team==1)digitalWrite(GREENLED, LOW);  
      if(team==2)digitalWrite(REDLED, LOW);
    }
    // Soundfeedback (verschiedene Warnungen je nach verbleibender Zeit)
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)tone(tonepin,tonoActivada,largoTono);

    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);

    // Umschalten zwischen Anzeige "GameTime" und Zonenzeit alle ~2 Sek.
    if(a+2000<millis()){
      a=millis();   
      showGameTime=!showGameTime;
      cls();
    }

    // === Anzeige-Logik ===
    if(showGameTime){ // Alle 2 Sekunden: Spielzeit
      lcd.setCursor(5,0);
      printProgmemString(GAME_TIME_TOP);
      lcd.setCursor(5,1);
      printTime(minutos, aTime);
    }
    else if (!showGameTime){ // Sonst: Zonen-Status (neutral/rot/grün) + kontrollierte Zeit

    lcd.setCursor(3,0);
    if(team == 0) printProgmemString(ZONE_NEUTRAL);
    if(team == 1) printProgmemString(ZONE_GREEN);
    if(team == 2) printProgmemString(ZONE_RED);

      if(team>0){
        lcd.setCursor(3,1);
        printTimeDom(millis()-iZoneTime,true);
      }
    }


    // === Check: Spielzeit abgelaufen? ===
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      gameOver();
    }

    // === ZONE NEUTRALISIEREN (Taste D oder C halten bei Team-Kontrolle) ===
    while((defuseando || cancelando) && team > 0)
    {
      cls();
      if(team>0)printProgmemString(ZONE_NEUTRALIZING);
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); // Start des Neutralisierens
      while(defuseando || cancelando)
      {
        // Spielzeit prüfen (falls Zeit während Neutralisation abläuft)
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        
        keypad.getKey();
        timeCalcVar = (millis()- xTime)%1000;

        // Akustisches Feedback beim Neutralisieren
        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(REDLED, LOW);
        }

        // Fortschrittsbalken (wie viel Prozent neutralisiert)
        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        // Wenn 100% erreicht -> Team verliert Kontrolle
        if(percent >= 100)
        {
          delay(1000);

          if(team==1){ 
            greenTime+=millis()-iZoneTime;
            iZoneTime=0; 

          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      cls();
    }

    // === ROTE ZONE ERFASSEN (Taste D, wenn neutral) ===
    while(defuseando && team == 0 )
    {
      cls();
      if(team==0)printProgmemString(ZONE_CAPTURING);
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)
      {
        keypad.getKey();
        // Zeitablauf prüfen
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        // Sound/LED Feedback
        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(REDLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        // Wenn 100% erreicht -> Team = rot, Timer starten
        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=2;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(REDLED, LOW);
    }

    // === GRÜNE ZONE ERFASSEN (Taste C, wenn neutral) ===
    while(cancelando && team == 0 )
    {
      cls();
      if(team==0)printProgmemString(ZONE_CAPTURING);
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(cancelando)
      {
        keypad.getKey();
        // Zeitablauf prüfen
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        // Sound/LED Feedback
        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(GREENLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        // Wenn 100% erreicht -> Team = grün, Timer starten
        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=1;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(GREENLED, LOW);  
    }
  }
}

// === Spielende-Logik ===
void gameOver(){

  // Restzeit der Zone zum jeweiligen Team addieren
  if(team==1)greenTime+=millis()-iZoneTime;
  if(team==2)redTime+=millis()-iZoneTime;
  digitalWrite(GREENLED, LOW);
  digitalWrite(REDLED, LOW);

  // Endlosschleife, bis Taste D gedrückt wird (defuseando)
  while(!defuseando){
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    lcd.clear();
    lcd.setCursor(3,0);
    printProgmemString(TIME_OVER);
    lcd.setCursor(0,1);

    // Sieger ermitteln und anzeigen
    if(greenTime>redTime){
      printProgmemString(GREEN_WIN);  // Grün gewinnt
      digitalWrite(GREENLED, HIGH);
    }
    else{
      //redteam wins 
      printProgmemString(RED_WIN);  // Rot gewinnt
      digitalWrite(REDLED, HIGH);
    }
    delay(3000);
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    // Zeige Rote Zeit
    cls();
    printProgmemString(RED_TIME_LABEL);
    lcd.setCursor(5,1);
    printTimeDom(redTime,false);
    delay(3000);
    keypad.getKey();
    if(defuseando){
      
      break;
    }
    // Zeige Grüne Zeit
    cls();
    printProgmemString(GREEN_TIME_LABEL);
    lcd.setCursor(5,1);
    printTimeDom(greenTime,false);
    delay(3000);
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
  }
  // Abfrage: Spiel erneut starten oder zurück ins Menü?
  cls();
  delay(100);
  printProgmemString(PLAY_AGAIN);
  lcd.setCursor(0,1);
  printProgmemString(YES_NO);
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      cls();
      domination();     // Neues Spiel starten
      break;
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();  // Zurück ins Hauptmenü
      break;
    }  
  } 
}
