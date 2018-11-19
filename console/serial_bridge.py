#!/usr/bin/env python
from __future__ import print_function

# sudo apt-get install python-serial
# sudo apt-get install python3-serial
import serial
from serial.tools import list_ports

import paho.mqtt.client as mqtt
import sys
from time import sleep
import json
import rl_comms as rl    # RobotLib common code


class SerialBridge:

   def __init__(self, CoData):
      self.ConfigData = CoData

      # Open serial port
      try:
         print("Connecting... ", self.ConfigData['Bridge']['SerialPort'])
         self.ser = serial.Serial(self.ConfigData['Bridge']['SerialPort'], 115200, timeout=0, write_timeout=0)
         self.ser.flushInput()

      except:
         print("Failed to connect serial '", self.ConfigData['Bridge']['SerialPort'], "', ports on this system:")
         ports = list_ports.comports()
         ports.sort()
         for p in ports:
            print(p)
         raise SystemExit

      # setup mqtt
      self.MqttClient = rl.MQttClient(self.ConfigData['MqttIp'], "Robotlib/ComRawTx")
      self.MqttClient.mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge started.]]\r\n")

      self.MySlip = rl.FrameDecoder()

   def cleanup(self):
      self.MqttClient.mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge terminated.]]\r\n")
      self.MqttClient.mqttc.disconnect()
      self.ser.close()
      print("Cleaning up done.")

   def main(self) :
      try:  # start of main loop

         #read data from serial and publish
         while 1 :
            # this delay has 2 purposes:
            # 1. it reduces cpu load when idle
            # 2. it allows for more char to arrive during high load, so
            #    less (but larger) MQTT messages are required (which seems
            #    to provide better performance on the broker/receiving end).
            sleep(0.05)
            if  self.ser.inWaiting() > 0:
               line = self.ser.read(self.ser.inWaiting())

               # post on Mqtt
               if self.ConfigData['Bridge']['UseMqtt'] :
                  self.MqttClient.mqttc.publish("Robotlib/ComRawRx", line)

                  # frame decoder build in
                  if self.ConfigData['Bridge']['FrameToMqtt'] :
                     Packets = self.MySlip.decode(line)
                     for Packet in Packets:
                        #print(Packet)
                        self.MqttClient.mqttc.publish("Robotlib/MsgFromRobot", Packet)

               if 0 :   # print *also* to stdout
                  if (sys.version_info > (3, 0)):
                     print(line.decode("cp1252", 'ignore'), end='')
                  else:
                     print(line, end='')
                  sys.stdout.flush()

            # process message from Mqtt to Serial
            while len(self.MqttClient.MsgQueue) :
               Message = self.MqttClient.MsgQueue.pop(0)
               self.ser.write(Message.encode("cp1252"))
               self.ser.flush()
         pass # end of main loop

      # handle app closure
      except (KeyboardInterrupt):
         print("Ctrl-C received, exit.")
         self.cleanup()


      except Exception as ex:
         print("com port gone?")
         message = "== An exception of type {0} occurred. ==".format(type(ex).__name__)
         print(message)
         import traceback
         print(traceback.format_exc())
         self.cleanup()
         sys.exit("failed")

if __name__ == "__main__":

   # get config data
   CData  = rl.LoadCfg("C:\\MyRobot\\arm\\ConsoleShortcuts\\rl_cfg.json")

   # change specific settings here (ROS params)

   Bridge = SerialBridge(CData)  # setup serial & mqtt
   Bridge.main()                 # infinite loop