#pragma once

#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


extern IPAddress ip;
extern IPAddress broker;

void on_message_callback(char* topic, byte* payload, unsigned int length);
void init_networking();

extern Wiznet55rp20lwIP eth;
extern WiFiClient ethClient;
extern PubSubClient mqttClient;