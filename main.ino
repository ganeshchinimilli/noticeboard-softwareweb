
#include <dummy.h>
//
#include <LEDMatrixDriver.hpp>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
//
//
///* 1. Define the WiFi credentials */
#define WIFI_SSID "realme 3"
#define WIFI_PASSWORD "123456789"

///* 2. Define the project ID */
#define FIREBASE_HOST "https://ledonoff-6cb7d.firebaseapp.com"
#define FIREBASE_PROJECT_ID "noticeboard-aac80"
#define API_KEY "AIzaSyDWJGkGrx9QGNGzXIngxkIrHgm1ixW3UCw"

///* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@gmail.com"
#define USER_PASSWORD "123456"

bool currentState;
//
////Define Firebase Data object
FirebaseData fbdo;
//
FirebaseAuth auth;
FirebaseConfig config;

//3.parloaaa

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
//#include <SPI.h>
#include "Font_Data.h"

#define DEBUG 0

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_ZONES 2
#define ZONE_SIZE 8
#define MAX_DEVICES (MAX_ZONES * ZONE_SIZE)
#define SCROLL_SPEED  30




#define ZONE_UPPER  1
#define ZONE_LOWER  0

#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    15

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
bool invertUpperZone = false;
char text[100] = "Welcome to my world";
char conect[100]="Connecting....";
int len=0;
unsigned long dataMillis = 0;
int count = 0;
//
void setup()
{

    Serial.begin(115200);

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
   Serial.println("hello");
      delay(1000);
  }

//    config.host = FIREBASE_HOST;
    config.api_key = API_KEY;

//    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
//
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    String content;
 FirebaseJson js;

// parolaa

invertUpperZone = (HARDWARE_TYPE == MD_MAX72XX::GENERIC_HW || HARDWARE_TYPE == MD_MAX72XX::PAROLA_HW);

  // initialise the LED display
  P.begin(MAX_ZONES);
  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setFont(ZONE_LOWER, BigFontLower);

  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  P.setFont(ZONE_UPPER, BigFontUpper);
  P.setCharSpacing(P.getCharSpacing() * 2); // double height --> double spacing
  if (invertUpperZone)
  {
    P.setZoneEffect(ZONE_UPPER, true, PA_FLIP_UD);
    P.setZoneEffect(ZONE_UPPER, true, PA_FLIP_LR);
  }
    
}

void loop()
{
        String documentPath = "products/firstone";
        String mask = "";

        Serial.println("------------------------------------");
        Serial.println("Get a document...");

        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
        {
            Serial.println("PASSED");
            Serial.println("------------------------------------");
            Serial.println(fbdo.payload().c_str());
            Serial.println("------------------------------------");
            Serial.println();
              FirebaseJson payload;
payload.setJsonData(fbdo.payload().c_str());

// Get the data from FirebaseJson object 
FirebaseJsonData jsonData;
payload.get(jsonData, "fields/data/stringValue", true);
//gun=jsonData.stringValue;
strcpy(text,jsonData.stringValue.c_str());

len=strlen(text);
//Serial.println(len);
Serial.println(jsonData.stringValue);
//displayText(text);
for(int i=0;i<10000;i++){
  
displayText(text);


}

     



        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
    

}

void displayText ( char * theText)
{
    P.displayAnimate();
  if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER))
  {

      P.setFont(ZONE_LOWER, BigFontLower);
      P.setFont(ZONE_UPPER, BigFontUpper);
      if (invertUpperZone)
      {
        P.displayZoneText(ZONE_LOWER,theText, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        P.displayZoneText(ZONE_UPPER, theText, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
      }
      else
      {
        P.displayZoneText(ZONE_LOWER, theText, PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        P.displayZoneText(ZONE_UPPER, theText, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      }
//      break;
//    cycle = (cycle + 1) % ARRAY_SIZE(msg);

    // synchronise the start
    P.displayClear();
    P.synchZoneStart();
  }
}
