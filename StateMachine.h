/*
  StateMachine.h

  Copyright (c) 2017, Xinfeng Chen. 
  chenxinfeng@hust.edu.cn

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 */
 
#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "Arduino.h"

#define doFunction      dofun
#define whenTrial       CountSet
#define whenTrialSTATE  CountSetSTATE
#define whenTime        TimerSet
#define whenTimeSTATE   TimerSetSTATE
#define whenOnce        varListener
#define whenOnceSTATE   varListenerSTATE
#define whenLoop        evtListener
#define whenLoopSTATE   evtListenerSTATE
#define commitLoop      addlisten
#define EXIT_STATE      0

////////////////////////////////// Simple Chain(Node) invisible to user///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

namespace STATEMACHINE_PRIVATE
{
  class Node;
}


///////////////////////////////// define STATE-MACHINE visible to user/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
class State
{
public:
  static void begin(State *);          // !!! Begin from certain State, and then run STATE-MACHINE by looping "State::NEXTSTATE->RUN()" 
  static unsigned long millis();       //millis() from State::begin();
  static unsigned long micros();       //micros() from State::begin();
  State();                             //Initialization, but DONOT popout State infomation.
  State(int si);                       //Allow popout State infomation from Serial, like "S7:12345", where SI was set to "7", "12345"ms from State::begin()
  State(int ci, int si);               //Allow popout State infomation from Serial, like "C3S7:12345", where CI was set to "3". 
  void * setCiSi(int ci, int si);      // Reset the mark (CISI) to this State, only for popout State infomation.
  void * setCiSi(int si);              // Reset the mark (SI) to this State, only for popout State infomation.
  void addlisten();                    // Append a new pair of evtListener, achive mult-evtListener.
  void RUN();                          // !!! Core FUNC. of STATE to run current State, and then change the handle of NEXTSTATE
  ~State();                            //Destructor
  
public:                                //basic members for a State Frame
  int Ci,Si;                           //Mark of each STATE that allow popout State infomation from Serial
  void (*dofun)(void);                 //FUNC.P.; DO-Function when STATE entry
  boolean (*varListener)(void);        //FUNC.P.; Listen-Event when STATE entry, once only
  State * (*varListenerSTATE)(void);   //FUNC.P.; corresponding transform STATE
  int (*CountSet)(void);               //FUNC.P.; COUNTER for STATE max count
  State * (*CountSetSTATE)(void);      //FUNC.P.; corresponding transform STATE
  float (*TimerSet)(void);             //FUNC.P.; TIMER for STATE max duration, unit = s
  State * (*TimerSetSTATE)(void);      //FUNC.P.; corresponding transform STATE
  boolean (*evtListener)(void);        //FUNC.P.; Listen-Event when STATE entry, loop
  State * (*evtListenerSTATE)(void);   //FUNC.P.; corresponding transform STATE
  
private:
  static State * NEXTSTATE;            //NEXT STATE to be run 
  static unsigned long _begin_millis;  //millis() when State::begin();
  static unsigned long _begin_micros;  //micros() when State::begin();
  unsigned int CountNow;               //current count
  int _CountSet;                       //COUNTER for STATE max count
  STATEMACHINE_PRIVATE::Node  *myNode; //EVT for STATE trigger
  void initMember();                   //Within constructor function
  void serialLog();                    //Popout State infomation from Serial
};

inline beginState(State *entryState) {State::begin(entryState);}
#endif
