#!/usr/bin/python3
# -*- coding: cp1252 -*-
# set encoding to allow 'slip' chars like
# ÁFILE  FCB aap 11À

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
import blob_msg as blob
import file_export as FileExport
import tkinter.scrolledtext as tkst

import os, time, sys

#------------------------------------------------------------------------------
def ReadListFile(FileName) :
   print("ReadListFile '" + FileName + "'")
   global Sources, Addresses, MemoryRecords

   Sources = {}
   SourceName = "UnKnown_error"
   MemoryLines = []

   Mode = 0

   with open(FileName) as f:
     for L in f:

      #print(Mode, L)
      Fields = L.split()

      #----------------
      # collect sources
      #----------------
      if L.startswith("**** SOURCE") :
         if Mode == 1 :
            # we're scanning, so save result
            Sources[SourceName] = SourceLines
         # setup to collect (next) source lines
         Mode = 1
         SourceName = Fields[2]
         SourceLines = []
         continue

      if L.startswith("**** END of sources") :
         if Mode != 1 :
            print("Error scanning sources")
            sys.exit(0)
         # store last source and move to next mode (data)
         Sources[SourceName] = SourceLines
         Mode = 10

      if Mode == 1 :
         i = L.find('#')
         if i != -1 :
            SourceLines.append(L[i+1:])

      #--------------------
      # collect memory info
      #--------------------
      if Mode == 10 :
         if L.startswith(";----------------------------------------------") :
            # trigger, next line is info
            Mode = 11;
            continue

      if Mode == 11 :
            # store memory info
            MemoryLines.append(L)
            Mode = 10;
            continue

   # dump sources
   if 0 :
      print("Sources dump************")
      for i in Sources.keys() :
         print(i)
         for L in Sources[i] :
            print(">>" + L, end='')

   #print(LineList)
   MemoryRecords = {}
   for L in MemoryLines :
      Fields = L.split()
      i = int(Fields[1], 16)
      MemoryRecords[i] = Fields[0]

   # create list of known addresses (sorted, with gaps)
   Addresses = sorted(MemoryRecords.keys())

   # dump addresses
   if 0 :
      print("Address dump***************")
      print(Addresses)
      for i in Addresses :
         print(i, MemoryRecords[i])

#------------------------------------------------------------------------------
def ShowSourceLocation(MemAddr) :

   print("ShowSourceLocation", MemAddr)

   Result = 0
   for A in Addresses :
      if A > MemAddr : break
      Result = A

   #print(Result, MemoryRecords[Result])
   Fields = MemoryRecords[Result].split(':')
   #print(Fields[0], format(int(Result), '04x'), Fields[1])

   Src = Fields[0]               # list of source lines
   LineNr = int(Fields[1])-1     # 0-based index of line to highlight

   # select lines to show
   WinHeight = Memo.cget('height') - 1
   Start = LineNr - int(WinHeight / 2)
   if Start < 0 : Start = 0
   End = Start + WinHeight
   if End > len(Sources[Src]) :
      End = len(Sources[Src]) + 1
      Start = End - WinHeight
      if Start < 0 : Start = 0

   Memo.delete('1.0', tk.END)
   while Start < End :
      if Start >= len(Sources[Src]) :
         Line = "<eof>"
      else :
         Line = Sources[Src][Start]
      if Start == LineNr :
         Memo.insert(tk.END, Line, "highlight")
      else :
         Memo.insert(tk.END, Line)
      Start += 1

#------------------------------------------------------------------------------
def StatusMsg(data) :
   data = str(data).rstrip()
   print("StatusMsg:", data)
   LabelStatus['text']  = data

#------------------------------------------------------------------------------
def ClickCMeta() :
   Msg.MetaFormat = MetaDataFormat.get()

   (Error, List) = Msg.GetMetaData()

   if Error == "" :
      Error = "MetaData converted." # not really an error...
   StatusMsg(Error)

