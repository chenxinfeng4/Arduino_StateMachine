# Arduino StateMachine Lib

Xinfeng Chen, chenxinfeng@hust.edu.cn, 2017

陈昕枫，华中科技大学，2017

## Basic Principle

This StateMachine Lib is a  program "control" structure, not a really machine. Each State is a stand-alone object that defines 1. do something (doFunction); 2. wait for something triggered (whenFunctions); 3. make a response by change itself's status to another corresponding State (transitionState).  By setting the properties of each State, and arranging States, the final State-Chain can handle complex logic-control programming.

![principle](http://img.image-storage.com/3372520490/f1a32d2941de4.png)

Demos shown below are all available in the "example" fold.



## 主要原理

在这里，"状态机" 是一种结构化编程，而不是一个机器实体。每一个状态单元是一个独立的对象，定义了1. 做什么事情; 2. 检测触发事件; 3. 通过改变自己到一下状态单元来相应的触发事件。通过设置每一个状态单元的属性，并把状态单元们合理排列，因此形成的状态机链条可以完成高度负责的逻辑编程。



演示数据如下，同时也在 "example" 文件中。



## demo "Blink" using StateMachine

```c++
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Similar the FILE/EXAMPLE/BASIC/BLINK, but only blink 10 trials.

  modified 26 Oct 2017
  by Xinfeng Chen
*/
#include "StateMachine.h"
State LEDon, LEDoff;

void setup() {
  pinMode(13, OUTPUT);
  
  //State: LEDon
  LEDon.doFunction     = ->void{digitalWrite(13, HIGH);}; //turn the LED on 
  LEDon.whenTime       = ->float{return 1;};           // wait for a second
  LEDon.whenTimeSTATE  = ->State *{return &LEDoff;};   // then goto State: LEDoff
  LEDon.whenTrial      = ->int{return 10;};            // the 10st (every 10) trial
  LEDon.whenTrialSTATE = ->State *{return EXIT_STATE;}; // exit State-Chain
  
  //State: LEDoff
  LEDoff.doFunction    = ->void{digitalWrite(13, LOW);};
  LEDoff.whenTime      = ->float{return 1;};
  LEDoff.whenTimeSTATE = ->State *{return &LEDon;};
  
  beginState(&LEDon); // begin dynamic running State-Chain, from LEDon until EXIT_STATE;
}
void loop() {
}
```



## demo "Switch"

![Switch](http://img.image-storage.com/734783758/fd28ea989ce64.gif)



```c++
#include "StateMachine.h"

State S_TrialBegin, S_Detecting, S_A0Detected, S_A1Detected,
      S_Timeout;

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
  beginState(&S_TrialBegin);    // begin dynamic running State-Chain
}

void loop() {
}

void contructStates() {
  // construct State: S_TrialBegin
  S_TrialBegin.doFunction = []()->void{            // give LED a blink
    delay(1000); 
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  };
  S_TrialBegin.whenTrial = []()->int{return 1;};   // directly goto State:S_Detecting
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
  S_Detecting.whenTime = []()->float {return 2;};  // wait 2sec, goto State:S_Timeout
  S_Detecting.whenTimeSTATE = []()->State* {return &S_Timeout;};

  // construct State: S_A0Detected
  S_A0Detected.doFunction = []()->void{
    Serial.println ("A0 detected!");
  };
  S_A0Detected.whenTrial = []()->int{return 1;};   // directly goto State:S_TrialBegin
  S_A0Detected.whenTrialSTATE = []()->State* {return &S_TrialBegin;};

  // construct State: S_A1Detected
  S_A1Detected.doFunction = []()->void{
    Serial.println ("A1 detected!");
  };
  S_A1Detected.whenTrial = []()->int{return 1;};      // directly goto State:S_TrialBegin
  S_A1Detected.whenTrialSTATE = []()->State* {return &S_TrialBegin;};

  // construct State: S_Timeout
  S_Timeout.doFunction = []()->void{
    Serial.println("Time out!");
  };
  S_Timeout.whenTime  = []()->float{return 0.001;};  // minor delay goto State: S_TrialBegin
  S_Timeout.whenTimeSTATE = []()->State* {return &S_TrialBegin;};
}
```

## GUI assitant

The textual frame (above), can be with rapid assistanted via graphic programming  (below).  See this [GUI programmer](https://github.com/chenxinfeng4/ArControl).



这些文本化的编程，可进一步借由图形操作完成其大致的框架。请看 [图形编程界面](https://github.com/chenxinfeng4/ArControl).

![GUI](http://img.image-storage.com/734783758/d8716e4cdab94.png)



![ino](http://img.image-storage.com/734783758/8136a46b51194.png)