//relays
int sideTrack= 7;

//botton
int button=4;

//setting
bool sideTrack1Power=false; //true power on track


void setup() {
  //relays
  pinMode(sideTrack, OUTPUT);
  //digitalWrite(sideTrack, HIGH);

  //button
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(button)==LOW){
    static unsigned long lastSwitch=0;
    unsigned long currentSwitch = millis();
    if (currentSwitch - lastSwitch >200){
      if (sideTrack1Power==true){
        digitalWrite(sideTrack, HIGH); //open power on side Track
      } else {
        digitalWrite(sideTrack, LOW); //close power on side Track

      }
      sideTrack1Power=!sideTrack1Power;
    }
    lastSwitch=currentSwitch;
  }
}