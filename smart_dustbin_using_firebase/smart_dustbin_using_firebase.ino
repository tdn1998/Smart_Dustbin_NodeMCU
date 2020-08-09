#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "smart-dustbin-9fb23.firebaseio.com"
#define FIREBASE_AUTH "wcPnSyMq5dJeOqgYkrlcXrTC7aQbe9vRZbVeK1xA"
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "turbodrive1"

int trig = D3;
int echo = D2;

int dur;
float dis;
int fillpercent;

void setup(){
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT); 
}

void loop(){
  digitalWrite(trig,LOW);
  delay(500);
  
  digitalWrite(trig,HIGH);
  delay(1000);
  
  digitalWrite(trig,LOW);
  
  dur=pulseIn(echo,HIGH);
  dis=(dur/2)*0.034;
  delay(500);

  Serial.println("Distance:");
  Serial.println(dis);
  
  if(dis<0)
  {fillpercent=100;}
  else if(dis>22)
  {fillpercent=0;}
  else
  {fillpercent=map(dis,0,21.5,100,0);}
  Serial.println("Percent Fill:");
  Serial.println(fillpercent);

  String fill = String(fillpercent);

  Firebase.setString("Percent Filled", fill);

  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
}
