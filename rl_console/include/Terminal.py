#!/usr/bin/python3

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code

import tkinter.scrolledtext as tkst

import os
import time
import sys

MySlip            = rl.FrameDecoder()  # only for the constants

CmdHistory        = []
CmdHistoryIx      = -1  # keep trach of where we are, scrolling through history
CmdSaved          = ""  # the initial line (you might be editing already)

PlayerLines       = []
PlayerCount       = 0

UploaderLines     = []
UploaderCount     = 0

FrameFilterState  = 0

def LogStart():
   if LogStart.Flag:
      # we're logging => restart (close before open)
      LogStart.File.close()

   # actual start of logging.
   LogStart.File = open(ConfigData['Terminal']['LogFile'],"wb")

   # change (retain) logging state
   LogStart.Flag = True
   BtnLogStart["text"]  = "LogRestart"
   BtnLogEnd["text"]    = "LogEnd"
LogStart.Flag = False # on startup, we're not logging.

def LogEnd():
   if LogStart.Flag:
      # we're logging => logend
      # save
      LogStart.File.close()

      # Change to non-logging state
      LogStart.Flag = False
      BtnLogStart["text"]  = "LogStart"
      BtnLogEnd["text"]    = "SaveAll"

   else :
      # not logging => SaveAll
      File = open(ConfigData['Terminal']['LogFile'], "w", encoding="cp1252", errors='ignore')
      t = Memo.get('1.0', tk.END) # line 1, position 0 => beginning of text
      File.write(t)
      File.close()

   # launch file
   import platform
   if platform.system() == 'Windows' :
      os.startfile(ConfigData['Terminal']['LogFile'])
   else :
      import subprocess
      subprocess.call(["xdg-open", ConfigData['Terminal']['LogFile']])

def Player():
   global PlayerLines
   file_path = tk.filedialog.askopenfilename()
   if file_path != "":
      with open(file_path, encoding="cp1252", errors='ignore') as f:
          PlayerLines += f.readlines()

def Uploader():
   global UploaderLines
   file_path = tk.filedialog.askopenfilename()
   if file_path != "":
      with open(file_path, encoding="cp1252", errors='ignore') as f:
          UploaderLines += f.readlines()

def PlayerSendLines():
   global PlayerCount
   master.after(ConfigData['Terminal']['PlayerDelay'], PlayerSendLines)
   if len(PlayerLines) > 0 :
      Data = PlayerLines.pop(0).encode("cp1252", 'ignore')
      if args.com == None :
         # no com-port => mqtt
         mqttc.publish("Robotlib/ComRawRx", Data)
      else :
         ser.write(Data)
      PlayerCount += 1
      BtnPlayer["text"]="Player " + str(PlayerCount)
   else:
      PlayerCount = 0
      BtnPlayer["text"]="Player"

def UploaderSendLines():
   global UploaderCount
   master.after(ConfigData['Terminal']['UploaderDelay'], UploaderSendLines)
   if len(UploaderLines) > 0 :
      Data = UploaderLines.pop(0).encode("cp1252", 'ignore')
      if args.com == None :
         # no com-port => mqtt
         mqttc.publish("Robotlib/ComRawTx", Data)
      else :
         ser.write(Data)
      UploaderCount += 1
      BtnUpload["text"]="Upload " + str(UploaderCount)
   else:
      UploaderCount = 0
      BtnUpload["text"]="Upload"

def LineInputEnterKey(event):
   Data = (LineInput.get() + "\r").encode("cp1252", 'ignore')
   if args.com == None :
      mqttc.publish("Robotlib/ComRawTx", Data)
   else :
      ser.write(Data)

   global CmdHistory, CmdHistoryIx
   CmdHistory.insert(0, LineInput.get())
   LineInput.delete(0, tk.END)   # clear input window
   CmdHistoryIx = -1          # reset index

# command line history - up key
def LineInputUpKey(event):
   global CmdSaved, CmdHistoryIx
   if (CmdHistoryIx == -1):
      CmdSaved = LineInput.get()
   CmdHistoryIx += 1
   if CmdHistoryIx < len(CmdHistory):
      LineInput.delete(0, tk.END)    # clear input window
      LineInput.insert(0, CmdHistory[CmdHistoryIx])
   else:
      CmdHistoryIx -= 1

# command line history - down key
def LineInputDownKey(event):
   global CmdHistoryIx
   if (CmdHistoryIx > -1): # if there is a history...
      CmdHistoryIx -= 1;
      LineInput.delete(0, tk.END)    # clear input window
      if (CmdHistoryIx > -1):
         LineInput.insert(0, CmdHistory[CmdHistoryIx])
      else:
         LineInput.insert(0, CmdSaved)

# command line history - Escape key
def LineInputEscapeKey(event):
   #print("Escape!\n")
   LineInput.delete(0, tk.END)    # clear input window

#------------------------------------------------------------------------------
# create parser
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--com', help='COM port (disables mqtt)')
args = parser.parse_args()

master = tk.Tk()
master.wm_title(os.path.basename(__file__))

# resize stuff
master.columnconfigure(8, weight = 3 )
master.rowconfigure(1, weight = 3 )

ConfigData = rl.LoadCfg()


print("ConfigData:", ConfigData)

# line input window
tk.Label(master, text="Send").grid(row=2)
LineInput = tk.Entry(master)
LineInput.grid(row=2, column=1, columnspan=8, sticky=(tk.N, tk.E, tk.W))
LineInput.bind('<Return>', LineInputEnterKey) # bind function to enter keystroke
LineInput.bind('<Up>', LineInputUpKey)
LineInput.bind('<Down>', LineInputDownKey)
LineInput.bind('<Escape>', LineInputEscapeKey)

