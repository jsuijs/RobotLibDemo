#!/usr/bin/python3

#### #!/usr/bin/env python
from __future__ import print_function

# sudo apt-get install python-serial
# sudo apt-get install python3-serial
import serial
from serial.tools import list_ports

import paho.mqtt.client as mqtt
import sys
from time import sleep

#import sys
#sys.path.append('..')
import rl_comms as rl      # RobotLib common code
#from   rl_gui  import *    # RobotLib common code

class SerialBridge:

   def __init__(self, CoData):
      self.ConfigData = CoData
      self.UseAltChannel = 0  # default, do not post on alternative channel
      self.AltChannel = []

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
      self.MqttClient.PassStream = True # we want raw data, not frames
      self.MqttClient.mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge started.]]\r\n")

      self.MySlip = rl.FrameDecoder()

   def cleanup(self):
      self.MqttClient.mqttc.publish("Robotlib/ComRawRx", "[[SerialBridge terminated.]]\r\n")
      self.MqttClient.mqttc.disconnect()
      self.ser.close()
      print("Cleaning up done.")

   def main(self) :
      while 1 :  # start of main loop
         takt

   def takt(self) :

      #read data from serial and publish
      try :
         # this delay has 2 purposes:
         # 1. it reduces cpu load when idle
         # 2. it allows for more char to arrive during high load, so
         #    less (but larger) MQTT messages are required (which seems
         #    to provide better performance on the broker/receiving end).
         sleep(0.01)
         if  self.ser.inWaiting() > 0:
            line = self.ser.read(self.ser.inWaiting())

            # post on Mqtt
            if self.ConfigData['Bridge']['UseMqtt'] :
               self.MqttClient.mqttc.publish("Robotlib/ComRawRx", line)

            if self.UseAltChannel :
               Packets = self.MySlip.decode(line)
               self.AltChannel = self.AltChannel + Packets
               print(Packets)


            if 1 :   # print *also* to stdout
               if (sys.version_info > (3, 0)):
                  print(line.decode("cp1252", 'ignore'))
               else:
                  print(line)
               sys.stdout.flush()

         # process message from Mqtt to Serial
         while len(self.MqttClient.MsgQueue) :
            Message = self.MqttClient.MsgQueue.pop(0)
            self.ser.write(Message.encode("cp1252"))
            self.ser.flush()

            if 1 :   # print *also* to stdout
               #if (sys.version_info > (3, 0)):
               print(">\n" + Message)
               #else:
               #   print("aaa" + daa + Message)
               sys.stdout.flush()


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
   CData  = rl.LoadCfg()

   # change specific settings here (ROS params)

   Bridge = SerialBridge(CData)  # setup serial & mqtt
   Bridge.main()                 # infinite loop