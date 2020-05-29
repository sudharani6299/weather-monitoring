#include<DHT.h>
#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
char data[100];

DHT  dht(D7,DHT11);
#define CLIENT_ID "temp-hum"
#define port 1883
const char *ssid="vinod chowdary";
const char *pswd="9490119492";
const char *mqtt_server="34.239.33.237";
WiFiClient espclient;
PubSubClient client(espclient);
String temperature,humidity,ppmvalue;
void setup()
{
  dht.begin();
  Serial.begin(9600);
  WiFi.begin(ssid,pswd);
  while(WiFi.status()!=WL_CONNECTED)
  {
   Serial.print("....");
   delay(200);
  }
Serial.println("\n WiFi CONNECTED");
Serial.println(ssid);
Serial.println(WiFi.localIP());
client.setServer(mqtt_server,1883);
}
void reconnect() {
  while(!client.connected())
  {
    Serial.print("Attempting mqtt connect");
  if(client.connect(CLIENT_ID))
  {
    Serial.println("connected");
    
  }
  }
  // put your setup code here, to run once:
}

void loop() {
  
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  if(!client.connected())
  {
    reconnect();
  }
   float temp,hum;
   int ppm;
   temp=dht.readTemperature();
   hum=dht.readHumidity();
   ppm=100;
   StaticJsonDocument<300> Doc;
   Doc["temp"]=String(temp);
   Doc["hum"]=String(hum);
   Doc["ppm"]=String(ppm);
   serializeJson(Doc, data);
   if(client.connect(CLIENT_ID))
   {
      client.publish("temp-hum",data);
   }
   delay(10000);
   
  
}


    
    
    
    
  
  

      
  
  
