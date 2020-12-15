#include <SoftwareSerial.h>
#include <Wire.h> 
#include <SPI.h>  
#include <SFE_MicroOLED.h>  
#include <avr/pgmspace.h>
#define PIN_RESET 9  
#define PIN_DC    8 
#define PIN_CS    10 
#define DC_JUMPER 0
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS);
#define ESP8266_BAUD  9600 
#define SS_RX 5
#define SS_TX 6
SoftwareSerial ESP8266_PORT(SS_RX, SS_TX);
#include "TimerOne.h" 
#define ONBOARD_LED   12 
#define button 2
#define led 13
#define bazzer 7
const String ESP8266_APNAME  = "Eldad";
const String ESP8266_APPSWD  = "12345678";
String ESP8266_CMD;
int CheckButton;
int hour=0;
int minute=0;
int second=0;
int b=0;

void setup()
{ 
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
  pinMode(bazzer,OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);
digitalWrite(bazzer,HIGH);
  ISR_Init(10000000); 
  Serial.begin(9600);    
  ESP8266_PORT.begin(ESP8266_BAUD); 
  delay(1000);
  ESP8266_PORT.println("AT+CIOBAUD=9600");
   ESP8266_PORT.end();
   delay(1000);
  ESP8266_PORT.begin(9600);
  delay(1000);
  ESP8266_Init();
  oled.begin();
  oled.clear(ALL);
  oled.display();   
}


void ESP8266_Init()
{
  String WWWResponse;
  Serial.print(F("Connecting to WiFi ("));
  ESP8266_Send("AT+CWMODE=1", 100, false);
  WWWResponse=ESP8266_Send("AT+CWJAP=\"" + ESP8266_APNAME + "\",\"" + ESP8266_APPSWD + "\"", 5000, false);
  if(find_text("CONNECTED", WWWResponse) == 0)
  {
    Serial.println(F("Error: can't connect to AP."));
    Timer1.stop();
    digitalWrite(ONBOARD_LED, 0);
    while(1);
  }
  else
  {
    Serial.println(F("Ready!"));     
    while(Serial.available())
      Serial.read();
  }
}

void loop() {
  if (ESP8266_PORT.available()) {
    int inByte = ESP8266_PORT.read();
    Serial.write(inByte);
  }
      ESP8266_Connect();
}
void ESP8266_Connect()
{
  String ESP8266_CMD, WWWResponse;
if(b==0){
  String hour1;
 String minute1; 
 String second1;
          ESP8266_Send("AT+CIPSTART=\"TCP\",\"ortyuvali.arduino2web.com\",80", 1000, false);
          ESP8266_CMD="GET /Eldad/watch/time.txt HTTP/1.1\r\nHost: ortyuvali.arduino2web.com\r\n\r\n";
          ESP8266_Send("AT+CIPSEND="+String(ESP8266_CMD.length()), 1000, false);
          WWWResponse=ESP8266_Send(ESP8266_CMD, 1000, true); 
            int idPos;
  for (int i = 0; i <= WWWResponse.length() - 4; i++) {
    if (WWWResponse.substring(i,4+i) == "hour") {
      idPos = i;
    }
  }
            if(WWWResponse.substring(idPos, idPos+4) == "hour")
           {
           hour1 = WWWResponse.substring(idPos+5,idPos+7);
           minute1 = WWWResponse.substring(idPos+8,idPos+10);
           second1 = WWWResponse.substring(idPos+11,idPos+13);
           b=5;
           ISR_Init(1000000);
           while(Serial.available())
              Serial.read();
           }
          hour = hour1.toInt();
          minute = minute1.toInt();
           second = second1.toInt();   
}
if (b==5){
  ESP8266_Send("AT+CIPSTART=\"TCP\",\"ortyuvali.arduino2web.com\",80", 1000, false);
  ESP8266_CMD="GET /Eldad/value.txt HTTP/1.1\r\nHost: ortyuvali.arduino2web.com\r\n\r\n";
  ESP8266_Send("AT+CIPSEND="+String(ESP8266_CMD.length()), 1000, false);
  WWWResponse=ESP8266_Send(ESP8266_CMD, 1000, true);
  int idPos=0;
    for (int i = 0; i <= WWWResponse.length() - 4; i++) {
    if (WWWResponse.substring(i,4+i) == "baby") {
      idPos = i;
    }
    else if (WWWResponse.substring(i,4+i) == "door") {
      idPos = i;
    }
  }
  oled.begin();
  oled.clear(ALL);
  oled.setFontType(0);
  
  if(WWWResponse.substring(idPos, idPos+4) == "baby")
  {
   
    digitalWrite(led,HIGH);
     digitalWrite(bazzer,LOW);
    CheckButton = digitalRead(button);
    while(CheckButton<1){
    CheckButton = digitalRead(button);

 
     oled.clear(PAGE);
     oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
     oled.print("Baby CRY");
     oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
     oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
     oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
     oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5);
     oled.display();
    }
    sendata();
    while(Serial.available())
      Serial.read();
  }
    else if(WWWResponse.substring(idPos, idPos+4) == "door")
  {
 
    digitalWrite(led,HIGH);
     digitalWrite(bazzer,LOW);
    CheckButton = digitalRead(button);
    while(CheckButton<1){
    CheckButton = digitalRead(button);
    oled.clear(PAGE);
    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
    oled.print("  Door");
    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10);
    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5);
    oled.display();
    }
    sendata();
    while(Serial.available())
      Serial.read();
  }
  else
    digitalWrite(led,LOW);
    digitalWrite(bazzer,HIGH);
}
}
void sendata(){
    Serial.println(F("Sending off"));
    ESP8266_PORT.println(F("AT+CIPSTART=\"TCP\",\"ortyuvali.arduino2web.com\",80"));
    delay(250);
    ESP8266_PORT.println(F("AT+CIPSEND=83"));
    delay(100);
    ESP8266_PORT.println(F("GET /Eldad/fupdate.php?value=off HTTP/1.1\r\nHost: ortyuvali.arduino2web.com\r\n\r\n"));
    delay(500);
}

