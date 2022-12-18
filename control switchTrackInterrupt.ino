//switch
volatile int forward=2;
volatile int backward=3;

//---------------- Functions 
void interRed(){
  static unsigned long lastRed=0;
  unsigned long currentRed = millis();
  if (currentRed - lastRed >200){
    Serial.println("forward");
  }
  lastRed=currentRed;
}
void interGreen(){
  static unsigned long lastGreen=0;
  unsigned long currentGreen = millis();
  if (currentGreen - lastGreen >200){
    Serial.println("backward");
    }
  lastGreen=currentGreen;
}
//---------------- Functions 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //SWITCH
  pinMode(forward, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(forward), interRed, LOW);
  pinMode(backward, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(backward), interGreen, LOW);
}

void loop() {

}
