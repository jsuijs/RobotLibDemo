#!/usr/bin/python3
# -*- coding: cp1252 -*-


def HexDump(Data) :
   n = 0
   while len(Data) :
      #process in 16 byte chunks
      b = Data[:16]
      Data = Data[16:]

      s1 = " ".join([f"{i:02x}" for i in b])
      s1 = s1[0:23] + " " + s1[23:]
      s2 = "".join([chr(i) if 32 <= i <= 127 else "." for i in b])
      print(f"{n * 16:04x}  {s1:<{48}}  |{s2}|")
      n += 1

def Export(MessageBuffer, FormatString) :

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

   # start processing of data.
   if NewFormat == 'h' :
      HexDump(RawData)
      return

   # convert binary data to requested format
   while len(RawData) :
      TmpFormat = NewFormat
      while len(TmpFormat) :
         c = TmpFormat[:1].lower()
         TmpFormat = TmpFormat[1:]

         if c == 'b' :     # byte values
            Value = ord(RawData[:1])
            RawData = RawData[1:]
            print("_", Value, end='')
            continue

         print("Error: unknown format char (", c, ")")
         return
      print()

   print("format string export end")
