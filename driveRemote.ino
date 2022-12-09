#define rxPin 11   //HC-05 read
#define txPin 12   //HC-05 send

//HC-05 Bluethooth
#include <SoftwareSerial.h>
SoftwareSerial HC05(txPin, rxPin);

//l289N
int motor1pin1 = 2;
int motor1pin2 = 4;
int speedPin =3;

//setting
int initSpeed=30;
int delayTime=5;
int maxSpeed=255;
int speed=0;
String text="";
char chracter;

int startUp(bool direction){
  if (direction==true){  
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  } else {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
  }
  analogWrite(speedPin,initSpeed);
  for (int i=initSpeed;i<maxSpeed;i++){
    analogWrite(speedPin,i);
    delay(delayTime);
  }
  return maxSpeed;
}

void stopIt(int currentSpeed){
    for (int i=currentSpeed;i>initSpeed;i--){
      analogWrite(speedPin,i);
      delay(delayTime);
    }
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
}

void setup() {
  //bluethoth
  HC05.begin(38400);
  
  //L298N
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
}

void loop() {
  while(HC05.available() > 0){
    chracter=HC05.read();
    text.concat(chracter);
    if (chracter=='\n'){
      //change maxSpeed
      if (text.startsWith("sMax")){
        maxSpeed=(text.substring(4,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
      }
      //change delay
      if (text.startsWith("sDel")){
        delayTime=(text.substring(4,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
      }
      //forward
      if (text.startsWith("forw")){
        speed=startUp(true);
      }
      //stop
      if (text.startsWith("stop")){
        stopIt(speed);
      }
      //backward
      if (text.startsWith("back")){
        speed=startUp(false);
      }
      //manual drive
      if (text.startsWith("m")){
        if (text.indexOf("-")>0){
          speed=(text.substring(2,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, HIGH);
          analogWrite(speedPin,speed);
        } else {
          speed=(text.substring(1,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW);
          analogWrite(speedPin,speed);
        }
      }
    text="";
    }
  }
}