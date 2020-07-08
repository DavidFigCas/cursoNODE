# 1 "C:\\Users\\Jos\\AppData\\Local\\Temp\\tmpgah2qx19"
#include <Arduino.h>
# 1 "C:/Users/Jos/Desktop/cursoNODE/src/mqtt_esp8266_curso.ino"
# 21 "C:/Users/Jos/Desktop/cursoNODE/src/mqtt_esp8266_curso.ino"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11



const char* ssid = "INFINITUM2522_5";
const char* password = "y75Ttb7Af7";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


DHT dht(DHTPIN, DHTTYPE);
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup();
void loop();
#line 46 "C:/Users/Jos/Desktop/cursoNODE/src/mqtt_esp8266_curso.ino"
void setup_wifi()
{

  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);


  } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }

}


void reconnect()
{

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.publish("outTopic", "hello world");

      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}


void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();



  float h = dht.readHumidity();

  float t = dht.readTemperature();

  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  float hic = dht.computeHeatIndex(t, h, false);
  snprintf (msg, MSG_BUFFER_SIZE, "%ld", hic);
  client.publish("/curso/profe/temp", msg);
  delay(2000);
}
