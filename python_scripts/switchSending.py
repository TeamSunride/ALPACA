# TOPIC STRUCTURE
# servos
# |--servos/set
# |--servos/status
# sequences
# |--sequences/trigger
# |--sequences/status


import paho.mqtt.client as mqtt
import time
import json

SERVO_TOPIC = "servos/set"
SEQUENCE_TOPIC = "sequences/trigger"

payloads = {
    "FUEL_CLOSE" : {"valve" : "fuel", "pos" : "close"},
    "FUEL_OPEN" : {"valve" : "fuel", "pos" : "open"},
    "OX_CLOSE" : {"valve" : "ox", "pos" : "close"},
    "OX_OPEN" : {"valve" : "ox", "pos" : "open"},
    "FILL_CLOSE" : {"valve" : "fill", "pos" : "close"},
    "FILL_OPEN" : {"valve" : "fill", "pos" : "open"},
    "DUMP_CLOSE" : {"valve" : "dump", "pos" : "close"},
    "DUMP_OPEN" : {"valve" : "dump", "pos" : "open"},
    "ESTOP" : {"sequence" : "estop"},
    "ABORT" : {"sequence" : "abort"},
    "LAUNCH_CONV" : {"sequence" : "launch_conv", "igniter_headstart_ms" : 200, "oxidiser_delay_ms" : 200},
    "LAUNCH_POPPET" : {"sequence" : "launch_poppet", "igniter_headstart_ms" : 200},
    "IGNITION_TEST" : {"sequence" : "ignition_test"}
}

def on_disconnect_callback(client, userdata, rc): # last will
    if rc != 0:
        print("connection lost, closing all valves")
        client.publish(SEQUENCE_TOPIC, json.dumps(payloads["ESTOP"]))


client = mqtt.Client()
client.on_disconnect = on_disconnect_callback

client.connect("localhost", 1883, 10)

while (1):
    client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    time.sleep(1)
    client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    time.sleep(1)




