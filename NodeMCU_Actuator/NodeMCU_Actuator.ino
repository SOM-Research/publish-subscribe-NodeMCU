#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *WIFI_SSID = "network_name";
const char *WIFI_PASSWORD = "password";
 
const char *MQTT_HOST = "mqtt_host";
const int MQTT_PORT = 1883;       //port number
const char *MQTT_CLIENT_ID = "ESP8266 NodeMCU";
const char *MQTT_USER = "user";   //broker user
const char *MQTT_PASSWORD = "pass"; //broker user password
const char *TOPIC_LED = "wwtp/led";
const char *TOPIC_BUZZER = "wwtp/buzzer";
const int LED_D2 = 4;
const int BUZZER_D8 = 15;

WiFiClient client;
PubSubClient mqttClient(client);
 
void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    int value = String((char*) payload).toInt();
 
    Serial.println(topic);
    Serial.println(value);

    if (strcmp(topic, "wwtp/led") == 0 && value == 1){
      digitalWrite(LED_D2, HIGH);
    }
    else if (strcmp(topic, "wwtp/led") == 0 && value == 0){
      digitalWrite(LED_D2, LOW);
    }

    if (strcmp(topic, "wwtp/buzzer") == 0 && value == 1){
      digitalWrite(BUZZER_D8, HIGH);
    }
    else if (strcmp(topic, "wwtp/buzzer") == 0 && value == 0){
      digitalWrite(BUZZER_D8, LOW);
    }
}

void setup()
{
    pinMode(LED_D2, OUTPUT);
    pinMode(BUZZER_D8, OUTPUT);
    Serial.begin(9600);
 
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("Connected to Wi-Fi");
 
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(callback);
 
    while (!client.connected()) {
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
            Serial.println("Connected to MQTT broker");
        } else {
            delay(500);
            Serial.print(".");
        }
    }

    mqttClient.subscribe(TOPIC_LED);
    mqttClient.subscribe(TOPIC_BUZZER);
}
 
void loop()
{
    mqttClient.loop();
}
