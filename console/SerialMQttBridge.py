#!/usr/bin/env python
from __future__ import print_function

import serial
from serial.tools import list_ports

import paho.mqtt.client as mqtt
import sys
import os
import time
from time import sleep
import json

try:
   with open('SerialMQttBridgeCfg.json', 'r') as fp:
      ConfigData = json.load(fp)

except:
   # no config => load defaults and & save (template) config file
   ConfigData = {  'SerialPort': '\\\\.\\com3',
                   'MqttIp'    : "127.0.0.1",
                   'MqttPort'  : 1883}
   with open('SerialMQttBridgeCfg.json', 'w') as fp:
      json.dump(ConfigData, fp, sort_keys=True, indent=4)

#MQTT callbacks

def on_connect(client, userdata, flags, rc):
   print("Connection returned result: ", rc)
   mqttc.subscribe("Robotlib/ComRawTx", qos=0)
   if rc == 0:
      #rc 0 successful connect
      print("Connected")
   else:
      raise Exception

def on_publish(client, userdata, mid):
   if userdata is None:
      userdata = "^"
   print("Published " + userdata, mid)

# subscribe part
def on_message(client, userdata, message):
#    print("Received message '" + str(message.payload) + "' on topic '"
#        + message.topic + "' with QoS " + str(message.qos))
   ser.write(message.payload)
   ser.flush()

def cleanup():
   mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge terminated.]]\r\n")
   mqttc.disconnect()
   print("Ending and cleaning up")
   ser.close()

try:
   print("Connecting... ", ConfigData['SerialPort'])
   ser = serial.Serial(ConfigData['SerialPort'], 115200, timeout=0, write_timeout=0)

except:
   print("Failed to connect serial, ports on this system:")
   ports = list_ports.comports()
   ports.sort()
   for p in ports:
      print(p)
   raise SystemExit

try:
   ser.flushInput()
   #create an mqtt client
   mypid = os.getpid()
   client_uniq = "arduino_pub_"+str(mypid)
   mqttc = mqtt.Client(client_uniq)

   #attach MQTT callbacks
   mqttc.on_connect = on_connect
   mqttc.on_message = on_message

   #connect to broker
   mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)
   mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge started.]]\r\n")


   mqttc.loop_start() # run mqtt client in separate thread

   #read data from serial and publish
   while 1 :
      # this delay has 2 purposes:
      # 1. it reduces cpu load when idle
      # 2. it allows for more char to arrive during high load, so
      #    less (but larger) MQTT messages are required (which seems
      #    to provide better performance on the broker/receiving end).
      sleep(0.05)
      if  ser.inWaiting() > 0:
         line = ser.read(ser.inWaiting())
         mqttc.publish("Robotlib/ComRawRx", line)
         if (sys.version_info > (3, 0)):
            print(line.decode("cp1252", 'ignore'), end='')
         else:
            print(line, end='')
         sys.stdout.flush()

   pass

# handle app closure
except (KeyboardInterrupt):
   print("Ctrl-C received, exit.")
   cleanup()

except (RuntimeError):
   print("uh-oh! time to die")
   cleanup()

except Exception as ex:
   print("com port verdwenen?")
   template = "== An exception of type {0} occurred. =="
   message = template.format(type(ex).__name__)
   print(message)
   import traceback
   print(traceback.format_exc())
   cleanup()
