#include <ESP8266WiFi.h>
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define WLAN_SSID   "ssid"
#define WLAN_PASS   "doen't matter"

//MQTT broker settings
#define HOST        "whatever(ip)"
#define PORT        1883
//#define USERNAME    ""
//#define PASSWORD    ""
  
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, HOST, PORT);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, "weatherStation/temperature");
Adafruit_MQTT_Publish pressure = Adafruit_MQTT_Publish(&mqtt, "weatherStation/pressure");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, "weatherStation/humidity");
Adafruit_MQTT_Publish light = Adafruit_MQTT_Publish(&mqtt, "weatherStation/light");
Adafruit_MQTT_Publish voltage = Adafruit_MQTT_Publish(&mqtt, "weatherStation/voltage");
void MQTT_connect();

Adafruit_BMP280 bmp; // I2C

void setup() {
  delay(100);
  pinMode(D6, OUTPUT);
  digitalWrite(D6,HIGH);
  WiFi.mode(WIFI_STA);  

  //Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
    delay(5000);
  //Serial.println("IP address: "); Serial.println(WiFi.localIP());
  dht.begin();
  bmp.begin();
  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    //while (1);
  }
  delay(5000);
  MQTT_connect();
  delay(100);
  temperature.publish(bmp.readTemperature());
  pressure.publish(bmp.readPressure());
  humidity.publish(dht.readHumidity());
//  voltage.publish(ESP.getVcc());
  light.publish((analogRead(A0))/10);
  digitalWrite(D6,LOW);
  Serial.println("deep sleep");
  ESP.deepSleep(3e+8);
}

void loop() 
{
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       //Serial.println(mqtt.connectErrorString(ret));
       //Serial.println("Retrying MQTT connection in 1 second...");
       mqtt.disconnect();
       delay(1000);
           
  }
  //Serial.println("MQTT Connected!");
}
