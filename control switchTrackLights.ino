//lights
int red=7;
int green=6;

//switch
int forward=2;
int backward=3;

//---------------- Functions 
void interRed(){
  Serial.println("forward");
  digitalWrite(red,HIGH);
  delay(500);
  digitalWrite(red,LOW);
}
void interGreen(){
  Serial.println("backward");
  digitalWrite(green,HIGH);
  delay(500);
  digitalWrite(green,LOW);
}
//---------------- Functions 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //LED
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  
  //SWITCH
  pinMode(forward, INPUT_PULLUP);
  pinMode(backward, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(forward)==LOW){
    interRed();
  }
  if (digitalRead(backward)==LOW){
    interGreen();
  }
}
