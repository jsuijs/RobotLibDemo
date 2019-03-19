#!/usr/bin/python3
# -*- coding: cp1252 -*-
# blob_msg.py - convert msg string to stuct (class) & do some checks
import rl_comms as rl      # RobotLib common code

TestMsg  = "BLOB START PolarScan GRAPH1 - w\n"
TestMsg += "BLOB BLK 0 agxoDFMMDSeJDA0nUAcNJ8wGKwUNJysExQOjAw0nDSchAxsDDicNJ/gJGwnpCA4nfggOJw4nDSfLAcYBvgHDAQ==\n"
TestMsg += "BLOB BLK 1 yAHgAQ0nDScOJw0nZgENJ14BWgFYAQ0nWgFbAV8BaQEOJw4nDidnBFYEDicOJw4nDicNJw0nDScOJxMBDAEPAQ==\n"
TestMsg += "BLOB BLK 2 DScMAQgBDicNJw0nDicOJ54DlgOPAw4niAOGA4UDfQMNJ38DeQN2A3MDDSd1A3IDcQNyAw0ncAN0A3QDdAMNJw==\n"
TestMsg += "BLOB BLK 3 eQN5A34DgAMNJ4YDhwONA5MDmQMNJ58DpAOsA7QDDSe9A8UDzwPZA+gDDScNJ8MDnwMNJ30DXQM+Aw0nJQMNAw==\n"
TestMsg += "BLOB BLK 4 +gLlAg0n0QK9Aq4CngINJ40CfwJzAg0nZgJdAlQCDScNJw0nDScNJw4nDicOJw4nDicOJw4nBQIHAgQCDif+AQ==\n"
TestMsg += "BLOB BLK 5 +wH3AQ0n9AHxAe8B7AHqAQ0n6AHnAeIBDSfQAfEB2AH2AQ0n8AHoAQ==\n"
TestMsg += "BLOB END 21401\n"

class BlobMsg:
    def __init__(self, InMsgString):

      self.MsgString = InMsgString.replace('Á', '').replace('À', '')
      self.MsgFlags = 0

      # convert message into fiels and raw (binary) data
      self.RawData = bytearray()
      for line in self.MsgString.splitlines() :
         Fields = line.split()
         #print(fields)

         if Fields[0] != "BLOB" :
            print("Error: invalid msg format")
            return

         if Fields[1] == "START" :
            self.Source       = Fields[2]
            self.Destination  = Fields[3]
            self.MetaFormat   = Fields[4]
            self.LineFormat   = Fields[5]
            self.MsgFlags += 2   # add exactly once
#            PolarScan GRAPH1 - wÀ

         if Fields[1] == "BLK" :
            import base64
            encoded = Fields[3]
            decoded = base64.b64decode(encoded)
            self.RawData = self.RawData + decoded
            self.MsgFlags |= 1   # OR once or more times

         if Fields[1] == "END" :
            self.Crc = int(Fields[2])
            self.ValidMsg = True
            self.MsgFlags += 4   # add exactly once

      if self.Crc == rl.crc16(self.RawData) :
         self.MsgFlags += 8   # add exactly once
      else :
         print("Crc Error")

      if self.MsgFlags == 15 :
         self.MsgOk = True
      else :
         self.MsgOk = False
