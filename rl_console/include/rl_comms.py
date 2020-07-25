# intended to be used with Python2 and Python3
import socket

# Configuration file stuff ----------------------------------------------------
def LoadCfg(filename = 'RlCommonCfg.json') :
   # load configuration file
   import json
   try:
      with open(filename, 'r') as fp:
         CfgData = json.load(fp)

   except:
      # no config => load defaults and & save (template) config file
      print("LoadCfg - file not found, create default")
      CfgData =   {  'MqttIp'       : "127.0.0.1"        ,
                     'MqttPort'     : 1883               ,
                     'Bridge' : {
                        'SerialPort'   : '\\\\.\\com3'   ,
                        'UseMqtt'      : True            ,
                     },
                     'Terminal' : {
                        'PlayerDelay'  : 100             ,
                        'UploaderDelay': 100             ,
                        'LogFile'      : "Logfile.txt"   ,
                     },
                     'Track' : {
                        'WinScale'  : 0.20,
                        'WinSizeX'  : 3800,
                        'WinSizeY'  : 2600
                     },
                     'FileService' : {
                        'FileRoot'  : 'C:\\RobotFiles'
                     }
                  }

      with open(filename, 'w') as fp:
         json.dump(CfgData, fp, sort_keys=True, indent=4)

   return CfgData

# MQtt ------------------------------------------------------------------------
import paho.mqtt.client as mqtt
import os
import sys

class _MQttClient():
   def __init__(self, MqttIp, SubscribeTopic):

      import sys
      print(sys.version)

      #create an mqtt client
      mypid = os.getpid()
      client_uniq = "pc_pub_"+str(mypid)
      self._mqttc = mqtt.Client(client_uniq)

      self.MsgQueue=[]
      self.SubscribeTopic = SubscribeTopic;
      self.PassStream = False # default pass frames from stream.

      self.MySlip = FrameDecoder()  # used when PassStream remains false

      #attach MQTT callbacks
      self._mqttc.on_connect = self.on_connect
      self._mqttc.on_message = self.on_message

      #connect to broker
      print("Connect to mqtt server at " + MqttIp)
      self._mqttc.connect(MqttIp, 1883, 60)

      self._mqttc.loop_start()   # run mqtt client in separate thread

   def on_message(self, client, userdata, message):
      print("====")
      print("Received message:" + message.payload.decode("cp1252", 'ignore') + "'\non topic '"
          + message.topic + "' with QoS " + str(message.qos))
      MsgData = message.payload.decode("cp1252", 'ignore')
      if self.PassStream == True :
         # queue stream (raw input)
         print('apppend\n', MsgData)
         self.MsgQueue.append(MsgData)
      else :
         # queue frames (decode stream into frames, default)
         print('slip\n', MsgData)
         Packets = self.MySlip.decode(message.payload)
         if len(Packets):
            self.MsgQueue.extend(Packets)
            #print(Packets)

   def on_connect(self, client, userdata, flags, rc):
      print("Connection returned result: ", rc, self.SubscribeTopic)
      client.subscribe(self.SubscribeTopic, qos=0)
      if rc == 0:
         #rc 0 successful connect
         print("Connected")
      else:
         raise Exception

   def Publish(self, Data) :
      #Data = Data.encode("cp1252", 'ignore')
      print("mPub", self.PublishTopic, Data)
      self._mqttc.publish(self.PublishTopic, Data)

   def PublishReverse(self, Data) :
      # publish on subscribe topic, used for playback from terminal...
      #Data = Data.encode("cp1252", 'ignore')
      print("mPubR", self.SubcribeTopic, Data)
      self._mqttc.publish(self.SubcribeTopic, Data)

   def Disconnect(self) :
      self._mqttc.disconnect()


class _UdpClient():
   def __init__(self, SubscribePort):

      self.RxSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP

      self.RxSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
      self.RxSock.bind(("127.0.0.1", SubscribePort))
      self.RxSock.setblocking(0)

      self.MsgQueue=[]
      self.SubscribePort = SubscribePort;
      self.PassStream = False # default pass frames from stream.

      self.MySlip = FrameDecoder()  # used when PassStream remains false

      self.TxSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

   def Takt(self):
      try:
         while True :
            MsgData, addr = self.RxSock.recvfrom(65535) # buffer size is 65535 bytes
            print("received message:", MsgData)
