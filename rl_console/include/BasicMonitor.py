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
   global Sources, VarList, Addresses, MemoryRecords

   Sources = {}
   SourceName = "UnKnown_error"
   MemoryLines = []
   VarList = {}

   Mode = 0

   with open(FileName) as f:
     for L in f:

      #print(Mode, L)
      Fields = L.split()

      #----------------
      # collect sources
      #----------------
      if L.startswith("**** SOURCE") :
         # new source file
         if Mode == 1 :
            # we're scanning, so save lines of previous sourcefile
            Sources[SourceName] = SourceLines
         # setup to collect lines of (next) sourcefile
         Mode = 1
         SourceName = Fields[2]
         SourceLines = []
         continue

      if L.startswith("**** END of sources") :
         if Mode != 1 :
            print("Error scanning sources")
            sys.exit(0)
         # store lines of last sourcefile and move to next mode (vars)
         Sources[SourceName] = SourceLines
         Mode = 5

      if Mode == 1 :
         i = L.find('#')
         if i != -1 :
            SourceLines.append(L[i+1:])

      #--------------------
      # collect var info
      #--------------------
      if Mode == 5 :
         if L.startswith("#### VARS") :
            Mode = 6
            continue

      if Mode == 6 :
         if L.startswith("#### END of VARS") :
            # end of vars
            Mode = 10
            continue
         # must be a var line, like 'S 0002 c'
         VarList[Fields[2]]  = int(Fields[1])
         print(Fields, int(Fields[1]), Fields[2])
         continue

      #--------------------
      # collect memory info
      #--------------------
      # MemoryLines look like: Test_A.upb:1:6 0007 [f00c]
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
      # convert 'Test_A.upb:1:6 0007 [f00c]'
      # to MemoryRecords[7] = 'Test_A.upb:1:6'
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

   # force display of new source data
   ShowSourceLocation(0)

   OptionList = ["-", *sorted(VarList.keys())]
   print("Set optionlist to", OptionList)
   SetOptionList(Pd1, Pd1TkStr, ChangePd1, OptionList)
   SetOptionList(Pd2, Pd2TkStr, ChangePd2, OptionList)
   SetOptionList(Pd3, Pd3TkStr, ChangePd3, OptionList)

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
   global VarList

   # process messages
   while len(mqttc.MsgQueue) > 0 :
      Message = mqttc.MsgQueue.pop(0)
      fields = Message.split()             # whitespace separated
      if fields[0] == "BASIC_TRC" :
         print("msg:", len(fields), fields)

         try :


            #----------------------------
            # field 1 = status flags
            # field 2 = address
            # field 3 = basic load
            # next: addr/data field pairs
            #----------------------------

            ShowSourceLocation(int(fields[2]))

            # Only show one of the status strings
            if int(fields[1]) & 1 : String = "Run"
            if int(fields[1]) & 2 : String = "Sleep"
            if int(fields[1]) & 4 : String = "Done"
            String = "Status: " + String + " (Load: " + fields[3] + "%)"
            StatusMsg(String)

            fields = fields[4:]  # remove fixed part

            # remove old values
            Pd1Value['text'] = ""
            Pd2Value['text'] = ""
            Pd3Value['text'] = ""

            while len(fields) > 1 : # loop through address/value pairs
               print("Addr:", fields[0], "Data:", fields[1]);
               if Pd1TkStr.get() in VarList :
                  if VarList[Pd1TkStr.get()] == int(fields[0]) : Pd1Value['text'] = fields[1]
               if Pd2TkStr.get() in VarList :
                  if VarList[Pd2TkStr.get()] == int(fields[0]) : Pd2Value['text'] = fields[1]
               if Pd3TkStr.get() in VarList :
                  if VarList[Pd3TkStr.get()] == int(fields[0]) : Pd3Value['text'] = fields[1]

               fields = fields[2:] # setup for next pair
            continue
         except:
            # try/except prevents the monitor to block on a corrupted message...
            message = "== An exception of type {0} occurred. ==".format(type(ex).__name__)
            print(message)
            import traceback
            print(traceback.format_exc())


   # reload file on change
   global FileTime
   try :
      NewTime = os.stat(sys.argv[2]).st_mtime
      if FileTime == 0 : FileTime = NewTime
      if FileTime != NewTime :
         FileTime = NewTime
         ReadListFile(ListFileName)
         print("file reloaded")
   except:
      print("file load error")

   root.after(20, DataTakt)
   # end of DataTakt
