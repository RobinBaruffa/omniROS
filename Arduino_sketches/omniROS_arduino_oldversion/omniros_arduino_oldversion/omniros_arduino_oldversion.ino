#include <PID_v1.h>
#include "pins_arduino.h"



volatile uint8_t *port_to_pcmask[] = {
  &PCMSK0,
  &PCMSK1,
  &PCMSK2};
static int PCintMode[24];
typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr PCintFunc[24] = { 
  NULL };
volatile static uint8_t PCintLast[3];
SIGNAL(PCINT0_vect) {
  PCint(0);
}
SIGNAL(PCINT1_vect) {
  PCint(1);
}
SIGNAL(PCINT2_vect) {
  PCint(2);
}

double computeSpeed(int motorNb){
  const int nbsteprevolution = 1398;
  const float pi=3.141592653;
  double newpos,newtime,w;
  newpos=pos[motorNb];
  newtime=millis();
  w=(((float)newpos-(float)oldpos[motorNb])*2*pi/(float)nbsteprevolution)/(((float)newtime-(float)oldtime[motorNb])/1000.0);
  oldtime[motorNb]=newtime;
  oldpos[motorNb]=newpos;
  return w;
}

////////////////////////////Variables/////////////////  //////////////
const int encoderA0=7;
const int encoderA1=8;
const int encoderB0=5;
const int encoderB1=6;
const int encoderC0=3;
const int encoderC1=4;

const int pinMotor[3][3]={
  {10,A1},
  {11,A3},
  {9,A0}};

unsigned long previousMicros;
int motorUpdateTime =0;
int count = 0;

volatile long pos[] = {0,0,0};
float oldpos[]={0,0,0};
float oldtime[]={0,0,0};

double pid[3] = {7.0,0.0,0.0};


double targetSpeed[3] = {0.0,0.0,0.0};
double measuredSpeed[3]= {0.0,0.0,0.0};
double PIDSpeed[3]= {0.0,0.0,0.0};
double motorSpeed[3]= {0.0,0.0,0.0};

PID pid1(&measuredSpeed[0], &PIDSpeed[0], &motorSpeed[0], pid[0], pid[1], pid[2], DIRECT);
PID pid2(&measuredSpeed[1], &PIDSpeed[1], &motorSpeed[1], pid[0], pid[1], pid[2], DIRECT);
PID pid3(&measuredSpeed[2], &PIDSpeed[2], &motorSpeed[2], pid[0], pid[1], pid[2], DIRECT);

/////////////////////////////Interrupts Service Routine ///////////////
void motorA(void) {
  if(digitalRead(encoderA0)==digitalRead(encoderA1)){
    pos[0]++;
  }
  else{
    pos[0]--;
  }
}

void motorB(void) {
  if(digitalRead(encoderB0)==digitalRead(encoderB1)){
    pos[1]++;
  }
  else{
    pos[1]--;
  }
}

void motorC(void) {
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
  Serial.begin(115200);
  pinMode(pinMotor[0][0], OUTPUT);
  pinMode(pinMotor[0][1], OUTPUT);
  pinMode(pinMotor[1][0], OUTPUT);
  pinMode(pinMotor[1][1], OUTPUT);
  pinMode(pinMotor[2][0], OUTPUT);
  pinMode(pinMotor[2][1], OUTPUT);
  
  pid1.SetMode(AUTOMATIC);
  pid1.SetOutputLimits(-255,255);
  
  pid2.SetMode(AUTOMATIC);
  pid2.SetOutputLimits(-255,255);

  pid3.SetMode(AUTOMATIC);
  pid3.SetOutputLimits(-255,255);
  
  
  pinMode(encoderA0, INPUT);
  pinMode(encoderB0, INPUT);
  pinMode(encoderC0, INPUT);
  pinMode(encoderA1, INPUT);
  pinMode(encoderB1, INPUT);
  pinMode(encoderC1, INPUT);
  PCattachInterrupt(encoderA0, motorA, CHANGE);
  PCattachInterrupt(encoderB0, motorB, CHANGE);
  PCattachInterrupt(encoderC0, motorC, CHANGE);

  
  
}

///////////////////////////////////loop///////////////////////////////////////////////

void loop(){
     
    String commandSpeed;
  char sz[] = "0.00000001,0.00000001,0.00000001";

  
  while (1){
    char buf[sizeof(sz)];
    char *p = buf;
    char *str;
    commandSpeed = Serial.readStringUntil('\n');
    commandSpeed.toCharArray(buf, sizeof(buf));
    if(buf[0]==':'){
      int j=0;
      while ((str = strtok_r(p, ",", &p)) != NULL){
        if(j==0){
          targetSpeed[j]=atof(str+1);
        }
        else{
          targetSpeed[j]=atof(str);
        }
        j++;
      }
    }

     float pi3 = 2.094395;
     float R=0.125;
     float r= 0.029;
     float sinpi3 = 0.8660254037;
     motorSpeed[0]=(targetSpeed[1]+R*targetSpeed[2])*1/r;
     motorSpeed[1]=(-sinpi3*targetSpeed[0]+0.5*targetSpeed[1]+R*targetSpeed[1])*1/r;
     motorSpeed[2]=(sinpi3*targetSpeed[0]+0.5*targetSpeed[1]+R*targetSpeed[2])*1/r;

      
     measuredSpeed[0]=computeSpeed(0);
     measuredSpeed[1]=computeSpeed(1);
     measuredSpeed[2]=computeSpeed(2);
     
     pid1.Compute();
     if(abs(measuredSpeed[0]-motorSpeed[0])> 0.1){
       if(PIDSpeed[0] > 0){
         digitalWrite(pinMotor[0][1], LOW);
         analogWrite(pinMotor[0][0], abs(PIDSpeed[0]));
       }
       else{
         digitalWrite(pinMotor[0][1], HIGH);
         analogWrite(pinMotor[0][0], abs(PIDSpeed[0]));
       }
     }
     pid2.Compute();
     if(abs(measuredSpeed[1]-motorSpeed[1])> 0.1){
       if(PIDSpeed[1] > 0){
         digitalWrite(pinMotor[1][1], LOW);
         analogWrite(pinMotor[1][0], abs(PIDSpeed[1]));
       }
       else{
         digitalWrite(pinMotor[1][1], HIGH);
         analogWrite(pinMotor[1][0], abs(PIDSpeed[1]));
       }
     }
     if(abs(measuredSpeed[2]-motorSpeed[2])> 0.1){
       pid3.Compute();
       if(PIDSpeed[2] > 0){
      
         digitalWrite(pinMotor[2][1], LOW);
         analogWrite(pinMotor[2][0], abs(PIDSpeed[2]));
       }
       else{
         digitalWrite(pinMotor[2][1], HIGH);
         analogWrite(pinMotor[2][0], abs(PIDSpeed[2]));
       }
      Serial.print(";");
      Serial.print(measuredSpeed[0]);
      Serial.print(",");
      Serial.print(measuredSpeed[1]);
      Serial.print(",");
      Serial.println(measuredSpeed[2]);

}
