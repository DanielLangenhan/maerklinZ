//Define start values
//l289N
int motor1pin1 = 2;
int motor1pin2 = 4;
int speedPin = 3; // frequency out Pin

//initSpeed
int initSpeed=30; //lower and the tarins makes some funny noice
//delay
int delayTime=5; // how slow to run up the train

//functions

void startUp(bool direction){
  if (direction==true){  
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  } else {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);

  }
  analogWrite(speedPin,initSpeed);
  for (int i=initSpeed;i<255;i++){
    analogWrite(speedPin,i);
    delay(delayTime);
  }
}

void stopIt(){
    for (int i=255;i>initSpeed;i--){
      analogWrite(speedPin,i);
      delay(delayTime);
    }
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
}


void setup() {
  //initil settingings
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT); 
}

void loop() {
  delay(5000); //wait 10 seconds
  // run it up
  startUp(true);
  delay(15000); //run for 30 seconds
  //stop
  stopIt();
  delay(5000); //hold for !5 seconds
  // run the other way
  startUp(false);
  delay(15000); //run for 30 seconds
  //stop
  stopIt();
  delay(5000); //run for 30 seconds
}