def MemoKey(event):
   if (event.char != '') :
      # printable character in memo field => focus to LineInput
      # (non-printable chars like control, shift, cursor movements
      #  and ctrl-c are valid on the memo window for text selection)
      LineInput.focus_set()
      LineInput.insert(INSERT, event.char)  # Beware UltraEdit: INSERT parameter in capitals!!

# text output window
Memo = tkst.ScrolledText(master, height=24, width=80, wrap="none")
Memo.grid(row=1, column=0, columnspan=9, sticky=(tk.N, tk.E, tk.S, tk.W))
Memo.configure(state='disabled')
Memo.bind("<Key>", MemoKey)

# buttons
CbValueFrames  = tk.IntVar()
BtnQuit     = tk.Button(master, text='Quit',     command=master.quit)
BtnLogStart = tk.Button(master, text='LogStart', command=LogStart)   # LogStart doubles as LogRestart
BtnLogEnd   = tk.Button(master, text='SaveAll',  command=LogEnd)     # LogEnd doubles as SaveAll
BtnUpload   = tk.Button(master, text='Upload',   command=Uploader)
BtnPlayer   = tk.Button(master, text='Player',   command=Player)
BtnFrames   = tk.Checkbutton(master, text='Frames', variable=CbValueFrames, relief='raised')
BtnFrames.select()

BtnQuit.grid(       row=0, column=0, sticky=tk.W, pady=4)
BtnLogStart.grid(   row=0, column=1, sticky=tk.W, pady=4)
BtnLogEnd.grid(     row=0, column=2, sticky=tk.W, pady=4)
BtnUpload.grid(     row=0, column=3, sticky=tk.W, pady=4)
BtnPlayer.grid(     row=0, column=4, sticky=tk.W, pady=4)
BtnFrames.grid(     row=0, column=5, sticky=tk.W, pady=4)

createToolTip(BtnQuit,     "Quit program")
createToolTip(BtnLogStart, "(Re)start logging")
createToolTip(BtnLogEnd,   "Close logfile & open in editor")
createToolTip(BtnUpload,   "Upload file to serial)")
createToolTip(BtnPlayer,   "Replay (log)file (publish like received from serial)")

# MQTT stuff
def on_message(client, userdata, message):
   global FrameFilterState

   if (sys.version_info > (3, 0)):
      InPayLoad = message.payload.decode('iso-8859-1')
   else:
      InPayLoad = message.payload

   if CbValueFrames.get() :
      # show all data
      FilteredPayload  = InPayLoad
      FrameFilterState = 0 # reset state
   else :
      # remote frames (+ CR/LF) from stream
      FilteredPayload = ""

      while len(InPayLoad) :
         # do not show frames
         if   FrameFilterState == 0 :
            # wait for frame start
            try :
               i = InPayLoad.index(MySlip.SLIP_START)
               FilteredPayload += InPayLoad[:i]
               InPayLoad = InPayLoad[i:]
               FrameFilterState = 1
#               print("0.1 ", i)
            except :
               FilteredPayload =  InPayLoad
               InPayLoad = ""
#               print("0.2 ")
               continue

         elif FrameFilterState == 1 :
            # wait for frame end
            try :
               i = InPayLoad.index(MySlip.SLIP_END)
               InPayLoad = InPayLoad[i+1:]
               FrameFilterState = 2
#               print("1.1 ", i)
            except :
               InPayLoad = ""
#               print("1.2 ")

         elif FrameFilterState == 2 :
            # discard CR
            InPayLoad = InPayLoad[1:]
            FrameFilterState = 3
#            print("2 ", len(InPayLoad))

         else : # FrameFilterState == 3
            # discard LF
            InPayLoad = InPayLoad[1:]
            FrameFilterState = 0
#            print("3 ", len(InPayLoad))

   if os.name == 'posix' :
      FilteredPayload = FilteredPayload.replace('\r', '')

   Memo.configure(state='normal')
   Memo.insert(tk.END, FilteredPayload)
   Memo.configure(state='disabled')
   Memo.see(tk.END) # see before configure sometimes flashes memo empty on linux...
   #print("FilteredPayload", FilteredPayload)

   if LogStart.Flag:
      LogStart.File.write(message.payload)

def on_connect(client, userdata, flags, rc):
   print("Connected with result code "+str(rc))
   mqttc.subscribe("Robotlib/ComRawRx", qos=0)

def SerialRx() :
   if  ser.inWaiting() > 0:
      line = ser.read(ser.inWaiting())
      Memo.configure(state='normal')
      Memo.insert(tk.END, line)
      Memo.see(tk.END)
      Memo.configure(state='disabled')
      if LogStart.Flag:
         LogStart.File.write(line)

   master.after(10, SerialRx) # check serial again soon

if (args.com == None) :
   print("No com port -> MQTT")

   #create an mqtt client & connect
   mypid = os.getpid()
   client_uniq = os.path.basename(__file__)+str(mypid)
   mqttc = rl.mqtt.Client(client_uniq)
   mqttc.on_message  = on_message
   mqttc.on_connect = on_connect  # handles subscribe

   mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)

   # run mqtt client in separate thread
   mqttc.loop_start()
else :
   print("Use serial port %s (MQTT disabled)" % args.com)
   import serial
   from serial.tools import list_ports
   try:
      ser = serial.Serial(args.com, 115200, timeout=0, write_timeout=0)

   except:
      print("Failed to connect serial, ports on this system:")
      ports = list_ports.comports()
      ports.sort()
      for p in ports:
         print(p)
      raise SystemExit

   ser.flushInput()

# drive GUI
LineInput.focus_force()
master.after(100, PlayerSendLines)
master.after(100, UploaderSendLines)

if (args.com != None) :
   master.after(10, SerialRx) # check serial again soon

tk.mainloop()
