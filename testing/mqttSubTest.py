import paho.mqtt.client as mqtt
import sys

def handle_message(client, userdata, msg):
    print(f"{msg.topic}: {msg.payload.decode()}")


client = mqtt.Client()

client.on_message = handle_message

if client.connect("localhost", 1883, 60) != 0:
    print("Couldn't connect to the mqtt broker")
    sys.exit(1)

client.subscribe("test_topic")

try:
    print("ctrl c to exit")
    client.loop_forever()
except Exception:
    print("caught exception")
finally:
    print("disconnecting")
    client.disconnect