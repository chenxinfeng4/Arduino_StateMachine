/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Similar the FILE/EXAMPLE/BASIC/BLINK, but only blink 10 trials.

  modified 26 Oct 2017
  by Xinfeng Chen
 */
#include "StateMachine.h"

State LEDon, LEDoff;
void turnOn() {digitalWrite(13, HIGH);}
float turnOnTime() { return 1;}
State * turnOnGoto() {return &LEDoff;}
void turnOff() {digitalWrite(13, LOW);}
float turnOffTime() {return 1;}
State * turnOffGoto() {return &LEDon;}
int trialSet() {return 10;}
State * trialSetGoto() {return EXIT_STATE;}

void setup() {  
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  // construct State-Chain
  /* Version 1*/
  LEDon.doFunction     = turnOn;                                // turn the LED on (HIGH is the voltage level)
  LEDon.whenTime       = turnOnTime;                            // wait for a second
  LEDon.whenTimeSTATE  = turnOnGoto;                            // then goto State: LEDoff
  LEDon.whenTrial      = trialSet;                              // the 10st trial
  LEDon.whenTrialSTATE = trialSetGoto;                          // exit State-Chain

  LEDoff.doFunction    = turnOff;                               // turn the LED off by making the voltage LOW
  LEDoff.whenTime      = turnOffTime;                           // wait for a second
  LEDoff.whenTimeSTATE = turnOffGoto;                           // then goto State: LEDon
    
  /* Version 2, alternative */
//  LEDon.doFunction     = []()->void{digitalWrite(13, HIGH);}; 
//  LEDon.whenTime       = []()->float{return 1;};
//  LEDon.whenTimeSTATE  = []()->State *{return &LEDoff;};
//  LEDon.whenTrial      = []()->int{return 10;};
//  LEDon.whenTrialSTATE = []()->State *{return EXIT_STATE;};
//
//  LEDoff.doFunction    = []()->void{digitalWrite(13, LOW);};
//  LEDoff.whenTime      = []()->float{return 1;};
//  LEDoff.whenTimeSTATE = []()->State *{return &LEDon;};
  

  // run State-Chain
  beginState(&LEDon);                                           // begin dynamic running State-Chain, from LEDon until EXIT_STATE;
}

void loop() {
}