#------------------------------------------------------------------------------
def DataTakt():
   #print("DataTakt")

   global Teller
   if Teller < 100 :
      ShowSourceLocation(Teller)
      Teller += 1


   # process messages
   while len(mqttc.MsgQueue) > 0 :
      Message = mqttc.MsgQueue.pop(0)
      fields = Message.split()             # whitespace separated
      if fields[0] == "BASIC_TRC" :
         print("msg:", len(fields), fields)
#         LabelRobotName['text'] = fields[1]
#         if fields[2] == "2017-01-01" :
#            # clockset  dd mm yyyy hh mm ss - set clock date & time
#            Data = datetime.datetime.now().strftime("clockset %d %m %Y %H %M %S\r")
#            MemoAdd("Send time: " + Data)
#            mqttc.mqttc.publish("Robotlib/ComRawTx", Data)
         continue

   # reload file on change
   global FileTime
   NewTime = os.stat(sys.argv[2]).st_mtime
   if FileTime == 0 : FileTime = NewTime
   if NewTime != FileTime :
      NewTime = FileTime
      ReadListFile(ListFileName)

   root.after(20, DataTakt)
   # end of DataTakt
Teller = 0
FileTime = 0

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
# start of main code
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def main(): # dummy for Ultraedit function list
   pass
# main at root level.

#print('Number of arguments:', len(sys.argv), 'arguments.')
#print('Argument List:', sys.argv[1], sys.argv[2])

if len(sys.argv) != 3:
   print("Use:", sys.argv[0], "<cfg_file_name> <list_file_name>")
   sys.exit(0)

CfgFileName =  sys.argv[1]
ListFileName = sys.argv[2]

#------------------------------------------------------------------------------
root = tk.Tk()
root.wm_title(os.path.basename(__file__) + ' - ' + ListFileName)

# resize stuff
root.columnconfigure(7, weight = 3 )
root.rowconfigure(3, weight = 3 )

ConfigData = rl.LoadCfg(CfgFileName)

# print path - implies check of config data
print("File root: ", ConfigData['FileService']['FileRoot'])

mqttc = rl.MQttClient(ConfigData['MqttIp']) # setup & connect MQtt client to receive messages from robot

#------------------------------------------------------------------------------
# Horizontal (top) bar + Conversion button & formats
HBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
HBBar.grid(row=0, column=0, columnspan=10, sticky=(tk.N, tk.E, tk.S, tk.W))

BtnCData      = tk.Button(HBBar, text='C-Data') #,  command=ClickCData)
BtnCData.pack(side=tk.LEFT)
createToolTip(BtnCData,"Convert data of message in top window, result in lower window")

tk.Label(HBBar, text="  Conversion formats:").pack(side=tk.LEFT)

MetaDataFormat = tk.Entry(HBBar, width=10)
MetaDataFormat.pack(side=tk.LEFT)
MetaDataFormat.insert(0, "-")
createToolTip(MetaDataFormat,  "MetaData Conversion format (b, w, i, f, number as multiplier)")

#------------------------------------------------------------------------------
# Vertical (right hand) bar + buttons
VBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
VBBar.grid(row=1, column=9, rowspan=3, sticky=(tk.N, tk.E, tk.S, tk.W))

B = tk.Button(VBBar, text='Paste') #,   command=ClickPaste)
B.pack(side=tk.TOP)
createToolTip(B,"Paste from clipboard to top (message) window.")

#------------------------------------------------------------------------------
# Paned (resizable) window with 2 memo fields
Paned = tk.PanedWindow(orient=tk.VERTICAL)
Paned.grid(row=3, column=0, columnspan=8, sticky=(tk.N, tk.E, tk.S, tk.W))

Memo = tkst.ScrolledText(root, height=40, width=80, wrap="none", bg="#f8d4ce")
Memo.grid(row=3, column=0, columnspan=8, sticky=(tk.N, tk.E, tk.S, tk.W))
Memo.tag_config("highlight", background="dodger blue", foreground="white")


#------------------------------------------------------------------------------
# bottom line
LabelStatus  = tk.Label(root, text="Startup ready.")
LabelStatus.grid(row=4, column=0,  columnspan=8, sticky=(tk.W))

# -----------------------------------------------------------------------------
ReadListFile(ListFileName)

#------------------------------------------------------------------------------
# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
