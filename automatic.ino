//HC-05 Bluethooth
#include <SoftwareSerial.h>
#define rxPin 11   //HC-05 read
#define txPin 12   //HC-05 send
SoftwareSerial HC05(txPin, rxPin);
String text="";
char chracter;

//75HC595 with Elegoo Octacoupler
#define numOfBanks 2
#define numOfRelays numOfBanks * 8
int latchPin=9; //RCLK
int clockPin=8; //SRCLK
int dataPin=10; //SER 
boolean relays [numOfRelays];
String relayLayout [numOfRelays] ={"br2","br1","-","sigFlood","sg2","sg1","sw1","sw7","swFlood","sw5","sw4","sw6","sw3","sw2","sw9","sw8"}; //0-8,9-15,16-23

//l289N
int tracks [2][6] ={{4,2,3,220,0,5},{5,7,6,220,0,5}}; //[Track] [0=trackPin1,1=trackPin2,2=speedPin,3=maxSpeed,4=currentSpeed,5=delay] 
int switchFloodGate;
int signalFloodGate;

//switchTracks
#define numberOfSwitchTracks 6
int switchTracks[numberOfSwitchTracks];

//Model Train settings
int initSpeed=30; //min speed lower then 30 Train emits bad noice
bool program1=true;
bool program2=false;

//FUNCTIONS --------------------------

//start train
void startUp(bool direction,int drive){
  if (direction==true){  
    digitalWrite(tracks[drive][0], HIGH);
    digitalWrite(tracks[drive][1], LOW);
  } else {
    digitalWrite(tracks[drive][0], LOW);
    digitalWrite(tracks[drive][1], HIGH);
  }
  analogWrite(tracks[drive][2],initSpeed);
  for (int i=initSpeed;i<tracks[drive][3];i++){
    analogWrite(tracks[drive][2],i);
    delay(tracks[drive][5]);
  }
}

//Stop Train
void stopIt(int drive){
    for (int i=tracks[drive][4];i>initSpeed;i--){
      analogWrite(tracks[drive][2],i);
      delay(tracks[drive][5]);
    }
    digitalWrite(tracks[drive][0], LOW);
    digitalWrite(tracks[drive][1], LOW);
}

//find relay
int findRelay(String search){
  if (text.indexOf("\n")>0){
    search=search.substring(0,text.length()-1); //get rid of \n
  }
  for (int index=0;index<sizeof(relays);index++){
    if (relayLayout[index]==search){
      return index;
    }
  }
}

// switch a SwitchPoint
void executeSwitch(){
  //floodgate open
  relays[switchFloodGate] = LOW;
  writeRegisters();
  delay(300);
  //floodgate closed
  relays[switchFloodGate] = HIGH;
  writeRegisters();
}

//execute signals
void executeSignal(){
  //floodgate open
  relays[signalFloodGate] = LOW;
  writeRegisters();
  delay(500);
  //floodgate closed
  relays[signalFloodGate] = HIGH;
  writeRegisters();
}

// write registers
void writeRegisters(){
	digitalWrite(latchPin, LOW);
  for (int i=0;i<numOfRelays;i++){
	  digitalWrite(clockPin, LOW); 
    int val = relays[i];
    digitalWrite(dataPin,val);
	  digitalWrite(clockPin, HIGH);
	}
	digitalWrite(latchPin, HIGH);
}

// FUNCTIONS -----------------------------------------

void setup() {
  Serial.begin(9600);
  //---------bluethoth
  HC05.begin(38400);
  
  //---------L298N
  pinMode(tracks[0][0], OUTPUT);
  pinMode(tracks[0][1], OUTPUT);
  pinMode(tracks[1][0], OUTPUT);
  pinMode(tracks[1][1], OUTPUT);
  
  //---------75HC595
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //initiallize registers
  switchFloodGate = findRelay("swFlood");
  signalFloodGate = findRelay("sigFlood");
	for(int i = numOfRelays-1; i >=  0; i--){
	  relays[i] = HIGH;
	}
  writeRegisters();
  executeSignal();
  executeSwitch();

  //switchTracks
  pinMode(A1,INPUT_PULLUP); //Sidetrack 1 -> right
  pinMode(A2,INPUT_PULLUP); //sidetrack 2 -> left
  pinMode(A3,INPUT_PULLUP); //back inner -> left
  pinMode(A4,INPUT_PULLUP); //back inner -> right
  pinMode(A5,INPUT_PULLUP); //back outer ->left
  pinMode(A6,INPUT_PULLUP); //back outer ->right
  for (int i=0;i<numberOfSwitchTracks;i++){
    switchTracks[i]=0;
  }
}

