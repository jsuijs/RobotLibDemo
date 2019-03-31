#!/usr/bin/python3
# -*- coding: cp1252 -*-
import blob_msg as blob

def Export(MessageBuffer, FormatString) :

   print(MessageBuffer)

   # convert message format to binary
   RawData = bytearray()
   for line in MessageBuffer :
      fields = line.split()
      #print(fields)
      if fields[1] == "BLK" :
         import base64
         encoded = fields[3]
         decoded = base64.b64decode(encoded)
         if int(fields[2]) == 0 :
            print("FCB: ", decoded)
         else :
            RawData = RawData + decoded

   if (len(RawData) == 0) :
      print("No data to export, abort.")
      return

   print("Bytes to export:", len(RawData))

   # expand format string
   print(FormatString)
   HexDumpFlag = False
   Multiplier = 0
   NewFormat = ""
   while len(FormatString) :
      c = FormatString[:1].lower()
      FormatString = FormatString[1:]

      if c.isdigit() :
         Multiplier = Multiplier * 10 + int(c)
         continue

      if c == ' ': continue
      if c == 'h':
         NewFormat = 'h'   # Hex mode overrides all other formats.
         break

      # place char one or more times in output
      if Multiplier == 0 : Multiplier = 1

      for _ in range(Multiplier):
#         print("aa ", c, "-", Multiplier)
         NewFormat += c
      Multiplier = 0
   print(NewFormat)

   OutBuffer = ""

   # start processing of data.
   if NewFormat == 'h' :
      return blob.HexDump(RawData)

   # convert binary data to requested format
   while len(RawData) :
      TmpFormat = NewFormat
      while len(TmpFormat) :
         c = TmpFormat[:1].lower()
         TmpFormat = TmpFormat[1:]

         if c == 'b' :     # byte values
            Value = RawData[0]
            RawData = RawData[1:]
            OutBuffer += str(Value) + '\t'
            continue
         if c == 'w' :     # word values
            Value = RawData[0] + 256 * RawData[1]
            RawData = RawData[2:]
            OutBuffer += str(Value) + '\t'
            continue

         print("Error: unknown format char (", c, ")")
         return
      OutBuffer = OutBuffer[:-1] + "\n" # change last tab to line end, each time the format string has been processed

   return OutBuffer
