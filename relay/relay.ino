#include <SoftwareSerial.h>
int ports[16]={2,3,4,5,6,7,8,9,14,15,16,17,18,19,13,10};
SoftwareSerial extSerial(11, 12);
String rs;
void setup(){
for(int i = 0;i<16;i++){
    pinMode(ports[i], OUTPUT);
  digitalWrite(ports[i],HIGH);
}
Serial.begin(14400);
extSerial.begin(14400);
}
   
void loop(){
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

