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

#include "StateMachine.h"

////////////////////////////////// Simple Chain(Node) ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
namespace STATEMACHINE_PRIVATE
{
  typedef boolean  (*T1)() ;
  typedef State *  (*T2)();
  class Node
  {
  private:
    unsigned int pos;                    //(position), current No. ITEM within CHAIN, 0~len-1
    unsigned int len;                    //(length),counts all ITEMs in CHAIN
    unsigned int lenstg;                 //(length storage) of CHAIN predistributed 
    T1 *Sub1;                            //array, store ITEMs-SUB1 
    T2 *Sub2;
    
  public:
    Node();
    ~Node();
    void addSubs(T1 t1, T2 t2);          // Append a new ITEM
    boolean Next();                      // Shift down an ITEM. Then judge whether available, Yes->true&shift, No->false&fresh 
    T1 getSub1();                        // Get current ITEM-SUB
    T2 getSub2();
    void setSub1(T1 sub);                // Set current ITEM-SUB
    void setSub2(T2 sub);
  };
}
using namespace STATEMACHINE_PRIVATE;

Node::Node(void)
{
    pos = 0;
    len = 1;
    lenstg = 1;
    Sub1 = new T1[lenstg];
    Sub2 = new T2[lenstg];
}

void Node::addSubs(T1 t1, T2 t2)
{
    if (len == lenstg) {
      lenstg = lenstg + 2;
      T1 *temp1 = new T1[lenstg];
      T2 *temp2 = new T2[lenstg];
      for (unsigned int i = 0; i < len; ++i) {
        temp1[i] = Sub1[i];
        temp2[i] = Sub2[i];
      }
      delete []Sub1; 
      delete []Sub2;
      Sub1 = temp1;
      Sub2 = temp2;
    }
    Sub1[len] = t1;
    Sub2[len] = t2;
    ++len;
}

boolean Node::Next(void)
{
    if (pos == len - 1) {
      pos = 0;
      return 0;
    }
    pos = (++pos) % len;
    return 1;
}

Node::~Node(){delete []Sub1; delete []Sub2;}
T1 Node::getSub1(void) {return Sub1[pos];}
T2 Node::getSub2(void) {return Sub2[pos];}
void Node::setSub1(T1 sub) {this->Sub1[pos] = sub;}
void Node::setSub2(T2 sub) {this->Sub2[pos] = sub;}




///////////////////////////////// define STATE-MACHINE ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void myVOIDNULL(void){ ; };
int myINTNULL(void){ return 0; };
boolean myBOOLNULL(void){ return false; };
float myFLOATNULL(void){ return 0; }
class State;
State * mySTATENULL(void);
State * mySTATENULL(void){ return NULL; };
State *State::NEXTSTATE = NULL;
unsigned long State::_begin_millis = NULL;
unsigned long State::_begin_micros = NULL;

unsigned long millis_Arduino(){return millis();}
unsigned long micros_Arduino(){return micros();}

void State::begin(State * entryState)
{
  State::NEXTSTATE = entryState;
  State::_begin_millis = millis_Arduino();
  State::_begin_micros = micros_Arduino();
  while (1) {
    if (State::NEXTSTATE != NULL) {       
      State::NEXTSTATE->RUN();
    }
    else {
      return;
    }
  }
}

void State::initMember()
{
    this->_CountSet = -1;              //Means need-to-init
    this->CountNow = 0; 
    this->dofun = myVOIDNULL;
    this->varListener = myBOOLNULL;
    this->varListenerSTATE = mySTATENULL;
    this->CountSet = myINTNULL;
    this->TimerSet = myFLOATNULL;
    this->evtListener = myBOOLNULL;
    this->CountSetSTATE = mySTATENULL;
    this->TimerSetSTATE = mySTATENULL; 
    this->evtListenerSTATE = mySTATENULL;
    this->myNode = new Node();
}

void State::serialLog()
{
  if(this->Ci == 0 && this->Si == 0) {
    return;
  }
  char temp[11], datachar[40]; 
  datachar[0] = '\0';
  if(this->Ci != 0) {
    strcat(datachar, "C");
    strcat(datachar, itoa(Ci, temp, 10));
  }
  strcat(datachar, "S");
  strcat(datachar, itoa(Si, temp, 10));
  strcat(datachar, ":");
  strcat(datachar, ultoa(State::millis(), temp, 10));
  strcat(datachar, "\n");
  Serial.print(datachar);
}

void State::RUN()
{
    //Start STATE timer
    unsigned long lifeEndTime = millis_Arduino() + this->TimerSet() * 1000;
    
    //popout State Infomation, if allowed
    this->serialLog();
    
    //run the <DO-Functions>
    dofun();
    
    //whether <varListener> fit
    if(this->varListener()) {
      State::NEXTSTATE = this->varListenerSTATE();    //STATE-transform
      return;
    }
        
    //whether <CountSet> fit
    if(this->_CountSet == -1) {                       //Means need-to-init
      this->_CountSet = this->CountSet();
    }
    this->CountNow++;
    if(this->_CountSet != 0 && this->CountNow == this->_CountSet) {
      this->_CountSet = this->CountSet();             //reset
      this->CountNow = 0;                             //reset
      State::NEXTSTATE = this->CountSetSTATE();       //STATE-transform
      return;
    }

    //whether <TimerSet> fit
    while(millis_Arduino() < lifeEndTime){

    //whether <evtListener> fit
      while(this->myNode->Next()) {
        if(this->myNode->getSub1()()) {
          State::NEXTSTATE = this->myNode->getSub2()();//STATE-transform
          return;
        }
      }
    }

  //It must be <TimerSet> fit
    State::NEXTSTATE = this->TimerSetSTATE();         //STATE-transform
    return;
}

State::State(){this->initMember();}
State::State(int si){this->setCiSi(si); this->initMember();}
State::State(int ci, int si){this->setCiSi(ci, si); this->initMember();}
State::~State(){delete this->myNode;}
void * State::setCiSi(int si){this->Ci = 0; this->Si = si;}
void * State::setCiSi(int ci, int si) {this->Ci = ci; this->Si = si;}
unsigned long State::millis(){return millis_Arduino() - State::_begin_millis;}
unsigned long State::micros(){return micros_Arduino() - State::_begin_micros;}
void State::addlisten(){this->myNode->addSubs(this->evtListener, this->evtListenerSTATE);}
