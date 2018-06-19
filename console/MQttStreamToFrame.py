#!/usr/bin/env python
import RLCommon

import paho.mqtt.client as mqtt
import os
from time import sleep

# MQTT callback - connect
def on_connect(client, userdata, flags, rc):
   print("Connection returned result: ", rc)
   mqttc.subscribe("Robotlib/ComRawRx", qos=0)
   if rc == 0:
      #rc 0 successful connect
      print("Connected")
   else:
      raise Exception

# MQTT callback - rx message
def on_message(client, userdata, message):
#    print("Received message '" + str(message.payload) + "' on topic '"
#        + message.topic + "' with QoS " + str(message.qos))
   Packets = MySlip.decode(message.payload)
   for Packet in Packets:
      print(Packet)
      mqttc.publish("Robotlib/MsgFromRobot", Packet)

try:

   ConfigData  = RLCommon.LoadCfg()
   MySlip      = RLCommon.FrameDecoder()
   mqttc       = mqtt.Client("StreamToFrame_"+str(os.getpid()))

   print("Connect to mqtt server at " + ConfigData['MqttIp'])
   mqttc.on_connect = on_connect
   mqttc.on_message = on_message
   mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)

   mqttc.loop_start() # run mqtt client in separate thread

   while 1 :
      sleep(0.05)

   pass

# handle app closure
except (KeyboardInterrupt):
   print("Ctrl-C received, exit.")

except Exception as ex:
   template = "== An exception of type {0} occurred. =="
   message = template.format(type(ex).__name__)
   print(message)
   import traceback
   print(traceback.format_exc())
