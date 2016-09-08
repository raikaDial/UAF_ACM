// Original wireless AP setup code from:
//   https://www.hackster.io/rayburne/esp8266-access-point-using-arduino-ide-19f632

#include <ESP8266WiFi.h>
#include <Wire.h>

#pragma GCC diagnostic ignored "-Wwrite-strings"

#define LED_PIN LED_BUILTIN

const char *ssid = "ACMBot-00";
const char *password = "uafrocks";
boolean    Lflag       = true;

WiFiServer server(80); // create object


void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is active low, so set high to turn off.
  setupWiFi();
  server.begin();
}


void loop() 
{
  WiFiClient client = server.available();  // Check if a client has connected
  if (!client)
  {
    if (Lflag)
    {
      Serial.println(F("Waiting for a client to connect to this server..."));
      Serial.println(F("On address 192.168.4.1"));
      delay(200);
      Lflag = false;
    }
      return;
  }

  String req = client.readStringUntil('\r');// Read the first line of the request
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1;                            // 'val' to keep track of both the
                                           // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/led/1") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/read") != -1)
    val = -2;                              // Will print pin reads
                                           // Otherwise request is invalidt
  if (val >= 0)                            // Set GPIO5 according to input
    digitalWrite(LED_PIN, val);

  client.flush();

  String ui = "<!DOCTYPE html>\
    <html>\
      <head>\
        <title>ACM Desktop Bot Control</title>\
      </head>\
      <body>\
        <form action='/led/0' method='get'>\
          <button type='submit' class='button'>LED On</button>\
        </form>\
        <form action='/led/1' method='get'>\
          <button type='submit' class='button'>LED Off</button>\
        </form>\      
      </body>\
      <style>\
        .button {\
          background-color: #4CAF50;\
          border: none;\
          color: white;\
          padding: 15px 32px;\
          text-align: center;\
          text-decoration: none;\
          display: inline-block;\
          font-size: 16px;\
        }\
      </style>\
    </html>";

  client.print(ui);                         // Send the response to the client
  delay(1);
  Serial.println(F("Client disonnected"));

  // The client will actually be disconnected 
  // when the function returns and 'client' object is destroyed
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
}