#      print("====")
#      print("Received message:" + message.payload.decode("cp1252", 'ignore') + "'\non topic '"
#          + message.topic + "' with QoS " + str(message.qos))
            if self.PassStream == True :
               # queue stream (raw input)
               MsgData = MsgData.decode("cp1252", 'ignore')
               print('apppend\n', MsgData)
               self.MsgQueue.append(MsgData)
            else :
               # queue frames (decode stream into frames, default)
               print('slip\n', MsgData)
               Packets = self.MySlip.decode(MsgData)
               if len(Packets):
                  self.MsgQueue.extend(Packets)
                  print(Packets)
      except(BlockingIOError):
         return

   def Publish(self, Data) :
      #Data = Data.encode("cp1252", 'ignore')
      print("uPub", self.PublishPort, Data)
      self.TxSock.sendto(Data, ("127.255.255.255", self.PublishPort))

   def PublishReverse(self, Data) :
      # publish on subscribe topic, used for playback from terminal...
      #Data = Data.encode("cp1252", 'ignore')
      print("uPubR", self.SubscribePort, Data)
      self.TxSock.sendto(Data, ("127.255.255.255", self.SubscribePort))

   def Disconnect(self) :
      pass

# RlCommsClient  --------------------------------------------------------------
class RlCommsClient() :
   def __init__(self, CfgData, Reverse = False):
      self.UseUdp = CfgData['UseUdp']
      if self.UseUdp :
         SubscribePort = 5005
         if Reverse : SubscribePort = 5006
         self.udpc = _UdpClient(SubscribePort)
         self.udpc.PublishPort = 5006
         if Reverse : self.udpc.PublishPort = 5005

         print("RlCommsClient UPD, reverse:", Reverse)
         print("   Publish:  ", self.udpc.PublishPort)
         print("   Subscribe:", self.udpc.SubscribePort)

      else :
         SubscribeTopic = "Robotlib/ComRawRx"
         if Reverse : SubscribeTopic = "Robotlib/ComRawTx"
         self.mqc = _MQttClient(CfgData['MqttIp'], SubscribeTopic)
         self.mqc.PublishTopic = "Robotlib/ComRawTx"
         if Reverse : self.mqc.PublishTopic = "Robotlib/ComRawRx"

         print("RlCommsClient MQTT, reverse:", Reverse)
         print("   Publish:  ", self.udpc.PublishTopic)
         print("   Subscribe:", self.udpc.SubscribeTopic)

   def MsgCount(self):
      if self.UseUdp :
         self.udpc.Takt()
         return len(self.udpc.MsgQueue)
      else :
         return len(self.mqc.MsgQueue)

   def MsgGet(self):
      if self.UseUdp :
         return self.udpc.MsgQueue.pop(0)
      else :
         return self.mqc.MsgQueue.pop(0)

   def Publish(self, Data) :
      if isinstance(Data, str) : Data = Data.encode()
      if self.UseUdp :
         self.udpc.Publish(Data)
      else :
         self.mqc.Publish(Data)

   def PublishReverse(self, Data) :
      if isinstance(Data, str) : Data = Data.encode()
      if self.UseUdp :
         self.udpc.PublishReverse(Data)
      else :
         self.mqc.PublishReverse(Data)

   def Disconnect(self) :
      if self.UseUdp :
         self.udpc.Disconnect()
      else :
         self.mqc.Disconnect()

   def Raw(self) :
      if self.UseUdp :
         self.udpc.PassStream = True
      else :
         self.mqc.PassStream = True

# FrameDecoder ----------------------------------------------------------------
# loosely based on SLIP decoder by Roman Haefeli
# https://github.com/reduzent/pyslip
class FrameDecoder():

   def __init__(self):
      self.receiving = False
      self.escaped   = False
      self.packet    = ''
      self.SLIP_END        = '\xc0' # dec: 192
      self.SLIP_START      = '\xc1' # dec: 193
      self.SLIP_ESC        = '\xdb' # dec: 219
      self.SLIP_ESC_END    = '\xdc' # dec: 220
      self.SLIP_ESC_ESC    = '\xdd' # dec: 221
      self.SLIP_ESC_START  = '\xde' # dec: 222

   def decode(self, stream):
      packetlist = []
      if (sys.version_info > (3, 0)):
     		data = stream.decode('iso-8859-1')
      else:
      	data = stream

      for char in data:
         if char == self.SLIP_START:
            self.receiving = True
            self.escaped   = False
            self.packet = ''
            continue

         if self.receiving == False:
            continue

         if char == self.SLIP_END:
            packetlist.append(self.packet)
            self.receiving = False
            continue

         # Escape flag
         if char == self.SLIP_ESC:
            self.escaped = True
            continue

         if self.escaped:
            if char == self.SLIP_ESC_END:
               self.packet += self.SLIP_END

            if char == self.SLIP_ESC_ESC:
               self.packet += self.SLIP_ESC

            if char == self.SLIP_ESC_START:
               self.packet += self.SLIP_START

            self.escaped = False
            continue

         # all others
         self.packet += char

      return (packetlist)

# 'our' CRC16 -----------------------------------------------------------------
def crc16(data: bytes):
   data = bytearray(data)
   crc = 0
   for b in data:
      cur_byte = 0xFF & b
      crc ^= cur_byte
      for _ in range(0, 8):
         if (crc & 0x0001) :
            crc = (crc >> 1) ^ 0xA001
         else:
            crc >>= 1
         cur_byte >>= 1

   return crc & 0xFFFF