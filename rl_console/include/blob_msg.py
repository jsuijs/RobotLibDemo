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
      self.MsgOk = False

      NewMessage = ""

      # convert message into fiels and raw (binary) data
      self.RawData = bytearray()
      for line in self.MsgString.splitlines() :
         Fields = line.split()
         #print(fields)
         if len(Fields) < 2 :
            continue # ignore any pre/post message garbage

         if Fields[0] != "BLOB" :
            continue # ignore any pre/post message garbage

         NewMessage += (line + '\n')

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

      # after processing all messages
      if self.MsgFlags != 7 :
         self.Error = "Invalid format (" + str(self.MsgFlags) +")"
         return

      if self.Crc != rl.crc16(self.RawData) :
         self.Error = "Crc Error"
         return

      # all went well
      self.MsgString = NewMessage
      self.MsgOk = True

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
# related functions (not methods)
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# expand multiplier in format string to chars
def ExpandFormatString(FormatString) :

   Multiplier = 0
   NewFormat = ""
   while len(FormatString) :
      c = FormatString[:1].lower()
      FormatString = FormatString[1:]

      if c.isdigit() :
         Multiplier = Multiplier * 10 + int(c)
         continue

      if c == ' ': continue   # spaces are allowed in gui, not in message
      if c == 'h':
         NewFormat = 'h'      # Hex mode overrides other formats.
         break
      if c == '-':
         NewFormat = '-'      # NONE (minus) overrides other formats (used for no meta data).
         break

      # place char one or more times in output
      if Multiplier == 0 : Multiplier = 1

      for _ in range(Multiplier):
#         print("aa ", c, "-", Multiplier)
         NewFormat += c
      Multiplier = 0

   return NewFormat

#------------------------------------------------------------------------------
# Count # of bytes used by format string.
# special return values:
#  0 : no bytes required (-)
# -1 : greedy ('h')
# -2 : invalid format
def CountFormatString(FormatString) :

   Length = 0
   for c in ExpandFormatString(FormatString) :

      # normal types
      if c == 'b' :
         Length += 1 # byte
         continue

      if c == 'w' :
         Length += 2 # word
         continue

      if c == 'i' :
         Length += 4 # int
         continue

      if c == 'f' :
         Length += 4 # float
         continue

      # special types
      if c == '-' :
         return 0 # no data (used for meta data)

      if c == 'h' :
         return -1 # hex dump (greedy)

      return -2 # invalid char
   return Length
