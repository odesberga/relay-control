#include <Arduino.h>

#include <SoftwareSerial.h>
int ports[16]={2,3,4,5,6,7,8,9,14,15,16,17,18,19,13,10};
SoftwareSerial extSerial(11, 12);
String rs;
char serbuf[250];
//char rulebuf[62];
struct msg
{
  char inbus[3];
  char inport[3];
  char msgtype[3];
  char tobus[3];
  char tomodule[3];
  char modulecount[3];
  char port[3];
  char filename[11];
  char data[200];
  char longpress[3];
};

Msg emptymsg;
Msg imessage;

void setup(){
for(int i = 0;i<16;i++){
    pinMode(ports[i], OUTPUT);
  digitalWrite(ports[i],HIGH);
}
Serial.begin(14400);
extSerial.begin(14400);
}

void loop(){

    memset(serbuf,0, sizeof(serbuf));
    imessage=emptymsg;
    char inchar;
    byte index=0;
    String inst;
    while (Serial.available() > 0)
    {
        inchar = Serial.read();
      if (inchar == '!') {
        inst= Serial.readStringUntil('#');
        inst.toCharArray(serbuf, 250);
        strcat(serbuf, '\0');
        populateiMsg(0);
        if (isthisforme()){
          processMsg();
      } else {


      }


        memset(serbuf,0, sizeof(serbuf));
        imessage=emptymsg;
        omessage=emptymsg;
      }
  }




  while (Serial.available()) {

  int module = Serial.parseInt();
  int modcnt = Serial.parseInt();
  modcnt++;
  int port = Serial.parseInt();
  int pstate = Serial.parseInt();
  if (Serial.read() == '\n') {

    if (module == modcnt) {
    PortSwitch(port,pstate);
    }
    else
    {
     extSerial.print(module) ;
     extSerial.print(',');
     extSerial.print(modcnt);
     extSerial.print(',');
     extSerial.print(port);
     extSerial.print(',');
     extSerial.print(pstate);
     extSerial.print('\n');
    // delay(5);
    }
  }
  }
}


bool isthisforme(){
int tm = atoi(imessage.tomodule);
int mc = atoi(imessage.modulecount);
if ( tm == mc ){
  return true;
} else {
  return false;
};


void PortSwitch(int port,int state){
  if (port != 33){
    if (state == 2){
        digitalWrite(ports[port-1],!digitalRead(ports[port-1]));
    }
    if (state == 1){
        digitalWrite(ports[port-1],LOW);
    }
    if (state == 0){
        digitalWrite(ports[port-1],HIGH);
    }

  }
  if (port == 33){
    for (int i=0;i<16;i++){
           if (state == 1){
              digitalWrite(ports[i],LOW);
            }
          if (state == 0){
              digitalWrite(ports[i],HIGH);
          }
    }

  }
}
