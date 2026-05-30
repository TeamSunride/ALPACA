#include "networking.h"
#include "actuation.h"

IPAddress ip(192, 168, 137, 52);  //local ip
IPAddress broker(192, 168, 137, 50); //broker ip (ip of the pi)
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
        const char* valve = payload_json["valve"];
        const char* pos = payload_json["pos"];

        if (strcmp(valve, "fill") == 0 )
        {
          fill.writeMicroseconds( (strcmp(pos, "open") == 0 ) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS );
          // digitalWrite(LED_PIN, HIGH);
          // delay(1000);
          // digitalWrite(LED_PIN, LOW);
        }

        if (strcmp(valve, "ox") == 0 )
        {
          ox.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
        }

        if (strcmp(valve, "fuel") == 0 )
        {
          fuel.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
        }

        if (strcmp(valve, "dump") == 0 )
        {
          dump.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
        }

        char response[100];
        sprintf(response, "moved %s valve to %s position", valve, pos);
        mqttClient.publish("servos/status", response);
    }


    if (strcmp(topic, "sequences/trigger") == 0)
    {
        const char* sequence = payload_json["sequence"];
        if (strcmp(sequence, "estop") == 0 )
        {
          estop_sequence();
        }
        
        if (strcmp(sequence, "abort") == 0 )
        {
          abort_sequence();
        }
        
        if (strcmp(sequence, "launch_conv") == 0 )
        {
          launch_conv_sequence((int)payload_json["t_igniter1"], (int)payload_json["t_igniter2"], (int)payload_json["t_fuel"], (int)payload_json["t_ox"]);
        }
        
        if (strcmp(sequence, "launch_poppet") == 0 )
        {
          launch_poppet_sequence((int)payload_json["t_igniter1"], (int)payload_json["t_igniter2"], (int)payload_json["t_propellants"]);
          Serial.print("after calling");
        }
        
        if (strcmp(sequence, "ignition_test") == 0 )
        {
          ignition_test_sequence((int)payload_json["t_igniter1"], (int)payload_json["t_igniter2"]);
        }
        
        char response[100];
        sprintf(response, "%s sequence activated successfully", sequence);
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

    mqttClient.setKeepAlive(2);

    if (mqttClient.connect("picoClient", NULL, NULL, "status", 0, false, "pico disconnected, check cable? :/"))
    {
        mqttClient.publish("status", "connected, hello world from pico :))");
        mqttClient.subscribe("servos/set");
        mqttClient.subscribe("sequences/trigger");
    }
}

void reconnect_mqtt(){
  if (mqttClient.connect("picoClient", NULL, NULL, "status", 0, false, "pico disconnected, check cable? :/"))
  {
    Serial.println("mqtt reconnected");
    mqttClient.publish("status", "pico reconnected :DDDD");
    mqttClient.subscribe("servos/set");
    mqttClient.subscribe("sequences/trigger");

  } 
  else 
  {
    Serial.print("could not connect to mqtt broker, rc=");
    Serial.println(mqttClient.state());
  }
  
}