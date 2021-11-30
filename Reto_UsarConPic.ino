#include <TimerOne.h>
#include <math.h>

#define Termistor A0
#define Actuador 10
#define Led_hot 9

  float e_0 = 0.0;
  float e_1 = 0.0;
  float e_2 = 0.0;

  float q_0, q_1, q_2;

  float r_1 = 0.0;
  float T_1 = 0.0;

  volatile float u_0 = 0.0;
  volatile float u_1 = 0.0;
  byte Ts = 10;

  float TempC;


  // VARIABLES PARA TERMISTOR NTC
  float Rt = 0.0;
  float R = 6500.0;
  float Rz = 46000.0;
  float VCC = 5;
  float B = 4101;
  float vol_term = 0.0;
  float temp = 23.0;
  

void PID () {
  e_0 = (r_1 - T_1);

  
  u_0 = u_1+q_0 * e_0+q_1 * e_1+q_2 * e_2;

  Serial.print("u_0->");

  Serial.println(u_0);
  
  if (u_0 >= 100.0){
        u_0 = 100.0;
    }
  
  if (u_0 <= 0.0 || r_1 == T_1){
        u_0 = 0.0;
    }
  
  e_2 = e_1;
  e_1 = e_0;
  u_1 = u_0;

  //Serial.print("\t");
  //Serial.println(map(u_0,  0,100,  0,255));
  analogWrite(Actuador,map(u_0,  0,100,  0,255));
  
}


void SampleTime() {
  //digitalWrite(Led_hot, !digitalRead(Led_hot));
  PID(); 
}

void setup() {

  float kp = 25;
  float ti = 1;
  float td = 0;

  pinMode(Termistor, INPUT);
  pinMode(Actuador, OUTPUT);
  
  pinMode(Led_hot,OUTPUT);
  digitalWrite(Led_hot,LOW);
  
  Serial.begin(9600);
  
  r_1 = 35.0; //Temperatura referencia
  
  Timer1.initialize(10000);
  Timer1.attachInterrupt(SampleTime);

  
  q_0 = kp * (1+Ts/(2.0 * ti)+td/Ts);
  q_1 = -kp * (1-Ts/(2.0 * ti)+(2.0 * td)/Ts);
  q_2 = (kp*td)/Ts;

}

void loop() {
  //float aux = 0.0;
  
  //for(int i = 0; i < 10; i++){

    /*
    TempC = float(analogRead(Termistor) * (500.0 / 1023.0) );
    */

    //vol_term = float(analogRead(Termistor));
    //vol_term = 5*(vol_term/1023);

    //Rt = (-R*vol_term)/(vol_term-VCC);
    //TempC = 1/((log(Rt/Rz)/B)+(1/(temp+273)));
    //TempC = TempC - 273.15;

    //aux = aux + TempC;
      
  //}
  
  //T_1 = aux/10.0;

  T_1 = 35.1;

  if (isnan(T_1)){
    T_1 = 0;
    }
  
  //Serial.println(T_1);
  Serial.println(Ts);
  //Serial.print("\t");
  //Serial.println(r_1); 
}
