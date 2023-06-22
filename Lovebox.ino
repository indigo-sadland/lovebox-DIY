#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "SSD1306Wire.h"
//#include <WiFiClient.h>
#include <Servo.h>
#include "credentials.h"

SSD1306Wire oled(0x3C, D2, D1); // ports to which oled is connected
Servo myservo; 

int pos = 90;
int increment = -1;
int analogPin = 0;     // port to which light sensor is connected
int analogPinVal = 0;  

String messageMode;
String id;
String msg;
char idSaved; 
bool wasRead;
int lightValue;

const char* ssid = _ssid;
const char* password = _password;
const String url = _url;

void wifiConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    //WiFi.hostname("LoveBox");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connecting");
      Serial.println(WiFi.status());
      delay(500);
    }

    Serial.println("WiFi connected");
 
    // Print the IP address
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
  }
}

void getGistMessage() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    Serial.println("");
    Serial.println("Requesting " + url);

    if (https.begin(client, url)) {
      int httpCode = https.GET();
      //Serial.print("Response code: " + httpCode);

      if (httpCode > 0) {
        int i = 0;
        // Read HTTP response line by line.
        while(client.available()) {
          if(i == 0) {
            id = client.readStringUntil('\n');
            i++;
          } else if(i == 1) {
            messageMode = client.readStringUntil('\n');
            i++;
          } else if(i == 2){
            msg = client.readStringUntil('\n');
            break;
          } 
        }
        Serial.println(id);
        Serial.println(messageMode);
        Serial.println(msg);
        //Serial.println(https.getString());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
    
    if(id[0] != idSaved){ // new message
      wasRead = 0;
      idSaved = id[0];
      EEPROM.write(142, idSaved);
      EEPROM.write(144, wasRead);
      EEPROM.commit();

      drawMessage(msg);
    }
  }
}

void drawMessage(const String& message) {
  oled.clear();

  // differentiat between 't'ext and image message
  // TODO: CURRENTLY ONLY TEXT TYPE WORKS PROPERLY
  if(messageMode[0] == 't'){
    oled.drawStringMaxWidth(0, 0, 128, message);    
  } 
  else {
    for(int i = 0; i <= message.length(); i++){
      int x = i % 129;
      int y = i / 129;
    
      if(message[i] == '1'){
        oled.setPixel(x, y);
      }
    } 
  }    
  oled.display();
}

void spinServo(){
    myservo.write(pos);      
    delay(50);    // wait 50ms to turn servo

    if(pos == 75 || pos == 105){ // 75°-105° range
      increment *= -1;
    }
    pos += increment;
}

void setup()
{
  Serial.begin(9600);        // serial connect (for debug).

  myservo.attach(16);       // Servo on D0
  
  oled.init();
  oled.flipScreenVertically();
  oled.setColor(WHITE);
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
     
  oled.clear();
  oled.drawString(30, 30, "<3 LOVEBOX <3");
  oled.display();

  // Wi-Fi connect.
  wifiConnect();

  EEPROM.begin(512);
  idSaved = EEPROM.get(142, idSaved);
  wasRead = EEPROM.get(144, wasRead);

}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  
  if(wasRead){
    getGistMessage();   
  }
  
  while(!wasRead){   
    yield();
    spinServo();    // turn heart

    
    analogPinVal = analogRead(analogPin);     // read light value
    //Serial.println(analogPinVal);

    if(analogPinVal > 100) { 
      wasRead = true;
      EEPROM.write(144, wasRead);
      EEPROM.commit();
    }
  }
  
  delay(60000); // wait a minute before request gist again
}
