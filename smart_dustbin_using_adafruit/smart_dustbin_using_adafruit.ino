#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "AndroidAP"
#define WLAN_PASS       "turbodrive1"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "tdn123"
#define AIO_KEY         "40378cd2dae1484bb9c075a6e555abe4"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish status = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/status");

int trig = D1;
int echo = D0;

int dur;
float dis;
float fillpercent;

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT); 

  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
}

void loop(){
  MQTT_connect();
  
  digitalWrite(trig,LOW);
  delay(500);
  
  digitalWrite(trig,HIGH);
  delay(1000);
  
  digitalWrite(trig,LOW);
  
  dur=pulseIn(echo,HIGH);
  dis=(dur/2)*0.034;
  delay(500);

  Serial.print("Distance: ");
  Serial.println(dis);
  
  if(dis<0)
  {fillpercent=100;}
  else if(dis>22)
  {fillpercent=0;}
  else
  {fillpercent=map(dis,0,21.5,100,0);}

  Serial.print(F("\nSending status of DustBin "));
  Serial.print(fillpercent);
  Serial.print("...");
  if (! status.publish(fillpercent)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  indicator();
  delay(500);
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void indicator(){
  if(fillpercent==0&&fillpercent<15){
    digitalWrite(D3,LOW);
    digitalWrite(D4,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    }
  else if(fillpercent==15&&fillpercent<=40){
    digitalWrite(D3,HIGH);
    digitalWrite(D4,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    }
  else if(fillpercent>40&&fillpercent<=60){
    digitalWrite(D3,HIGH);
    digitalWrite(D4,HIGH);
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    }
    else if(fillpercent>60&&fillpercent<=80){
    digitalWrite(D3,HIGH);
    digitalWrite(D4,HIGH);
    digitalWrite(D5,HIGH);
    digitalWrite(D6,LOW);
    }
    else if(fillpercent>80&&fillpercent<=100){
    digitalWrite(D3,HIGH);
    digitalWrite(D4,HIGH);
    digitalWrite(D5,HIGH);
    digitalWrite(D6,HIGH);
    }
  }
