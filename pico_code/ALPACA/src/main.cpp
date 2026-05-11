#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <PubSubClient.h>

#include "actuation.h"
#include "networking.h"


void setup() {
  Serial.begin();
  init_networking();
  init_servos();
  
}

void loop() {
  mqttClient.loop();
  delay(10);

}
