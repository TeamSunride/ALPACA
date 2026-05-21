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
        print(rc)

def parse_keystroke(key):
    match key:
        case "0": #abort
            return 
        case "1": #prop armed
            return 
        case "2": #prop disarmed
            return
        case "3": #switches armed
            return
        case "4": #switches disarmed
            return
        case "5": #estop
            client.publish(SEQUENCE_TOPIC, json.dumps(payloads["ESTOP"]))
        case "6": #launch
            client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"])) #modify according to injector type
        case "7": #fill open
            client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_OPEN"]))
        case "8": #fill close
            client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_CLOSE"]))
        case "9": #fuel open
            client.publish(SERVO_TOPIC, json.dumps(payloads["FUEL_OPEN"]))
        case "10": #fuel close
            client.publish(SERVO_TOPIC, json.dumps(payloads["FUEL_CLOSE"]))
        case "11": #ox open
            client.publish(SERVO_TOPIC, json.dumps(payloads["OX_CLOSE"]))  
        case "12": #ox close
            client.publish(SERVO_TOPIC, json.dumps(payloads["OX_CLOSE"]))
        case "13": #dump open
            client.publish(SERVO_TOPIC, json.dumps(payloads["DUMP_OPEN"]))
        case "14": #dump close
            client.publish(SERVO_TOPIC, json.dumps(payloads["DUMP_CLOSE"])) 
            
        

client = mqtt.Client()
client.on_disconnect = on_disconnect_callback

client.connect("localhost", 1883, 15)

ser = serial.Serial(SERIAL_PORT)

while(ser.isOpen):
    keystroke = ser.readline()
    decoded_keystroke = keystroke.decode('ascii').strip()
    # print(decoded_keystroke)
    # print(type(decoded_keystroke))
    parse_keystroke(decoded_keystroke)


# while (1):
#     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
#     time.sleep(1)
#     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
#     time.sleep(1)




