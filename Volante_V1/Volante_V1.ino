#include "Joystick.h"

//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 4, 0,//Buttons, Hat switchies
  true, true, false, //X,Y,Z
  false, false, false,//Rx,Ry,Rz
  false, false, true, true, false);


Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};



void setup(){
    pinMode(A1,INPUT); //Entrada Potenciometro del freno
    pinMode(A2,INPUT); //Entrada Potenciometro del volante
    pinMode(A3,INPUT); //Entrada Potenciometro del acelerador
    pinMode(9,OUTPUT); //Salida PWM
    pinMode(6,OUTPUT); //Enable Right spin
    pinMode(7,OUTPUT); //Enable Left spin
    pinMode(17, OUTPUT); //Led indicador que está haciendo cosas
    Joystick.setXAxisRange(-512, 512);
    //set X Axis gains
    mygains[0].totalGain = 70;//0-100
    mygains[0].springGain = 70;//0-100
    //enable gains REQUIRED
    Joystick.setGains(mygains);
    Joystick.begin();
}


void loop(){
  int value = analogRead(A2); //Lee el valor del potenciometro
  //set X Axis Spring Effect Param
  //Steering wheel
  myeffectparams[0].springMaxPosition = 512;
  myeffectparams[0].springPosition = value - 512; //-512-512


  int accel = analogRead(A3);
  Joystick.setAccelerator(accel);
  int brak = analogRead(A1);
  Joystick.setBrake(brak);
  
  Joystick.setXAxis(value);

  //Recv HID-PID data from PC and caculate forces
  //Steering wheel
  //Joystick.setXAxis(value - 512);
  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  if(forces[0] > 0){
    digitalWrite(6,LOW);
    digitalWrite(17, HIGH);
    digitalWrite(7,HIGH);
    analogWrite(9,abs(forces[0]));
  }else{
    digitalWrite(6,HIGH);
    digitalWrite(17, HIGH);
    digitalWrite(7,LOW);
    analogWrite(9,abs(forces[0]));
  }
  delay(5);
  digitalWrite(17, LOW);
}




