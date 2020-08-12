#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <TinyGPS++.h>


const char* ssid = "ABC";
const char* password = "12345678";

// values for GPS
float latitude=0.0;
float longitude=0.0; 
String latstr= "0.0";
String lngstr="0.0";

String Link="http://ec2-18-136-102-102.ap-southeast-1.compute.amazonaws.com/api/v1/tracker/track/robrob/save?";
String URL;

TinyGPSPlus gps;
SoftwareSerial ss(4,5);
WiFiServer server(80);
HTTPClient http;


void setup() {
  Serial.begin(115200);
  start_wifi();
  ss.begin(9600);
  
}

void loop() {
  
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        
        latitude = gps.location.lat();
        latstr = String(latitude , 2);
        longitude = gps.location.lng();
        lngstr = String(longitude , 2);
//        Serial.print(latitude);
//        Serial.print("   ");
//        Serial.print(longitude);
//        Serial.print("   ");
//        Serial.print(latstr);
//        Serial.print("   ");
//        Serial.print(lngstr);
//        Serial.print("   ");
      }
    }
  }
  if (WiFi.status() == WL_CONNECTED) { 
      URL = Link + "lat=" + latstr + "&long=" + lngstr;
      //URL = Link + "lat=" + 23.78 + "&long=" + 90.41;
      
      http.begin(URL); 
      //Serial.println(URL);
       int httpCode = http.GET();                                                                  
      if (httpCode > 0) { //Check the returning code
       String payload = http.getString();   //Get the request response payload
        Serial.println(payload);                     //Print the response payload
      }
      http.end();   //Close connection
    }
    delay(1000);
}

// wifi Connector code...
void start_wifi() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}
