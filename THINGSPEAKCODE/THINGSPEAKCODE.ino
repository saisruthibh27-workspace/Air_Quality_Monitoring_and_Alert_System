#include <MQ135.h>

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

String apiKey = "KGKXH4LC3S3DMVCA"; // Enter your Write API key from ThingSpeak
const char *ssid = "raju";     // replace with your wifi ssid and wpa2 key
const char *pass = "sridevi9";
const char* server = "api.thingspeak.com";
 
WiFiClient client;
 
 
void setup()
{
  Serial.begin(115200);

  delay(10);
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    
    
    delay(4000);
}
 
 
  void loop()
  {
    MQ135 gasSensor = MQ135(A0);
    float AQI = gasSensor.getPPM();
    Serial.print("Air Quality: ");  
    Serial.print(AQI);
    Serial.println("  PPM");   
    Serial.println();
 

 
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(AQI);
    postStr += "r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Data Send to Thingspeak");
  }
    client.stop();
    Serial.println("Waiting...");
 
    delay(2000);      // thingspeak needs minimum 15 sec delay between updates.
}
