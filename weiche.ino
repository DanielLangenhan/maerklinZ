int button =5;
int floodRelay= 6;
int weiche1= 7; //German for switch Point

bool direction= true;

void setup() {
  pinMode(floodRelay, OUTPUT);
  pinMode(weiche1, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  digitalWrite(floodRelay, LOW); //set initial power State
}

void loop() {
  if(digitalRead(button)==LOW){
    if (direction==true){
      digitalWrite(weiche1, LOW);
      digitalWrite(floodRelay, HIGH);
      delay(200);
      digitalWrite(floodRelay, LOW);
      direction=false;
    } else  {
      digitalWrite(weiche1, HIGH);
      digitalWrite(floodRelay, HIGH);
      delay(200);
      digitalWrite(floodRelay, LOW);
      direction=true;
    }
  }
}
