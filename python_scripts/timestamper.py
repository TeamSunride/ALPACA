import socket
import time
import LabJackPython
import ue9
from datetime import datetime
import traceback
import sys

# InfluxDB line protocol:
# measurement,tag1=val1 field1=val1,field2=val2 timestamp_ns

SCAN_FREQUENCY = 15000
MEASUREMENT = "ue9_ain"
TAGS = "device=labjack"
CHANNELS = ["AIN0", "AIN1", "AIN2", "AIN3"]

start = None
stop = None

def formatInfluxLine(r, SCAN_FREQUENCY, batch_start_ns):
    num_samples = len(r[CHANNELS[0]])
    scan_period_ns = int(1e9 / SCAN_FREQUENCY) # 1/f * 1e9 to convert to nanoseconds

    for i in range(num_samples):
        offset_ns = (num_samples - 1 - i) * scan_period_ns # multiply scan period by how many periods earlier the ith sample is to the last sample
        sample_timestamp_ns = batch_start_ns - offset_ns

        components = []
        for ch in CHANNELS:
            raw_voltage = r[ch][i]
            formatted = "%s=%.6f" % (ch, raw_voltage)
            components.append(formatted)
        fields = ",".join(components)

        yield "%s,%s %s %d\n" % (MEASUREMENT, TAGS, fields, sample_timestamp_ns)


def connectSocket():
    while True:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect(("localhost", 8094))
            print("socket connected")
            return sock
        except Exception as e:
            print("could not connect to socket: %s , retrying" % e)
            time.sleep(2)

def connectLabjack():
    while True:
        try:
            d = ue9.UE9(ethernet=True, ipAddress="192.168.1.52")
            d.getCalibrationData()
            # ChannelOptions: 0 = +-10V | 1 = +-1V | 2 = +-0.1V | 3 = +-0.01V | 8 = 0-20V
            d.streamConfig(NumChannels=4, ChannelNumbers=[0, 1, 2, 3], ChannelOptions=[0, 0, 0, 0], SettlingTime=0, Resolution=12, ScanFrequency=SCAN_FREQUENCY)
            print("Labjack connected")
            return d
        except Exception as e:
            print("connection failed: %s , retrying" % e)
            time.sleep(5)

while True:
    sock = connectSocket()
    d = connectLabjack()

    missed = 0
    dataCount = 0
    packetCount = 0
            

    try:
        print("start stream, CTRL+C to stop")
        d.streamStart()
        start = datetime.now()

        for r in d.streamData():
            if r is None:
                print("no data")
                continue

            batch_start_ns = time.time_ns()

            if r["errors"] != 0:
                print("Errors: %s at %s" % (r["errors"], datetime.now()))
            if r["numPackets"] != d.packetsPerRequest:
                print("missing packets! requested: %s received: %s" % (d.packetsPerRequest, r["numPackets"]))
            if r["missed"] != 0:
                missed += r["missed"]
                print("missed %s samples" % r["missed"])

            for line in formatInfluxLine(r, SCAN_FREQUENCY, batch_start_ns):
                sock.sendall(line.encode())  

            dataCount += 1
            packetCount += r["numPackets"]


    except KeyboardInterrupt:
        print("stopping stream")
        sys.exit(0)

    except:
        print("".join(i for i in traceback.format_exc()))

    finally:
        stop = datetime.now()

        try:
            d.streamStop()
        except:
            pass

        try:
            d.close()
        except:
            pass
        
        try:
            sock.close()
        except:
            pass

        print("stream stopped")
        if dataCount > 0:
            runtime = (stop - start).seconds + (stop - start).microseconds / 1e6
            print("runtime: %.2fs | missed: %d" % (runtime, missed))

    print("connection lost, retrying")
    time.sleep(5)