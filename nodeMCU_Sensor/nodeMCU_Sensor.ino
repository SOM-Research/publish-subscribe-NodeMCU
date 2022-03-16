#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *WIFI_SSID = "network_name";
const char *WIFI_PASSWORD = "password";
 
const char *MQTT_HOST = "mqtt_host";
const int MQTT_PORT = 1883;       //port number
const char *MQTT_CLIENT_ID = "Temp Sensor NodeMCU";
const char *MQTT_USER = "user";   //broker user
const char *MQTT_PASSWORD = "pass"; //broker user password
const char *TOPIC_TEMPERATURE = "wwtp/temperature";
const char *TOPIC_TDS = "wwtp/TDS";

// GPIO where the DS18B20 is connected to
const int oneWireBus = 2;   
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

WiFiClient client;
PubSubClient mqttClient(client);
 
void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    int value = String((char*) payload).toInt();
 
    Serial.println(topic);
    Serial.println(value);
}

void setup()
{
    Serial.begin(9600);
    // Start the DS18B20 sensor
    sensors.begin();
  
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

    // Start the DS18B20 sensor
    sensors.begin();
}
 
void loop()
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    delay(3000);
    mqttClient.publish(TOPIC_TEMPERATURE, String(temperatureC).c_str());
}