FileTime = 0


#------------------------------------------------------------------------------
# to change the options of PD menu (var list)
def SetOptionList(PD, tkvar, ChangePD, Options) :

   # remove options and add new ones
   m = PD.children['menu']
   m.delete(0, 'end')
   for i in Options:
      m.add_command(label=i, command=lambda x=i: ChangePD(x))

   # check if current selection is still valid
   if not tkvar.get() in Options :
      # no -> set to first entry
      tkvar.set(Options[0])

#------------------------------------------------------------------------------
# Tell robot which vars to send
def SetTraceVars() :
   V1 = -1
   if Pd1TkStr.get() in VarList.keys() : V1 = VarList[Pd1TkStr.get()]
   V2 = -1
   if Pd2TkStr.get() in VarList.keys() : V2 = VarList[Pd2TkStr.get()]
   V3 = -1
   if Pd3TkStr.get() in VarList.keys() : V3 = VarList[Pd3TkStr.get()]
   print(VarList)
   Msg = "basic tracevars " + str(V1) + " " + str(V2) + " " + str(V3) + "\r"
   print(Msg)
   mqttc.mqttc.publish("Robotlib/ComRawTx", Msg.encode())

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
   sys.exit(1)

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

##------------------------------------------------------------------------------
## Horizontal (top) bar + Conversion button & formats
#HBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
#HBBar.grid(row=0, column=0, columnspan=10, sticky=(tk.N, tk.E, tk.S, tk.W))
#
#BtnCData      = tk.Button(HBBar, text='C-Data') #,  command=ClickCData)
#BtnCData.pack(side=tk.LEFT)
#createToolTip(BtnCData,"Convert data of message in top window, result in lower window")
#
#tk.Label(HBBar, text="Load:").pack(side=tk.LEFT)
#
#MetaDataFormat = tk.Entry(HBBar, width=6)
#MetaDataFormat.pack(side=tk.LEFT)
#MetaDataFormat.insert(0, "-")
#createToolTip(MetaDataFormat,  "MetaData Conversion format (b, w, i, f, number as multiplier)")
#
#------------------------------------------------------------------------------
# Vertical (right hand) bar + buttons
VBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
VBBar.grid(row=1, column=9, rowspan=3, sticky=(tk.N, tk.E, tk.S, tk.W))

B = tk.Label(VBBar, text="VARS")
B.pack(side=tk.TOP)
createToolTip(B,"Variable inspector")

# Create a Tkinter variable
Pd1TkStr = tk.StringVar(root)
Pd2TkStr = tk.StringVar(root)
Pd3TkStr = tk.StringVar(root)

Pd1 = tk.OptionMenu(VBBar, Pd1TkStr, "-")
Pd2 = tk.OptionMenu(VBBar, Pd2TkStr, "-")
Pd3 = tk.OptionMenu(VBBar, Pd3TkStr, "-")

Pd1Value = tk.Label(VBBar, text="", width=10)
Pd2Value = tk.Label(VBBar, text="", width=10)
Pd3Value = tk.Label(VBBar, text="", width=10)

Pd1.pack(side=tk.TOP)
Pd1Value.pack(side=tk.TOP)
Pd2.pack(side=tk.TOP)
Pd2Value.pack(side=tk.TOP)
Pd3.pack(side=tk.TOP)
Pd3Value.pack(side=tk.TOP)

# change fuction for each PulDownList
def ChangePd1(x) : Pd1TkStr.set(x) ; Pd1Value['text'] = "" ; SetTraceVars()
def ChangePd2(x) : Pd2TkStr.set(x) ; Pd2Value['text'] = "" ; SetTraceVars()
def ChangePd3(x) : Pd3TkStr.set(x) ; Pd3Value['text'] = "" ; SetTraceVars()

# PullDown lists are filled by ReadListFile

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
# first time load here, without try so program fails if input does not exist.
ReadListFile(ListFileName)

#------------------------------------------------------------------------------
# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
