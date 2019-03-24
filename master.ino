#include <ESP8266WiFi.h>      //basic wifi
#include <DNSServer.h>        //local dns biar langsung portal
#include <ESP8266WebServer.h> //web server
#include <WiFiManager.h>      //lib wifi setup
#include <PubSubClient.h>     /MQTT
#include <SoftwareSerial.h>

const char* mqtt_server =  "103.122.64.35";
const char* mqttUser = "YourMQTTUsername";
const char* mqttPassword = "YourMQTTPassword";
const char* topicstate = "/h/2f/topan/ss1/s";
int relay = 16; //Pin 0

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  if ((char)payload[0] == '1') {
    digitalWrite(relay, LOW);
    client.publish(topicstate,"1");
    Serial.print("HIGH");
  } else {
    digitalWrite(relay, HIGH);
    client.publish(topicstate, "0");
Serial.print("LOW");
}
}


void mqttReconnect(){
  while (!client.connected()) {
    Serial.print("Mencoba konek ke MQTT...");


    if (client.connect("Topan SS1")) {
      Serial.println("Connected");
      client.publish("Init", "Topan SS1 Here");
      client.subscribe("yo");
    } else{
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 sec");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager; //begin wifiManager

  wifiManager.autoConnect("Lampu Aquarium Topan"); //nama SSID
  Serial.println("Tersambung ke WiFi Anda :)");

  //pin Relay
  pinMode(relay, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(relay, HIGH);
}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

}
