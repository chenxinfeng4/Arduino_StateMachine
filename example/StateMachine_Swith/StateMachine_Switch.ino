/*
  Switch statement

 Demonstrates the use of a switch statement.
 
 User should put A0 and A1 to a swticher individually. Each trail begin
 with a LED13 blink, end up with 4 case:
 [1] IF detect A0==LOW -> induce series statements-1;
 [2] ELSE IF detect A1==LOW -> induce series statements-2;
 [3] ELSE IF waited  above 2sec -> induce serial statements-3; 
 [4] AFTER every 5 times of the case [3] -> induce serial statements-4.

  modified 26 Oct 2017
  by Xinfeng Chen
 */
#include "StateMachine.h"

State S_TrialBegin, S_Detecting, S_A0Detected, S_A1Detected,
      S_Timeout, S_5times;

void constructStates();
void setup() {  
  // initialize
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  Serial.begin(9600);
  
  //contruct States
  contructStates() ;

  // run State-Chain
  beginState(&S_TrialBegin);                                           // begin dynamic running State-Chain
}

void loop() {
}



void contructStates() {
  // construct State: S_TrialBegin
  S_TrialBegin.doFunction = []()->void{                         // give LED a blink
    delay(1000); 
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  };
  S_TrialBegin.whenTrial = []()->int{return 1;};                 // directly goto State:S_Detecting
  S_TrialBegin.whenTrialSTATE = []()->State* {return &S_Detecting;};

  // construct State: S_TrialBegin
  S_Detecting.doFunction = []()->void{ 
    Serial.println ("Detecting A0 and A1...");
  };
  S_Detecting.whenLoop = []()->bool {return digitalRead(A0)==LOW;}; // detecting A0, goto State:S_A0Detected
  S_Detecting.whenLoopSTATE = []()->State* {return &S_A0Detected;};
  S_Detecting.commitLoop();  //commit adding such evt-listener
  S_Detecting.whenLoop = []()->bool {return digitalRead(A1)==LOW;}; // detecting A1, goto State:S_A1Detected
  S_Detecting.whenLoopSTATE = []()->State* {return &S_A1Detected;};
  S_Detecting.commitLoop();  //commit adding
  S_Detecting.whenTime = []()->float {return 2;};                    // wait 2sec, goto State:S_Timeout
  S_Detecting.whenTimeSTATE = []()->State* {return &S_Timeout;};

  // construct State: S_A0Detected
  S_A0Detected.doFunction = []()->void{
    Serial.println ("A0 detected!");
  };
  S_A0Detected.whenTrial = []()->int{return 1;};                      // directly goto State:S_TrialBegin
  S_A0Detected.whenTrialSTATE = []()->State* {return &S_TrialBegin;};

  // construct State: S_A1Detected
  S_A1Detected.doFunction = []()->void{
    Serial.println ("A1 detected!");
  };
  S_A1Detected.whenTrial = []()->int{return 1;};                      // directly goto State:S_TrialBegin
  S_A1Detected.whenTrialSTATE = []()->State* {return &S_TrialBegin;};

  // construct State: S_Timeout
  S_Timeout.doFunction = []()->void{
    Serial.println("Time out!");
  };
  S_Timeout.whenTrial = []()->int{return 5;};                         // every 5 times goto State:S_TrialBegin
  S_Timeout.whenTrialSTATE = []()->State* {return &S_5times;};
  S_Timeout.whenTime  = []()->float{return 0.001;};                   // minor delay goto State: S_TrialBegin
  S_Timeout.whenTimeSTATE = []()->State* {return &S_TrialBegin;};

  // construct State:S_5times
  S_5times.doFunction = []()->void{
    Serial.println("Counting timeout 5times!");
  };
  S_5times.whenTrial = []()->int{return 1;};                           // directly goto State:S_TrialBegin
  S_5times.whenTrialSTATE = []()->State* {return &S_TrialBegin;};
}

