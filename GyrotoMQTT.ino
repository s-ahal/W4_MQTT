#include <Wire.h>
#include <Arduino_LSM6DS3.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

WiFiClient wifi;
MqttClient mqtt(wifi);

// WiFi network info: ssid and password
const char wifi_ssid[] = "sandbox370";
const char wifi_pass[] = "+s0a+s03!2gether?";

// MQTT broker info: url and port (1883 default for MQTT)
const char broker[] = "sahalnet.cloud.shiftr.io";
const int port = 1883;

// If needed: broker authentication credentials
const char mqtt_user[] = "sahalnet";
const char mqtt_pass[] = "snailssnailssnails";

// The topic this device will publish messages to
const char pubTopic[] = "snail_facts";

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);
  }
  Serial.println("\nWiFi connected!");

  // Give your device any name, to use for identification
  mqtt.setId("snail_teacher");

  // Set mqtt credentials, if needed
  mqtt.setUsernamePassword(mqtt_user, mqtt_pass);

  if (!mqtt.connect(broker, port)) {
    // Error codes
    // -1: credentials rejected
    // -2: can't connect to broker
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt.connectError());
    while (true)
      ;  // Do nothing forever
  } else
    Serial.println("MQTT connected.");

  // Initialize the LSM6DS3 sensor
  if (!IMU.begin()) {
    Serial.println("Failed to initialize LSM6DS3!");
    while (1)
      ;
  }
}

const int sendInterval = 1000;

void loop() {
  // Read gyroscope data
  float gyroX, gyroY, gyroZ;
  IMU.readGyroscope(gyroX, gyroY, gyroZ);

  // Display gyroscope data
  Serial.print("GyroX: ");
  Serial.print(gyroX);
  Serial.print("\tGyroY: ");
  Serial.print(gyroY);
  Serial.print("\tGyroZ: ");
  Serial.println(gyroZ);

  // Publish gyroscope data to MQTT broker
  if (millis() % sendInterval < 10) {
    // Begin MQTT message
    mqtt.beginMessage(pubTopic);

    // Include gyroscope data in the message
    mqtt.print("GyroX: ");
    mqtt.print(gyroX);
    mqtt.print("\tGyroY: ");
    mqtt.print(gyroY);
    mqtt.print("\tGyroZ: ");
    mqtt.print(gyroZ);

    // End MQTT message
    mqtt.endMessage();
    delay(10);
  }
}