void loop() {
  //check trigger
  //back inner left
  if (analogRead(A3)<200){
    unsigned long currentSwitch = millis();
    if (currentSwitch - switchTracks[2] >2000){
      switchTracks[2]=currentSwitch;
    }
  }
  //back inner right
  if (analogRead(A4)<200){
    unsigned long currentSwitch = millis();
    if (currentSwitch - switchTracks[3] >2000){
      switchTracks[3]=currentSwitch;
    }    
  }
  //back outer left
  if (analogRead(A5)<200){
    unsigned long currentSwitch = millis();
    if (currentSwitch - switchTracks[4] >2000){
      switchTracks[4]=currentSwitch;
    }
  }
  //back outer right
  if (analogRead(A6)<200){
    unsigned long currentSwitch = millis();
    if ((currentSwitch - switchTracks[5] >1000) && (currentSwitch - switchTracks[1] >1000)){
      int signal=findRelay("sg2");
      if (program1==true && relays[signal]==HIGH){
        relays[findRelay("sw3")]=LOW;
        relays[signal]=LOW;
        executeSwitch();
        executeSignal();
        switchTracks[5]=currentSwitch;
      }
    }
  }
  // sidetrack 1 - STOP
  if (analogRead(A1)<200){
    unsigned long currentSwitch = millis();
    if (currentSwitch - switchTracks[0] >1000){
      int signal=findRelay("sg1");
      if (relays[signal]==LOW){
        relays[signal]=HIGH;
        relays[findRelay("sw1")]=HIGH;
        executeSwitch();
        executeSignal();
        switchTracks[0]=currentSwitch;
      }
    }
  }
  // sidetrack 2 - STOP
  if (analogRead(A2)<200){
    unsigned long currentSwitch = millis();
    if (currentSwitch - switchTracks[1] >1000){
      int signal=findRelay("sg2");
      if (relays[signal]==LOW){
        relays[signal]=HIGH;
        relays[findRelay("sw3")]=HIGH;
        executeSignal();
        executeSwitch();
        switchTracks[1]=currentSwitch;
      }
    }
  }
// remote controll
  while(HC05.available() > 0){
    chracter=HC05.read();
    text.concat(chracter);
    if (chracter=='\n'){
      //change maxSpeed
      //Serial.println(text.substring(0,text.length()-1));
      if (text.startsWith("sMa")){ //sMa[track]
        int track=(text.substring(3,4)).toInt()-1;
        tracks[track][3]=(text.substring(4,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
      }
      //change delay
      if (text.startsWith("sDe")){ //sDe[track]
        int track=(text.substring(3,4)).toInt()-1;
        tracks[track][5]=(text.substring(4,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
      }
      //forward
      if (text.startsWith("for")){  //for[track]
        int track=(text.substring(3,4)).toInt()-1;
        startUp(true,track);
      }
      //stop
      if (text.startsWith("sto")){ //sto[track]
        int track=(text.substring(3,4)).toInt()-1;
        stopIt(track);
      }
      //backward
      if (text.startsWith("bac")){ //bac[track]
        int track=(text.substring(3,4)).toInt()-1;
        startUp(false,track);
      }
      //manual drive
      if (text.startsWith("m")){ //m[track][-255-255]
        int track=text.substring(1,2).toInt()-1;
        if (text.indexOf("-")>0){
          tracks[track][4]=(text.substring(3,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
          digitalWrite(tracks[track][0], LOW);
          digitalWrite(tracks[track][1], HIGH);
          analogWrite(tracks[track][2],tracks[track][4]);
        } else {
          tracks[track][4]=(text.substring(2,text.length()-1)).toInt(); //need to convert the String to just a string of the value before coverting
          digitalWrite(tracks[track][0], HIGH);
          digitalWrite(tracks[track][1], LOW);
          analogWrite(tracks[track][2],tracks[track][4]);
        }
      }
      //switche Points
      if (text.startsWith("sw")){ //sw[number]
        int relaynumber=findRelay(text);
        relays[relaynumber]=!relays[relaynumber];
        executeSwitch();
      }
      if (text.startsWith("sg")){ // sq[number]
        int relaynumber=findRelay(text);
        relays[relaynumber]=!relays[relaynumber];
        executeSignal();
        if (text.indexOf("1")>0){
          switchTracks[0]=millis();
        }
        if (text.indexOf("2")>0){
          switchTracks[1]=millis();
        }
      }
      if (text.startsWith("br")){ // br[number]
        int relaynumber=findRelay(text);
        relays[relaynumber]=!relays[relaynumber];
        
        writeRegisters();
      }
      if (text=="relRe\n"){ //set all realys to initial
        for(int i = numOfRelays-1; i >=  0; i--){
          relays[i] = HIGH;
        }
        writeRegisters();
        executeSignal();
        executeSwitch();
      }
      if (text.startsWith("pr1")){ //programs
        program1=!program1;
      }
      if (text.startsWith("pr2")){ //programs
        program2=!program2;
      }
    text="";
    }
  }
}