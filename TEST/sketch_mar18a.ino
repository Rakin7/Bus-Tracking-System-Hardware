#include <b64.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4,5); // The serial connection to the GPS device

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

const char* ssid = "ABC";
const char* password = "12345678";
float latitude , longitude;
String lat_str , lng_str;
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
//   Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{
  while (ss.available() > 0) 
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
//        Serial.println(latitude);
//        Serial.println(longitude); 
          root["lat"] = latitude;
          root["long"] =longitude;

HTTPClient http;

http.begin("http://­ec2-18-136-102-102.ap­-southeast-1.compute­.amazonaws.com/api/­v1/bustracker/­%7Brobrob%7D");
http.addHeader("Content-Type","application/json");
http.POST(root);
http.end();
          
      }
    }
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS Interfacing with NodeMCU</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS Interfacing with NodeMCU</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td>  </tr> </table> ";   
  if (gps.location.isValid())
  {
    s += "<p align=center><a style=""color:RED;font-size:125%;"" href="" https://www.google.com/maps ";
    s += lat_str;
    s += "+";
    s += lng_str;
    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }
  s += "</body> </html> \n";
 client.print(s);
  delay(100);
}