String ESP8266_Send(String command, const int timeout, boolean debug)
{
    String response = "";
    ESP8266_PORT.println(command); 
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(ESP8266_PORT.available())
      {
        char c = ESP8266_PORT.read();
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

int find_text(String needle, String haystack) {
  int foundpos = 0;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

void ISR_Init(unsigned long ISR_INTERVAL)
{
  Timer1.initialize(ISR_INTERVAL);
  Timer1.attachInterrupt(ISR_callback);
}

void ISR_callback()
{
  if (b==5){
  oled.begin();
  oled.clear(ALL);
  oled.setFontType(0);
  if (hour<12 && hour!=11){
                     if (minute<60 && minute!=59){
                                   if (second<60 && second!=59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5);  
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                    second++;
                                     }
                                    else if(second == 59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                     second=00;
                                     minute++;                                    
                                    }
                       }
                     else if(minute == 59){
                                          if (second<60 && second!=59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10);  
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                          second++;
                                        }
                                        else if(second == 59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                          second=00;
                                          minute=00;
                                          hour++;                                   
                                        }
                   }
  }
  else if (hour==11){
                    if (minute<60 && minute!=59){
                                   if (second<60 && second!=59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5);  
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5);  
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                    second++;
                                     }
                                    else if(second == 59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5);
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5); 
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5); 
                                    oled.display();
                                     second=00;
                                     minute++;                                   
                                    }
                       }
                     else if(minute == 59){
                                          if (second<60 && second!=59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5);
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5);
                                    oled.display();
                                          second++;
                                        }
                                        else if(second == 59){
                                    oled.clear(PAGE);
                                    oled.setCursor(LCDWIDTH/5, LCDHEIGHT/2);
                                    oled.print(hour); oled.print(":");
                                    oled.print(minute); oled.print(":");
                                    oled.print(second);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)+10);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)-5, LCDWIDTH-3, (LCDHEIGHT/2)-5);
                                    oled.line(LCDWIDTH-3, (LCDHEIGHT/2)+10, LCDWIDTH-3, (LCDHEIGHT/2)-5);
                                    oled.line((LCDWIDTH/5)-5, (LCDHEIGHT/2)+10, (LCDWIDTH/5)-5, (LCDHEIGHT/2)-5);
                                    oled.display();
                                          second=00;
                                          minute=00;
                                          hour=00;                                       
                                        }
  }
}
}
}
