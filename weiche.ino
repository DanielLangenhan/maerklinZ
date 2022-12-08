int button =5;
int floodRelay= 6;
int weiche1= 7; //German for switch Point

bool direction= true;

void setup() {
  pinMode(floodRelay, OUTPUT);
  pinMode(weiche1, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  digitalWrite(floodRelay, LOW); //set initial power State
  digitalWrite(weiche1, LOW);  //set initial power State
}

void loop() {
  if(digitalRead(button)==LOW){
    if (direction==true){
      digitalWrite(floodRelay, HIGH);
      digitalWrite(weiche1, LOW);
      delay(200);
      digitalWrite(floodRelay, LOW);
      digitalWrite(weiche1, HIGH);
      direction=false;
    } else  {
      digitalWrite(floodRelay, HIGH);
      digitalWrite(weiche1, HIGH);
      delay(200);
      digitalWrite(floodRelay, LOW);
      direction=true;
    }
  }
}