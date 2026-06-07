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
import sys

SERVO_TOPIC = "servos/set"
SEQUENCE_TOPIC = "sequences/trigger"

SERIAL_PORT = '/dev/ttyACM0'

# ALL DELAYS IN MILLISECONDS
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
    "LAUNCH_CONV" : {"sequence" : "launch_conv", "t_igniter1" : 0, "t_igniter2" : 100, "t_fuel" : 100, "t_ox" : 200},
    "LAUNCH_POPPET" : {"sequence" : "launch_poppet", "t_igniter1" : 0, "t_igniter2" : 100, "t_propellants" : 500},
    "IGNITION_TEST" : {"sequence" : "ignition_test", "t_igniter1" : 0, "t_igniter2" : 2000}
}
def on_connect_callback(client, userdata, flags, rc):
    if rc == 0:
        print("connected sucessfully to broker")
    else:
        print("connection failed with code " + rc)


def on_disconnect_callback(client, userdata, rc): # last will
    if rc == 0:
        print("clean disconnection")
    elif rc == 7:
        print("connection lost with rc = 7, will reconnect")
    else:
        print("unexpected disconnection " + rc)

# def on_publish_callback(client, userdata, mid):
#     print("message published: " + mid)


def parse_keystroke(key, client):
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
            client.publish(SEQUENCE_TOPIC, json.dumps(payloads["ESTOP"]), 1)
        case "6": #launch
            client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]), 1) #modify according to injector type
        case "7": #fill open
            client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_OPEN"]), 1)
        case "8": #fill close
            client.publish(SERVO_TOPIC, json.dumps(payloads["FILL_CLOSE"]), 1)
        case "9": #fuel open
            client.publish(SERVO_TOPIC, json.dumps(payloads["FUEL_OPEN"]), 1)
        case "10": #fuel close
            client.publish(SERVO_TOPIC, json.dumps(payloads["FUEL_CLOSE"]), 1)
        case "11": #ox open
            client.publish(SERVO_TOPIC, json.dumps(payloads["OX_OPEN"]), 1)  
        case "12": #ox close
            client.publish(SERVO_TOPIC, json.dumps(payloads["OX_CLOSE"]), 1)
        case "13": #dump open
            client.publish(SERVO_TOPIC, json.dumps(payloads["DUMP_OPEN"]), 1)
        case "14": #dump close
            client.publish(SERVO_TOPIC, json.dumps(payloads["DUMP_CLOSE"]), 1) 
            

def main():
    client = mqtt.Client(client_id="switchbox", clean_session=True)
    client.on_connect = on_connect_callback
    client.on_disconnect = on_disconnect_callback
    client.will_set(SEQUENCE_TOPIC, json.dumps(payloads["ESTOP"]))
    # client.on_publish = on_publish_callback

    print("trying to connect")
    try:
        client.connect("localhost", 1883, keepalive=20)
    except Exception as e:
        print("failed to connect ")
        print(e)
        sys.exit(1)

    client.loop_start()
    time.sleep(1)


    try:
        ser = serial.Serial(SERIAL_PORT, timeout=1)
        print("serial port opened")
    except Exception as e:
        print("failed to open serial port ")
        print(e)
        client.loop_stop()
        sys.exit(1)

    while(ser.isOpen):
        keystroke = ser.readline()
        decoded_keystroke = keystroke.decode('ascii').strip()
        # print(decoded_keystroke)
        # print(type(decoded_keystroke))
        parse_keystroke(decoded_keystroke, client)


    # while (1):
    #     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    #     time.sleep(1)
    #     client.publish(SEQUENCE_TOPIC, json.dumps(payloads["LAUNCH_POPPET"]))
    #     time.sleep(1)

if __name__ == "__main__":
    main()


