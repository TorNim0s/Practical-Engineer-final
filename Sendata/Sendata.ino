#include <SoftwareSerial.h> 

#define ESP8266_BAUD  9600
#define SS_RX 5
#define SS_TX 6

SoftwareSerial ESP8266_PORT(SS_RX, SS_TX);
const String ESP8266_APNAME  = "Eldad";
const String ESP8266_APPSWD  = "12345678";

String ESP8266_CMD;
int CheckButton = 7;
int voicepin = A4;
int analogval = 0;

void setup()
{
  pinMode(voicepin, INPUT);
  pinMode(CheckButton, INPUT);
  Serial.begin(9600); 
  ESP8266_PORT.begin(ESP8266_BAUD);
  delay(1000);
  ESP8266_Init();
}

void ESP8266_Init()
{
  String WWWResponse;
  Serial.println("Connecting to WiFi (" + ESP8266_APNAME + ", " + ESP8266_APPSWD + "), please wait...");
  ESP8266_Send("AT+CWMODE=1", 100, false);
  WWWResponse = ESP8266_Send("AT+CWJAP=\"" + ESP8266_APNAME + "\",\"" + ESP8266_APPSWD + "\"", 5000, false);
  if (find_text("CONNECTED", WWWResponse) == 0)
  {
    Serial.println("Error: can't connect to AP."); 
    while (1);
  }
  else
  {
    Serial.println("Ready!"); 
    while (Serial.available())
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
  analogval = analogRead(voicepin);
  Serial.println(analogval); 
  
  if (analogval > 68) { 
  sendata("baby"); 
  }
  
  else if(digitalRead(CheckButton) == HIGH){
    sendata("door"); 
  }
}

void sendata(String problam){ 
  
    String ESP8266_CMD, WWWResponse; 
    Serial.println(String(problam)); 
    ESP8266_Send("AT+CIPSTART=\"TCP\",\"ortyuvali.arduino2web.com\",80", 1000, false); 
    ESP8266_CMD="GET /Eldad/fupdate.php?value=" + String(problam) + " HTTP/1.1\r\nHost: ortyuvali.arduino2web.com\r\n\r\n";
    ESP8266_Send("AT+CIPSEND="+String(ESP8266_CMD.length()), 1000, false);
    ESP8266_Send(ESP8266_CMD, 1000, false);
}

String ESP8266_Send(String command, const int timeout, boolean debug)
{
  String response = "";
  ESP8266_PORT.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (ESP8266_PORT.available())
    {
      char c = ESP8266_PORT.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

int find_text(String needle, String haystack) {
  int foundpos = 0;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i, needle.length() + i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
