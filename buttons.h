// Process button inputs and return button activity

#define NUMBUTTONS 2
#define MODEBUTTON 3
#define BRIGHTNESSBUTTON 4

#define BTNIDLE 0
#define BTNDEBOUNCING 1
#define BTNPRESSED 2
#define BTNRELEASED 3
#define BTNLONGPRESS 4
#define BTNLONGPRESSREAD 5

#define BTNDEBOUNCETIME 20
#define BTNLONGPRESSTIME 1000

unsigned long buttonEvents[NUMBUTTONS];
byte buttonStatuses[NUMBUTTONS];
byte buttonmap[NUMBUTTONS] = {BRIGHTNESSBUTTON, MODEBUTTON};

void updateButtons() {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    switch(buttonStatuses[i]) {
      case BTNIDLE:
        if (digitalRead(buttonmap[i]) == LOW) {
          buttonEvents[i] = currentMillis;
          buttonStatuses[i] = BTNDEBOUNCING;
        }
      break;
      
      case BTNDEBOUNCING:
        if (currentMillis - buttonEvents[i] > BTNDEBOUNCETIME) {
          if (digitalRead(buttonmap[i]) == LOW) {
            buttonStatuses[i] = BTNPRESSED;
          }
        }
      break;
      
      case BTNPRESSED:
        if (digitalRead(buttonmap[i]) == HIGH) {
          buttonStatuses[i] = BTNRELEASED;
        } else if (currentMillis - buttonEvents[i] > BTNLONGPRESSTIME) {
            buttonStatuses[i] = BTNLONGPRESS;
        }
      break;
      
      case BTNRELEASED:
      break;
      
      case BTNLONGPRESS:
      break;
 
      case BTNLONGPRESSREAD:
        if (digitalRead(buttonmap[i]) == HIGH) {
          buttonStatuses[i] = BTNIDLE;
        }
      break;     
    }  
  }
}

byte buttonStatus(byte buttonNum) {

  byte tempStatus = buttonStatuses[buttonNum];
  if (tempStatus == BTNRELEASED) {
    buttonStatuses[buttonNum] = BTNIDLE;
  } else if (tempStatus == BTNLONGPRESS) {
    buttonStatuses[buttonNum] = BTNLONGPRESSREAD;
  }
  
  return tempStatus;

}

void doButtons() {
  // Check the mode button (for switching between effects)
  switch(buttonStatus(0)) {
    
    case BTNRELEASED: // button was pressed and released quickly
      cycleMillis = currentMillis; 
      if (++currentSet >= numSets) currentSet = 0; // loop to start of set list
      currentEffect = 0; // start at beginning of set
      effectInit = false; // trigger effect initialization when new effect is selected
    break;
    
  }
  
  // Check the brightness adjust button  
  switch(buttonStatus(1)) {
    
    case BTNRELEASED: // button was pressed and released quickly
      cycleBrightness();
    break;
    
    case BTNLONGPRESS: // button was held down for a while
      brightness = startbrightness;
      setBrightness(glassesRight, brightness);
      setBrightness(glassesLeft, brightness);
  
  }
}
