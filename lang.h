#pragma once
#include <avr/pgmspace.h>

//
// =============================
// === STARTBILDSCHIRM ========
// =============================
//
const char START_TITLE[]       PROGMEM = "4 DELTA 1";
const char START_SUBTITLE[]    PROGMEM = "Paintball System";
const char START_AUTHOR[]      PROGMEM = "By s0n11c3";
const char START_VERSION[]     PROGMEM = "v1";
const char GAME_TIME_TOP[]     PROGMEM = "GAME TIME";


//
// =============================
// === HAUPTMENÜ & EINSTELLUNGEN
// =============================
//
const char m1_0[] PROGMEM = "Search&Destroy";
const char m1_1[] PROGMEM = "Sabotage";
const char m1_2[] PROGMEM = "Domination";
const char m1_3[] PROGMEM = "Cut the Wire";
const char m1_4[] PROGMEM = "Settings";
const char* const menu1[] PROGMEM = {m1_0, m1_1, m1_2, m1_3, m1_4};

const char m2_0[] PROGMEM = "Hardware Test";
const char m2_1[] PROGMEM = "LED Test";
const char m2_2[] PROGMEM = "Siren Test";
const char m2_3[] PROGMEM = "Pyro Test";
const char m2_4[] PROGMEM = "Language";
const char m2_5[] PROGMEM = "Information";
const char* const menu2[] PROGMEM = {m2_0, m2_1, m2_2, m2_3, m2_4, m2_5};


//
// =============================
// === SYSTEMTESTS (CONFIG)
// =============================
//
const char PIEZO_TEST_START[] PROGMEM = "Start Piezo Test";
const char PIEZO_ON[]         PROGMEM = "Piezo Beep ON!";
const char PIEZO_OFF[]        PROGMEM = "Piezo Beep OFF!";
const char PIEZO_DONE[]       PROGMEM = "Piezo Test Finished";

const char LED_TEST_START[]   PROGMEM = "Start LED Test";
const char LED_ON[]           PROGMEM = "LED's ON!";
const char LED_OFF[]          PROGMEM = "LED's OFF!";
const char LED_DONE[]         PROGMEM = "LED TEST FINISHED!";

const char SIREN_TEST_START[] PROGMEM = "Start Siren Test";
const char SIREN_ON[]         PROGMEM = "Siren ON!";
const char SIREN_OFF[]        PROGMEM = "Siren OFF!";
const char SIREN_DONE[]       PROGMEM = "TEST FINISHED";

const char PYRO_TEST_START[]  PROGMEM = "Start Pyro Test";
const char PYRO_ON[]          PROGMEM = "Mosfet ON!";
const char PYRO_OFF[]         PROGMEM = "Mosfet OFF!";
const char PYRO_DONE[]        PROGMEM = "TEST FINISHED";

const char COMING_SOON[]      PROGMEM = "coming soon..!";
const char VERSION_TEXT[]     PROGMEM = "Version 1.0.0";
const char BUILD_TEXT[]       PROGMEM = "Build 000001";
const char DESIGN_TEXT[]      PROGMEM = "Designed by s0n11c3";


//
// =============================
// === SPIELKONFIGURATION ======
// =============================
//
const char GAME_TIME[]        PROGMEM = "Game Time:";
const char BOMB_TIME[]        PROGMEM = "Bomb Time:";
const char ZERO_MINUTES[]     PROGMEM = "00 minutes";
const char ARM_TIME[]         PROGMEM = "Arm Time:";
const char ZERO_SECS[]        PROGMEM = "00 seconds";
const char ENABLE_SOUND[]     PROGMEM = "Enable Sound?";
const char ENABLE_MOSFET[]    PROGMEM = "Enable Siren?";
const char ENABLE_PYRO[]      PROGMEM = "Enable Pyro?";
const char ENABLE_CODE[]      PROGMEM = "Enable Code Arm?";
const char YES_OR_NOT[]       PROGMEM = "A : Yes B : No";
const char SELECT_CORRECT_WIRE[] PROGMEM = "Correct Wire? 1-4";


//
// =============================
// === SPIELSTART (COUNTDOWN) ==
// =============================
const char GAME_READY[]       PROGMEM = "Ready to Begin";
const char PRESS_ANY_BUTTON[] PROGMEM = "Push ANY Button!";
const char GAME_STARTING[]    PROGMEM = "Starting Game";
const char COUNTDOWN_IN[]     PROGMEM = "IN ";


//
// =============================
// === PASSWORT-VERWALTUNG =====
// =============================
const char ENTER_NEW_PASS[]    PROGMEM = "Enter New Pass";
const char RETYPE_PASS[]       PROGMEM = "Retype Pass";
const char PASSWORD_SET_OK[]   PROGMEM = "Password Set OK!";
const char PASSWORD_ERROR[]    PROGMEM = "ERROR Dont Match!";


//
// =============================
// === IN-GAME TEXTE ===========
// =============================
const char ENTER_CODE[]        PROGMEM = "Enter Code";
const char CODE_ERROR[]        PROGMEM = "Code Error!";
const char BOMB_ARMED[]        PROGMEM = "BOMB ARMED";
const char DETONATION_IN[]     PROGMEM = "DETONATION IN";
const char DISARMING_BOMB[]    PROGMEM = "DISARMING BOMB";
const char DISARMING[]         PROGMEM = "DISARMING";
const char TIME_LEFT[]         PROGMEM = "Time left:";

const char DEFENDERS_WIN[]    PROGMEM = " DEFENDERS WIN ";
const char DISARM[]           PROGMEM = "DISARM";
const char ARMING_BOMB[]      PROGMEM = "ARMING BOMB";
const char GAME_OVER[]         PROGMEM = "GAME OVER";


const char ZONE_NEUTRAL[]      PROGMEM = "NEUTRALE ZONE";
const char ZONE_GREEN[]        PROGMEM = " GREEN ZONE";
const char ZONE_RED[]          PROGMEM = "  RED ZONE";
const char ZONE_NEUTRALIZING[] PROGMEM = "NEUTRALIZING...";
const char ZONE_CAPTURING[]    PROGMEM = " CAPTURING ZONE";

const char TIME_OVER[]         PROGMEM = "TIME OVER!";
const char GREEN_WIN[]         PROGMEM = " GREEN TEAM WIN";
const char RED_WIN[]           PROGMEM = "  RED TEAM WIN";
const char RED_TIME_LABEL[]    PROGMEM = "Red Time:";
const char GREEN_TIME_LABEL[]  PROGMEM = "Green Time:";


//
// =============================
// === SPIELENDE ===============
// =============================
const char BOMB_DISARMED[]     PROGMEM = "BOMB DISARMED";
const char GOODS_WIN[]         PROGMEM = "GOODS WIN";
const char TERRORISTS_WIN[]    PROGMEM = "TERRORISTS WIN";
const char GAME_OVER_TEXT[]    PROGMEM = "GAME OVER";
const char PLAY_AGAIN[]        PROGMEM = "Play Again?";
const char YES_NO[]            PROGMEM = "A : Yes B : No";


//
// =============================
// === HILFSFUNKTIONEN =========
// =============================

void printProgmemString(const char* str);
void printMenuEntry(const char* const* menu, byte idx);
