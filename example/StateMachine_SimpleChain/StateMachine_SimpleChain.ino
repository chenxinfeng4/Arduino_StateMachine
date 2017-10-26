/*
  Switch statement

 Demonstrates how to automaticlly log the State begin time..
 Demonstrates how the State Chain work.
 
  modified 26 Oct 2017
  by Xinfeng Chen
*/
#include "StateMachine.h"


/*               State Machine cloosed Chain

   ENTRY →----->> S1(data not logged) ------>> S2 ------
          ↑                                            ↓
           ----- C1S4 <<------------------------ S3 <<---
                                     (5 trials)  ↓
                                                EXIT
*/

State S1;        //no data logged
State S2(2);     //S2:xxxxx
State S3;        //after S3.setCiSi(3) -> S3:xxxxxx
State C1S4(1, 4);//C1S4:xxxxx

void constructStates();
void setup() {  
  // initialize
  Serial.begin(9600);
  
  //contruct States
  contructStates() ;

  // run State-Chain
  Serial.print("Entry State Chain!!\n\n");
  beginState(&S1);                          // begin dynamic running State-Chain
  Serial.print("\n\nExit State Chain!!");
}

void loop() {
}



void contructStates() {
  S3.setCiSi(3);
  
  // construct State: S1
  S1.doFunction = []()->void{                         // print a message
    Serial.print("It's S1=");
    Serial.println(State::millis());
  };
  S1.whenTime = []()->float {return 1;};              // wait 1sec, goto State:S2
  S1.whenTimeSTATE = []()->State* {return &S2;};

  // construct State: S2
  S2.doFunction = []()->void{                         // print a message
    Serial.print("It's S2=");
    Serial.println(State::millis());
  };
  S2.whenTime = []()->float {return 1;};
  S2.whenTimeSTATE = []()->State* {return &S3;};

  // construct State: S3
  S3.doFunction = []()->void{                         // print a message
    Serial.print("It's S3=");
    Serial.println(State::millis());  
  };
  S3.whenTime = []()->float {return 1;};
  S3.whenTimeSTATE = []()->State* {return &C1S4;};
  S3.whenTrial = []()->int {return 5;};
  S3.whenTrialSTATE =  []()->State* {return EXIT_STATE;}; // EXIT State Chain
  
  // construct State: C1S4
  C1S4.doFunction = []()->void{                       // print a message
    Serial.print("It's C1S4=");
    Serial.println(State::millis());
  };
  C1S4.whenTime = []()->float {return 1;};
  C1S4.whenTimeSTATE = []()->State* {return &S1;};
}

