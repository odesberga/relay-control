#include <Arduino.h>
#include <compilemessage.h>
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
typedef struct msg Msg;
Msg emptymsg;
Msg imessage;
Msg omessage;
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
            if (isthisforme()){
                populateiMsg(0);
                processMsg();
          } else {
                sendIgnoredMessage(0);
          }
      }
  }
memset(serbuf,0, sizeof(serbuf));
    while (extSerial.available() > 0)
    {
        inchar = Serial.read();
      if (inchar == '!') {
            inst= extSerial.readStringUntil('#');
            inst.toCharArray(serbuf, 250);
            strcat(serbuf, '\0');
        if (isthisforme()){
                populateiMsg(1);
                processMsg();
            } else {
                sendIgnoredMessage(1);
        }
      }
  }
}

void sendIgnoredMessage(int inpbus){
    char modcnt[3];
    modcnt[0]=serbuf[2];
    modcnt[1]=serbuf[3];
    modcnt[2]='\0';
    int pt = atoi(modcnt);

    if (inpbus == 0) {
            pt++;
    }
    if (inpbus == 1) {
            pt--;
    }
    String astring = String(pt);
    if (astring.length()==1)  {
        serbuf[2]="0";
        serbuf[3]=astring[0];

    } else {
      serbuf[2]=astring[0];
      serbuf[3]=astring[1];
    }
    if (inpbus == 0) {
            extSerial.print("!"+String(serbuf)+"#");
    }
    if (inpbus == 1) {
            Serial.print("!"+String(serbuf)+"#");
    }
}

bool isthisforme(){
    char md[3];
    char modcnt[3];
md[0]=serbuf[0];
md[1]=serbuf[1];
md[2]='\0';
modcnt[0]=serbuf[2];
modcnt[1]=serbuf[3];
modcnt[2]='\0';
int tm = atoi(md);
int mc = atoi(modcnt);
if ( tm == mc ){
  return true;
} else {
  return false;
};
}



void populateiMsg(int inpBus) {
  imessage=emptymsg;
  if (inpBus == 0){
  imessage.inbus[0]='0';
  imessage.inbus[1]='0';
  imessage.inbus[2]='\0';
}
if (inpBus == 1){
imessage.inbus[0]='0';
imessage.inbus[1]='1';
imessage.inbus[2]='\0';
}
  imessage.msgtype[0] = serbuf[5];
  imessage.msgtype[1] = serbuf[6];
  imessage.msgtype[3] = '\0';
  int commacount=0;
  int idx=0;

  for (int i =8; i< sizeof(serbuf)-1;i++){
      if (serbuf[i]=='\0'){break;};
      if (serbuf[i]==','){
        commacount++;
        idx=0;
      } else {
        genMsg(i,idx,commacount);
        idx++;
      }
  }
}

void genMsg(int index,int index2,int commacount){
  int pt = atoi(imessage.msgtype);
  switch (pt) {
    case 21:
            switch (commacount) {
              case 0:
                imessage.port[index2]= serbuf[index];
                break;
              case 1:
                imessage.data[index2]= serbuf[index];
                break;
              }
    break;
    }
}

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
void processMsg(){
  int pt = atoi(imessage.msgtype);
  switch (pt) {
    case 21:
      PortSwitch(atoi(imessage.port),atoi(imessage.data));
      break;

  }
  memset(serbuf,0, sizeof(serbuf));
  imessage=emptymsg;
  omessage=emptymsg;
}
void sendData(){

  int pt = atoi(omessage.tobus);
  switch (pt) {
    case 0:
      Serial.print("!");
      Serial.print(omessage.tomodule);
    //  Serial.print(",");
      Serial.print(omessage.modulecount);
      Serial.print(",");
      Serial.print(omessage.msgtype);
      Serial.print(",");
      Serial.print(omessage.data);
      Serial.print("#");
      break;
    case 1:
      extSerial.print("!");
      extSerial.print(omessage.tomodule);
      extSerial.print("01");
      extSerial.print(omessage.data);
      extSerial.print("#");
      break;

  }

  omessage=emptymsg;
}
void genOmessageAndSendData(String data,String obus,String omod){
omessage=emptymsg;
data.toCharArray(omessage.data, data.length()+1);
  if (obus.length() == 1) {
    omessage.tobus[0] = "0";
    omessage.tobus[1] = obus[0];
  } else {
    omessage.tobus[0] = obus[0];
    omessage.tobus[1] = obus[1];
  }
  if (omod.length() == 1) {
    omessage.tomodule[0] = "0";
    omessage.tomodule[1] = omod[0];
  } else {
    omessage.tomodule[0] = omod[0];
    omessage.tomodule[1] = omod[1];
};
sendData();
omessage=emptymsg;
}
