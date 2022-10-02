//Garima Roy
//2110994840
//3.3D
//3.3D - Sender

/*
  ArduinoMqttClient - WiFi Simple Sender

  This example connects to a MQTT broker and publishes a message to
  a topic once a second.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev2 board

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#endif

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.mqttdashboard.com";
int        port     = 1883;
const char topic[]  = "SIT210/wave";

const long interval = 1000;
unsigned long previousMillis = 0;

//defines variables 
int count = 0;
const int trig = 3;
const int echo = 4;
float timer;
float distance;
 

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  //Set the LED pin as output
  //pinMode(LED_BUILTIN, OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  Serial.print("Message sent: Garima Roy To started");  //print some text in Serial Monitor
  mqttClient.beginMessage(topic);
  mqttClient.print("Garima Roy Started");
  // mqttClient.print(count);
  mqttClient.endMessage();
  
}

void loop() 
{
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();
  pinMode(trig, OUTPUT);
  //Clears the trigPin condition
  digitalWrite(trig,LOW);
  delayMicroseconds(2);

  //Sets the trigPin HIGH(ACTIVE) for 10 microseconds
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  pinMode(echo, INPUT);
  
  timer = pulseIn(echo,HIGH); //Reads the ecopin, return the wave travel time in microseconds
  distance=timer*0.034 / 2;   //Calculating the distance

  if(distance < 20)
  {
    Serial.println("Wave: ");
    Serial.println();
    
    mqttClient.beginMessage(topic);
    mqttClient.print("Name: Garima Roy");
    mqttClient.print("Distance: ");
    mqttClient.print(distance);
    Serial.println(distance);
    Serial.println("cm");
    mqttClient.endMessage();
  }
    Serial.println();
}
