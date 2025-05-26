// Gibt einen String aus PROGMEM auf dem LCD aus
void printProgmemString(const char* str) {
  char buffer[21];
  strncpy_P(buffer, str, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';
  lcd.print(buffer);
}

// Gibt einen Menüeintrag (aus String-Array in PROGMEM) auf dem LCD aus
void printMenuEntry(const char* const* menu, byte idx) {
  char buffer[21];
  strncpy_P(buffer, (char*)pgm_read_word(&(menu[idx])), sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';
  lcd.print(buffer);
}

// =======================
// Hauptmenü (Startmenü)
// =======================
void menuPrincipal(){

  digitalWrite(GREENLED, LOW); 
  digitalWrite(REDLED, LOW); 

  // Modus-Reset bei Neustart des Menüs
  currentGameMode = NONE;

  //Draw menu
  cls(); //clear lcd and set cursor to 0,0
  int i=0;

 // Ersten Menüeintrag anzeigen
  printMenuEntry(menu1, i);
  lcd.setCursor(10,1);
  checkArrows(i,4); // Pfeile/Scrollanzeigen
  while(1){

    var = keypad.waitForKey();
    // Navigation nach oben
    if(var == BT_UP && i>0){
      tone(tonepin,2400,30);
      i--;
      cls();
      printMenuEntry(menu1, i);
      checkArrows(i,3);
      delay(50);
    }

    // Navigation nach unten
    if(var == BT_DOWN && i<4){ // Maximale Menüzahl (4 Einträge)
      tone(tonepin,2400,30);
      i++;
      cls(); 
      printMenuEntry(menu1, i);    
      checkArrows(i,4);
      delay(50);
    }

    // Auswahl/OK
    if(var == BT_SEL){
      tone(tonepin,2400,30);
      cls();
      switch (i){

      case 0: // Search & Destroy
        currentGameMode = SEARCH_DESTROY;
        configQuickGame();
        startGameCount();
        search();
        break;

      case 1: // Sabotage
        currentGameMode = SABOTAGE;
        configQuickGame();
        startGameCount();
        sabotage();
        break;

      case 2: // Domination
        currentGameMode = DOMINATION;
        configQuickGame();
        startGameCount();
        domination();
        break;

      case 3: // Cut the Wire
         // Sicherheitsprüfung: Sind alle 4 Drahtkreise verbunden?
        {
          bool allConnected = true;
          for (int w = 0; w < 4; w++) {
            if (digitalRead(wirePins[w]) != LOW) {
              allConnected = false;
              break;
            }
        }
        if (!allConnected) {
          cls();
          lcd.setCursor(0, 0); lcd.print("FEHLER: Alle 4");
          lcd.setCursor(0, 1); lcd.print("Drähte verbinden!");
          delay(3000);
          cls();
          menuPrincipal(); // Menü neu aufrufen
          return;
        }
      }
      // Wenn alle verbunden, Spiel starten
      currentGameMode = CUT_THE_WIRE;
      configQuickGame();
      startGameCount();
      wire();
      break;


      case 4: // Config
        config();
        break;
      }
    }
  }
}


// =======================
// Einstellungen / Config-Menü
// =======================
void config(){
  lcd.clear();
  int i = 0;

  delay(50);
  printMenuEntry(menu2, i);
  checkArrows(i, 4);

  while (1) {
    var = keypad.waitForKey();

    // Menü-Navigation
    if (var == BT_UP && i > 0) {
      tone(tonepin, 2400, 30);
      i--;
      lcd.clear();
      printMenuEntry(menu2, i);
      checkArrows(i, 5);
      delay(50);
    }

    if (var == BT_DOWN && i < 5) {
      tone(tonepin, 2400, 30);
      i++;
      lcd.clear();
      printMenuEntry(menu2, i);
      checkArrows(i, 5);
      delay(50);
    }

    // Abbrechen/Zurück
    if (var == BT_CANCEL) {
      tone(tonepin, 2400, 30);
      menuPrincipal();
    }

    // Auswahl/OK
    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      lcd.clear();

      switch (i) {
        case 0:  // Hardwaretest: Piezo, LEDs, Sirene, Pyro
          cls(); printProgmemString(PIEZO_TEST_START); delay(3000);
          cls(); printProgmemString(PIEZO_ON); tone(tonepin, 2400, 2000); delay(2000);
          cls(); printProgmemString(PIEZO_OFF); noTone(tonepin); delay(2000);
          cls(); printProgmemString(PIEZO_ON); tone(tonepin, 2400, 2000); delay(2000);
          noTone(tonepin);
          cls(); printProgmemString(PIEZO_DONE); delay(2000);

          cls(); printProgmemString(LED_TEST_START); delay(3000);
          cls(); printProgmemString(LED_ON); digitalWrite(GREENLED, HIGH); digitalWrite(REDLED, HIGH); delay(3000);
          cls(); printProgmemString(LED_OFF); digitalWrite(GREENLED, LOW); digitalWrite(REDLED, LOW); delay(2000);
          cls(); printProgmemString(LED_DONE); delay(2000);

          cls(); printProgmemString(SIREN_TEST_START); delay(4000);
          cls(); printProgmemString(SIREN_ON); digitalWrite(mosfet, HIGH); delay(2000);
          cls(); printProgmemString(SIREN_OFF); digitalWrite(mosfet, LOW); delay(1000);
          cls(); printProgmemString(SIREN_DONE); delay(2000);

          cls(); printProgmemString(PYRO_TEST_START); delay(3000);
          cls(); printProgmemString(PYRO_ON); digitalWrite(mosfetpyro, HIGH); delay(4000);
          cls(); printProgmemString(PYRO_OFF); digitalWrite(mosfetpyro, LOW); delay(4000);
          cls(); printProgmemString(PYRO_DONE); delay(2000);
          config();
          break;

        case 1:  // LED TEST
          cls(); printProgmemString(LED_TEST_START); delay(3000);
          cls(); printProgmemString(LED_ON); digitalWrite(GREENLED, HIGH); digitalWrite(REDLED, HIGH); delay(3000);
          cls(); printProgmemString(LED_OFF); digitalWrite(GREENLED, LOW); digitalWrite(REDLED, LOW); delay(2000);
          cls(); printProgmemString(LED_DONE); delay(2000);
          config();
          break;

        case 2:  // SIREN TEST
          cls(); printProgmemString(SIREN_TEST_START); delay(4000);
          cls(); printProgmemString(SIREN_ON); digitalWrite(mosfet, HIGH); delay(3000);
          cls(); printProgmemString(SIREN_OFF); digitalWrite(mosfet, LOW); delay(1000);
          cls(); printProgmemString(SIREN_DONE); delay(2000);
          config();
          break;

        case 3:  // PYRO TEST
          cls(); printProgmemString(PYRO_TEST_START); delay(3000);
          cls(); printProgmemString(PYRO_ON); digitalWrite(mosfetpyro, HIGH); delay(4000);
          cls(); printProgmemString(PYRO_OFF); digitalWrite(mosfetpyro, LOW); delay(4000);
          cls(); printProgmemString(PYRO_DONE); delay(2000);
          config();
          break;

        case 4:  // Sprache (coming soon)
          cls(); printProgmemString(COMING_SOON); delay(3000);
          config();
          break;

        case 5:  // INFORMATION
          cls(); lcd.setCursor(0, 0); printProgmemString(VERSION_TEXT);
                 lcd.setCursor(0, 1); printProgmemString(BUILD_TEXT);
                 lcd.setCursor(0, 2); printProgmemString(DESIGN_TEXT);
          break;
      }
    }
  }
}


// =======================
// Konfiguration eines schnellen Spiels (ohne lange Menüs)
// =======================
void configQuickGame() {
  cls();

  // === Spielzeit einstellen ===
  if (currentGameMode != NONE) {
    menu1:
    cls();
    printProgmemString(GAME_TIME);
    delay(100);
    lcd.setCursor(0,1);
    lcd.print(F("00:00 hh:mm"));
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    byte var2=0;
    for(int i=0;i<4;i++){
      while(1){
        if(i==2 && var2==0){
          lcd.print(F(":"));
          var2=1;
        }
        byte varu = getRealNumber();
        if(varu != 11){
          time[i] = varu;
          lcd.print(varu);
          tone(tonepin,2400,30);
          break;
        }
      }
    }
    lcd.noCursor();
    lcd.noBlink();
    lcd.setCursor(13,1);
    lcd.print(F("ok?"));
    while(1){
      var = keypad.waitForKey();
      if(var == 'd') {
        tone(tonepin,2400,30);
        GAMEMINUTES = ((time[0]*600)+(time[1]*60)+(time[2]*10)+(time[3]));
        break;
      }
      if(var == 'c') {
        tone(tonepin,2400,30);
        goto menu1;
      }
    }
    tone(tonepin,2400,30);
    cls();
  }

  // === Bombenzeit einstellen (nur bei SEARCH_DESTROY/SABOTAGE) ===
  if(currentGameMode == SEARCH_DESTROY || currentGameMode == SABOTAGE){
    menu2:
    cls();
    printProgmemString(BOMB_TIME);
    delay(100);
    lcd.setCursor(0,1);
    printProgmemString(ZERO_MINUTES);
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    for(int i=0;i<2;i++){
      while(1){
        byte varu = getRealNumber();
        if(varu != 11){
          time[i] = varu;
          lcd.print(varu);
          tone(tonepin,2400,30);
          break;
        }
      }
    }
    lcd.noCursor();
    lcd.noBlink();
    lcd.setCursor(13,1);
    lcd.print(F("ok?"));
    while(1){
      var = keypad.waitForKey();
      if(var == 'd'){
        tone(tonepin,2400,30);
        BOMBMINUTES = ((time[0]*10)+(time[1]));
        break;
      }
      if(var == 'c'){
        tone(tonepin,2400,30);
        goto menu2;
      }
    }
    tone(tonepin,2400,30);
    cls();
  }

  // === Armierungszeit (Aktivierungszeit) ===
  if(currentGameMode != NONE){
    menu3:
    cls();
    printProgmemString(ARM_TIME);
    delay(100);
    lcd.setCursor(0,1);
    printProgmemString(ZERO_SECS);
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    for(int i=0;i<2;i++){
      while(1){
        byte varu = getRealNumber();
        if(varu != 11){
          time[i] = varu;
          lcd.print(varu);
          tone(tonepin,2400,30);
          break;
        }
      }
    }
    lcd.noCursor();
    lcd.noBlink();
    lcd.setCursor(13,1);
    lcd.print(F("ok?"));
    while(1){
      var = keypad.waitForKey();
      if(var == 'd'){
        tone(tonepin,2400,30);
        ACTIVATESECONDS = ((time[0]*10)+(time[1]));
        break;
      }
      if(var == 'c'){
        tone(tonepin,2400,30);
        goto menu3;
      }
    }
    tone(tonepin,2400,30);
    cls();
  }

  // === Sound aktivieren/deaktivieren ===
  if(currentGameMode != NONE){
    cls();
    printProgmemString(ENABLE_SOUND);
    lcd.setCursor(0,1);
    printProgmemString(YES_OR_NOT);
    while(1){
      var = keypad.waitForKey();
      if(var == 'a'){
        soundEnable = true;
        tone(tonepin,2400,30);
        break;
      }
      if(var == 'b'){
        soundEnable = false;
        tone(tonepin,2400,30);
        break;
      }
    }
  }

  // === Sirene (MOSFET) aktivieren ===
  if(currentGameMode == SEARCH_DESTROY || currentGameMode == SABOTAGE || currentGameMode == CUT_THE_WIRE){
    cls();
    printProgmemString(ENABLE_MOSFET);
    lcd.setCursor(0,1);
    printProgmemString(YES_OR_NOT);
    while(1){
      var = keypad.waitForKey();
      if(var == 'a'){
        mosfetEnable = true;
        tone(tonepin,2400,30);
        break;
      }
      if(var == 'b'){
        mosfetEnable = false;
        tone(tonepin,2400,30);
        break;
      }
    }
  }

  // === Pyro (MOSFET2) aktivieren ===
  if(currentGameMode == SEARCH_DESTROY || currentGameMode == SABOTAGE || currentGameMode == CUT_THE_WIRE){
    cls();
    printProgmemString(ENABLE_PYRO);
    lcd.setCursor(0,1);
    printProgmemString(YES_OR_NOT);
    while(1){
      var = keypad.waitForKey();
      if(var == 'a'){
        mosfetpyroEnable = true;
        tone(tonepin,2400,30);
        break;
      }
      if(var == 'b'){
        mosfetpyroEnable = false;
        tone(tonepin,2400,30);
        break;
      }
    }
  }

  // === Passwort aktivieren/setzen ===
  if(currentGameMode == SEARCH_DESTROY || currentGameMode == SABOTAGE || currentGameMode == CUT_THE_WIRE){
    cls();
    printProgmemString(ENABLE_CODE);
    lcd.setCursor(0,1);
    printProgmemString(YES_OR_NOT);
    while(1){
      var = keypad.waitForKey();
      if(var == 'a'){
        tone(tonepin,2400,30);
        setNewPass();
        passwordEnable = true;
        break;
      }
      if(var == 'b'){
        tone(tonepin,2400,30);
        passwordEnable = false;
        break;
      }
    }
    tone(tonepin,2400,30);
  }

  // === Cut The Wire: richtigen Draht auswählen ===
  if(currentGameMode == CUT_THE_WIRE){
    cls();
    lcd.setCursor(0, 0);
    printProgmemString(SELECT_CORRECT_WIRE);
    while(1){
      var = keypad.waitForKey();
      if(var >= '1' && var <= '4'){
        correctWire = var - '0';
        tone(tonepin,2400,30);
        break;
      }
    }
    cls();
  }
}
