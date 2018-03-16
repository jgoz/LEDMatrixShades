// AS1130 LED Shades
// Designed for 8MHz 3.3V ATMega328P
// Garrett Mace
// 2015 macetech LLC

#include <Arduino.h>
#include <Wire.h>         // I2C library
#include <avr/pgmspace.h> // Flash storage of long strings and fonts

// Initial settings
#define CYCLETIME 10000
#define startbrightness 153

// Global variables
unsigned long currentMillis = 0;
unsigned long cycleMillis = 0;
unsigned long effectMillis = 0;
byte currentSet = 0;
byte currentEffect = 0;

// Data tables stored in flash memory
#include "mathtables.h"    // Useful tables of values
#include "pcbiosfont.h"    // 8x8 bitmap font
#include "graphicframes.h" // bitmap graphics

// Helper functions for AS1130 interfacing and graphics generation
#include "AS1130functions.h"  // Generic AS1130 functions
#include "glassesfunctions.h" // Glasses-specifc functions
#include "effects.h"          // Graphical patterns
#include "buttons.h"          // button handler

// Begin program
void setup()
{
  Wire.begin(); // Start I2C
  TWBR = 2;     // Set to 400KHz bus speed (on 8MHz device)

  glassesInit(); // Preconfigure AS1130 chips

  pinMode(MODEBUTTON, INPUT_PULLUP);       // Prepare button input
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP); // Prepare button input
}

// Scrolling messages
#define MESSAGE_FN(name, index, mode) \
  void name() { scrollMessage(index, mode); }

const char string0[] PROGMEM = "24K Magic  ";
const char string1[] PROGMEM = "So Player  ";
const char string2[] PROGMEM = "Keep UP  ";
const char string3[] PROGMEM = "We're Up All Night    ";
const char string4[] PROGMEM = "We've Come Too Far    ";
const char string5[] PROGMEM = "Let's Raise The Bar    ";
const char *const stringArray[] PROGMEM = {
    string0,
    string1,
    string2,
    string3,
    string4,
    string5,
};

MESSAGE_FN(msg_24kmagic, 0, SCROLL1X);
MESSAGE_FN(msg_soPlayer, 1, SCROLL1X);
MESSAGE_FN(msg_keepUp, 2, SCROLL1X);
MESSAGE_FN(msg_upAllNight, 3, SCROLL2X);
MESSAGE_FN(msg_comeTooFar, 4, SCROLL2X);
MESSAGE_FN(msg_raiseTheBar, 5, SCROLL2X);

// Set list
const byte numSets = 3;
const byte numEffects = 6;

functionList setList[numSets][numEffects] = {
    // love/intro
    {beatingHearts, scrollingHearts, beatingHearts, scrollingHearts, beatingHearts, scrollingHearts},
    // 24K Magic
    {sparkles, msg_24kmagic, sparkles, msg_soPlayer, sparkles, msg_keepUp},
    // Get Lucky
    {rider, msg_upAllNight, rider, msg_comeTooFar, rider, msg_raiseTheBar}};

// Main loop
void loop()
{
  currentMillis = millis();
  updateButtons();
  doButtons();

  if (setList[currentSet][currentEffect] == rider || setList[currentSet][currentEffect] == sparkles)
    fadeAllPWM();

  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > CYCLETIME)
  {
    cycleMillis = currentMillis;
    if (++currentEffect >= numEffects)
      currentEffect = 0; // loop to start of effect list
    effectInit = false;  // trigger effect initialization when new effect is selected
  }

  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay)
  {
    effectMillis = currentMillis;
    setList[currentSet][currentEffect](); // run the selected effect function
  }
}
