#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TP-Link";
const char* password = "bappy4321";
float latitude , longitude; 
String latstr, lngstr;
TinyGPSPlus gps;
SoftwareSerial ss(4,5); 
WiFiServer server(80);//?
void setup () {
Serial.begin(115200);
ss.begin(9600);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print("Connecting..");
}
server.begin();
}
 
void loop() 
{
  while(ss.available()>0)
  {
  if(gps.encode(ss.read()))
  {
    if(gps.location.isValid())
    {
      latitude = gps.location.lat();
        latstr = String(latitude , 4);
        longitude = gps.location.lng();
        lngstr = String(longitude , 4);       
  //Serial.println(lat_str);
        //Serial.println(lng_str);
    }
  }
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
HTTPClient http;  //Declare an object of class HTTPClient
http.begin("http://ec2-18-136-102-102.ap-southeast-1.compute.amazonaws.com/api/v1/tracker/track/newbus/save?lat="+latstr+"&long="+lngstr); //Specify request destination
Serial.println("http://ec2-18-136-102-102.ap-southeast-1.compute.amazonaws.com/api/v1/tracker/track/newbus/save?lat="+latstr+"&long="+lngstr);
//http.begin("http://ec2-18-136-102-102.ap-southeast-1.compute.amazonaws.com/api/v1/tracker/track/newbus/save?lat=69.4111&long=96.1000");
int httpCode = http.GET();                                                                  //Send the request
if (httpCode > 0) { //Check the returning code
String payload = http.getString();   //Get the request response payload
Serial.println(payload);                     //Print the response payload
}
http.end();   //Close connection
}
delay(1000);    //Send a request every  seconds
}
}
