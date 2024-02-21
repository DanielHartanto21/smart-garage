#include <SPI.h>
#include <Ethernet.h>
#include<Servo.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 1, 177 }; 
byte gateway[] = { 192, 168, 1, 1 }; 
byte subnet[] = { 255, 255, 255, 0 }; 
EthernetServer server(80); 

String readString;
int trigpin=5;
int echopin=4;
int lampudalam=7;
int ldr=A0;
int motoservo=8;
int cahaya=0;
long distance, duration;
long waktu, jarak;
int buka=30;
int buka2=30;
int tutup=90;
int sensor  = 9;
int val = 0;
int trigpin2=3;
int echopin2=2;
  
Servo myservo;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(8);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(trigpin2,OUTPUT);
  pinMode(echopin2,INPUT);
  pinMode(lampudalam,OUTPUT);
  pinMode(sensor,INPUT);
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = server.available();
    int bacaldr= analogRead(ldr);
    digitalWrite(trigpin,LOW);
    delayMicroseconds(5);
    digitalWrite(trigpin,HIGH);
    delayMicroseconds(10);
    duration=pulseIn(echopin,HIGH);
    distance=duration*0.034/2*3;
    cahaya=map(bacaldr,0,1023,0,100);
    digitalWrite(trigpin2,LOW);
    delayMicroseconds(5);
    digitalWrite(trigpin2,HIGH);
    delayMicroseconds(10);
    waktu=pulseIn(echopin2,HIGH);
    jarak=waktu*0.034/2;
    Serial.println(distance);
    if(cahaya<20){
      digitalWrite(lampudalam,HIGH);
    }
    else{
      digitalWrite(lampudalam,LOW);
    }
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (readString.length() < 100) {
                    readString += c;
                }
                if (c == 0x0D) {
                    client.println("HTTP/1.1 200 OK"); 
                    client.println("Content-Type: text/html");
                    client.println();
                    client.println("<HTML>");
                    client.println("<HEAD>");
                    client.println("<TITLE>web buka pintu</TITLE>");
                    client.println("</HEAD>");
                    client.println("<BODY>");
                    client.println("<div align='center'><br>");
                    client.println("<H1>Kontrol garasi</H1>");
                    client.println("<br>");         
                    client.println("<H2>pastikan sudah dekat dan nyalakan lampu</H2>");
                    client.println("<br>");
                    client.println("<H2><a href=\"/?buka\"\">buka dari luar</a><br></H2>");
                    client.println("<H2><a href=\"/?buka2\"\">buka dari dalam</a><br></H2>");
                    client.println("<H2><a href=\"/?tutup\"\">tutup</a><br></H2>");
                    client.println("</div></BODY>");
                    client.println("</HTML>");
                    delay(10);
                    client.stop();
                    if(readString.indexOf("?buka2") > -1)
                            {
                                if(jarak<13){
                                    myservo.write(buka2);
                                }
                            }
                    if(readString.indexOf("?buka") > -1) 
                    {
                        if(distance<15){
                          if(cahaya>20){
                            myservo.write(buka);
                         }
                        }
                    }
                    else{
                      if(readString.indexOf("?tutup") > -1) 
                        {                                                 
                              myservo.write(tutup);                           
                        }
                        else{                          
                            
                        }
                    }
                    readString="";
                }
            }
        }
   
    
    }
    
}
