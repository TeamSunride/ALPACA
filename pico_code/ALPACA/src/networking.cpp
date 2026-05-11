#include "networking.h"
#include "actuation.h"

IPAddress ip(192, 168, 137, 11);  //local ip  CHANGE LATER
IPAddress broker(192, 168, 137, 121); //broker ip (ip of the pi), CHANGE LATER
Wiznet55rp20lwIP eth(20); //20 is internal CS pin
WiFiClient ethClient;
PubSubClient mqttClient(broker, 1883, on_message_callback, ethClient);

void on_message_callback(char* topic, byte* payload, unsigned int length){
    //copy the payload the memory as buffer will be overwritten

    byte* p = (byte*)malloc(length);
    memcpy(p, payload, length);

    JsonDocument payload_json;
    deserializeJson(payload_json, p);

    if (strcmp(topic, "servos/set") == 0)
    {
        const char* requested_valve = payload_json["valve"];
        const char* requested_position = payload_json["pos"];

        moveServo(requested_valve, requested_position);

        char response[100];
        sprintf(response, "moved %s valve to %s position", requested_valve, requested_position);
        mqttClient.publish("servos/status", response);
    }

    if (strcmp(topic, "sequences/trigger") == 0)
    {
        const char* requested_sequence = payload_json["sequence"];
        const char* param1 = payload_json["igniter_headstart_ms"];
        const char* param2 = payload_json["oxidiser_delay_ms"];

        triggerSequence(requested_sequence, param1, param2);

        char response[100];
        sprintf(response, "activated %s sequence", requested_sequence);
        mqttClient.publish("sequences/status", response);
    }

    free(p);

    return;
}

void init_networking(){
    eth.config(ip);
    if (!eth.begin()) //begin ethernet with static ip
    {
        Serial.println("cant start ethernet");
        while (1)
        {
            delay(1000);
        }
    }

    while (!eth.connected())
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("eth connected");
  
    Serial.println("local ip: ");
    Serial.println(eth.localIP());

    if (mqttClient.connect("picoClient"))
    {
        mqttClient.publish("helloworld", "hello world from pico");
        mqttClient.subscribe("servos/set");
        mqttClient.subscribe("sequences/trigger");
    }



}