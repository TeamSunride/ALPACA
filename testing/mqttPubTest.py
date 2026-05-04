import paho.mqtt.client as mqtt
import sys


client = mqtt.Client()

if client.connect("localhost", 1883, 10) != 0:
    print("couldnt connect to broker")
    sys.exit(1)

client.publish("test_topic", "Hello world, publishing works ok")
client.disconnect()

