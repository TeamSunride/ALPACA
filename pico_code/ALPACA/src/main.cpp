#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <PubSubClient.h>

#include "actuation.h"
#include "networking.h"


void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  init_networking();
  init_servos();
  
}

void loop() {
  if (!mqttClient.connected())
  {
    Serial.println("mqtt disconnected, attempting to reconnect");
    estop_sequence();
    reconnect_mqtt();
    return;
  }
  
  mqttClient.loop();
  delay(10);

}