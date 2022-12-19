//HC-05 Bluethooth
#include <SoftwareSerial.h>
#define rxPin 11   //HC-05 read
#define txPin 12   //HC-05 send
SoftwareSerial HC05(txPin, rxPin);
String text="";
char chracter;

//75HC595 with Elegoo Octacoupler
#define numOfRegister 1
#define numOfRegisterPins numOfRegister * 8
int latchPin=9; //RCLK
int clockPin=8; //SRCLK
int dataPin=10; //SER 
boolean registers [numOfRegisterPins];

//l289N
int track1pin1 = 4;
int track1pin2 = 2;
int speedPin1 =3;

//Model Train settings
int initSpeed=30;
int delayTime=5;
int maxSpeed=255;
int speed=0;
int floodGateRelay=0; // the relay that acts as floodgate

//FUNCTIONS --------------------------

//start train
int startUp(bool direction){
  if (direction==true){  
    digitalWrite(track1pin1, HIGH);
    digitalWrite(track1pin2, LOW);
  } else {
    digitalWrite(track1pin1, LOW);
    digitalWrite(track1pin2, HIGH);
  }
  analogWrite(speedPin1,initSpeed);
  for (int i=initSpeed;i<maxSpeed;i++){
    analogWrite(speedPin1,i);
    delay(delayTime);
  }
  return maxSpeed;
}

//Stop Train
void stopIt(int currentSpeed){
    for (int i=currentSpeed;i>initSpeed;i--){
      analogWrite(speedPin1,i);
      delay(delayTime);
    }
    digitalWrite(track1pin1, LOW);
    digitalWrite(track1pin2, LOW);
}

// switch a SwitchPoint
void executeSwitch(){
  //floodgate open
  registers[floodGateRelay] = false;
  writeRegisters();
  delay(300);
  //floodgate closed
  registers[floodGateRelay] = true;
  writeRegisters();
}

void writeRegisters(){
	digitalWrite(latchPin, LOW);
	for(int i = numOfRegisterPins-1; i >=  0; i--){
	  digitalWrite(clockPin, LOW); 
    int val = registers[i];
	  digitalWrite(dataPin, val);
	  digitalWrite(clockPin, HIGH);
	}
	digitalWrite(latchPin, HIGH);
}
// FUNCTIONS -----------------------------------------

void setup() {
  //bluethoth
  HC05.begin(38400);
  
  //L298N
  pinMode(track1pin1, OUTPUT);
  pinMode(track1pin2, OUTPUT);

  //75HC595
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //initiallize registers
	for(int i = numOfRegisterPins-1; i >=  0; i--){
	  registers[i] = HIGH;
	}
  writeRegisters();
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
          digitalWrite(track1pin1, LOW);
          digitalWrite(track1pin2, HIGH);
          analogWrite(speedPin1,speed);
        } else {
          speed=(text.substring(1,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
          digitalWrite(track1pin1, HIGH);
          digitalWrite(track1pin2, LOW);
          analogWrite(speedPin1,speed);
        }
      }
      //switche Points
      if (text.startsWith("sw")){
        int switchNumber=(text.substring(2,text.length()-1)).toInt();
        //switch flip the bit
        registers[switchNumber]=!registers[switchNumber];
        executeSwitch();
      }
    text="";
    }
  }
}