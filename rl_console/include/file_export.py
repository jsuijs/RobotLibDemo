#!/usr/bin/python3
# -*- coding: cp1252 -*-

def Dump(MessageBuffer) :

   for line in MessageBuffer :
      fields = line.split()
      print(fields)
      MyTmp = bytearray()
      if fields[1] == "BLK" :
         import base64
         encoded = fields[3]
         decoded = base64.b64decode(encoded)
         if int(fields[2]) == 0 :
            print("FCB: ", decoded)
         else :
            MyTmp = MyTmp + decoded
   print("Data: ", MyTmp)

