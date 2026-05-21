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
import serial

SERVO_TOPIC = "servos/set"
SEQUENCE_TOPIC = "sequences/trigger"

SERIAL_PORT = '/dev/ttyACM0'

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

def parse_keystroke(keystroke):
    if keystroke == "abort":
        return
    if keystroke == "estop":
        client.publish(SEQUENCE_TOPIC, json.dumps(payloads["ESTOP"]))
    if keystroke == "launch":
        client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"])) # CORRESPONDS TO INJECTOR TYPE
    if keystroke == "fill open":
        client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_OPEN"]))
    if keystroke == "fill close":
        client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_CLOSE"]))


client = mqtt.Client()
client.on_disconnect = on_disconnect_callback

client.connect("localhost", 1883, 10)

ser = serial.Serial(SERIAL_PORT)

while(ser.isOpen):
    keystroke = ser.readline()
    print(keystroke)
    parse_keystroke(keystroke)


# while (1):
#     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
#     time.sleep(1)
#     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
#     time.sleep(1)




