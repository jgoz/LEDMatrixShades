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
const char string6[] PROGMEM = "HARDER BETTER FASTER STRONGER    ";
const char string7[] PROGMEM = "Work it Harder  Make it Better    ";
const char string8[] PROGMEM = "Do it Faster  Makes us Stronger    ";
const char string9[] PROGMEM = "Our work is Never Over    ";
const char string10[] PROGMEM = "$$$$        CASH        $$$$        MONEY       ";
const char *const stringArray[] PROGMEM = {
    string0,
    string1,
    string2,
    string3,
    string4,
    string5,
    string6,
    string7,
    string8,
    string9,
    string10
};

MESSAGE_FN(msg_24kmagic, 0, SCROLL1X);
MESSAGE_FN(msg_soPlayer, 1, SCROLL1X);
MESSAGE_FN(msg_keepUp, 2, SCROLL1X);
MESSAGE_FN(msg_upAllNight, 3, SCROLL2X);
MESSAGE_FN(msg_comeTooFar, 4, SCROLL2X);
MESSAGE_FN(msg_raiseTheBar, 5, SCROLL2X);
MESSAGE_FN(msg_harderBetter, 6, SCROLL2X);
MESSAGE_FN(msg_workitHarder, 7, SCROLL2X);
MESSAGE_FN(msg_doitFaster, 8, SCROLL2X);
MESSAGE_FN(msg_neverOver, 9, SCROLL2X);
MESSAGE_FN(msg_dollars, 10, SCROLL1X);


// Set list
const byte numSets = 2;
const byte numEffects = 6;

functionList setList[numSets][numEffects] = {
    // MONEY
    {diamonds, msg_dollars, sparkles, diamonds, diamonds, rider},

    // LOVE
    {beatingHearts, scrollingHearts, beatingHearts, scrollingHearts, beatingHearts, scrollingHearts},
};

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
