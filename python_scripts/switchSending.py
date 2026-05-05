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
    "FUEL_CLOSE" : {"valve" : "fuel", "pos" : "open"},
    "FUEL_OPEN" : {"valve" : "fuel", "pos" : "open"},
    "OX_CLOSE" : {"valve" : "ox", "pos" : "open"},
    "OX_OPEN" : {"valve" : "ox", "pos" : "open"},
    "FILL_CLOSE" : {"valve" : "fill", "pos" : "open"},
    "FILL_OPEN" : {"valve" : "fill", "pos" : "open"},
    "DUMP_CLOSE" : {"valve" : "dump", "pos" : "open"},
    "DUMP_OPEN" : {"valve" : "dump", "pos" : "open"},
    "ESTOP" : {"sequence" : "estop"},
    "ABORT" : {"sequence" : "abort"},
    "LAUNCH_CONV" : {"sequence" : "launch", "prop_delay" : 0.2, "ox_delay" : 0.2},
    "LAUNCH_POPPET" : {"sequence" : "launch", "prop_delay" : 0.2}
}

def on_disconnect_callback(client, userdata, rc): # last will
    if rc != 0:
        print("connection lost, closing all valves")
        client.publish(SEQUENCE_TOPIC, json.dumps(payloads["ABORT"]))


client = mqtt.Client()
client.on_disconnect = on_disconnect_callback

client.connect("localhost", 1883, 10)

while (1):
    client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    time.sleep(1)
    client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    time.sleep(1)



