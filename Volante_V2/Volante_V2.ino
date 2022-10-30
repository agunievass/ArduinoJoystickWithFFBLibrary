#include <SimpleKalmanFilter.h>
#include "Joystick.h"

int contador = 0; //vendría a ser el número de botón que pregunta el estado en el que está

//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 6, 0,//Buttons, Hat switchies
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
    pinMode(2, INPUT_PULLUP); //Boton cambio 1
    pinMode(3, INPUT_PULLUP); //Boton cambio 2
    pinMode(4, INPUT_PULLUP); //Boton 1
    pinMode(5, INPUT_PULLUP); //Boton 2
    pinMode(8, INPUT_PULLUP); //Boton 3
    pinMode(10, INPUT_PULLUP); //Boton 4

    
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
  int kalmanTune = 0.001;
  SimpleKalmanFilter kf = SimpleKalmanFilter(2, 2, kalmanTune);
  int value = kf.updateEstimate(analogRead(A2)); //Lee el valor del potenciometro (despues puedo probar dividir por 1024 y multiplicar por 100
  
  
  //set X Axis Spring Effect Param
  //Steering wheel
  myeffectparams[0].springMaxPosition = 512;
  myeffectparams[0].springPosition = value - 512; //-512-512

  int accel = analogRead(A3); //Lee el accelerador
  //accel = map(accel, 0, 1023, 0, 500);
  Joystick.setAccelerator(accel);
  int brak = analogRead(A1); //Lee el freno
  //brak = map(brak, 0, 1023, 0, 500);
  Joystick.setBrake(brak);
  
  //value = map(value, 0, 1023, 0, 500);
  Joystick.setXAxis(value);
  //Joystick.setYAxis(value);
  

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

  int pinButton_[6] = {2,3,4,5,8,10}; //pines de la placa donde van los botones
  
  //int button_1 = 0; //boton 1
  //int button_2 = 0; //boton 2
  //int button_3 = 0; //boton 3
  //int button_4 = 0; //boton 4
  //int right_change = 0; //cambio derecho palanca
  //int left_change = 0; //cambio izquierdo palanca
  
  
  while(contador <= 6){
    contador = contador + 1;
    //digitalWrite(17, HIGH);
    if(contador > 6){
      contador = 0;
    }
    int estado = !digitalRead(pinButton_[contador]); //Lee el estado del pin del item número"contador" del conjunto "pinButton_", 
    Joystick.setButton(contador, estado); //le envía a la pc que el botón numero"Contador" está en el estado "estado"
  }
  
    
  delay(5);
  digitalWrite(17, LOW);
}




