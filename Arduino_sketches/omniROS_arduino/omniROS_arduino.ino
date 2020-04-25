/*
 * This is the omniROS arduino sketch, it computes the angular velocity of the three wheels
 * and send it back to the esp8266 via serial 
*/

#include "PinChangeInterrupt.h"
#define pi 3.141592653
#define nbsteprevolution 1398
////////////////////////////Variables/////////////////  //////////////
const short encoderA0=2;
const short encoderA1=3;
const short encoderB0=4;
const short encoderB1=5;
const short encoderC0=6;
const short encoderC1=7;

volatile short pos[] = {0,0,0};

unsigned long oldtime[]={0,0,0};

/////////////////////////////Interrupt Service Routine ///////////////

void motorA(void){
  if(digitalRead(encoderA0)==digitalRead(encoderA1)){
    pos[0]++;
  }
  else{
    pos[0]--;
  }
}

void motorB(void){
  if(digitalRead(encoderB0)==digitalRead(encoderB1)){
    pos[1]++;
  }
  else{
    pos[1]--;
  }
}

void motorC(void){
  if(digitalRead(encoderC0)==digitalRead(encoderC1)){
    pos[2]++;
  }
  else{
    pos[2]--;
  }
}

////////////////////////////////////Setup////////////////////////////////////////////
void setup()
{

  pinMode(encoderA0, INPUT_PULLUP);
  pinMode(encoderA1, INPUT_PULLUP);
  pinMode(encoderB0, INPUT_PULLUP);
  pinMode(encoderB1, INPUT_PULLUP);
  pinMode(encoderC0, INPUT_PULLUP);
  pinMode(encoderC1, INPUT_PULLUP);

  attachPCINT(digitalPinToPCINT(encoderA0), motorA, CHANGE); 
  attachPCINT(digitalPinToPCINT(encoderB0), motorB, CHANGE); 
  attachPCINT(digitalPinToPCINT(encoderC0), motorC, CHANGE);  
  Serial.begin(9600);
  delay(200);
}

///////////////////////////////////loop///////////////////////////////////////////////

void loop(){
  float speedA = computeSpeed(0);
  float speedB = computeSpeed(1);
  float speedC = computeSpeed(2);
  String packet = String(speedA) + ',' + String(speedB) + ',' + String(speedC) + "\n";  //Create a packet with the 3 speeds in csv format ending with \n
  Serial.print(packet);
}
