#!/usr/bin/python3

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code

import tkinter.scrolledtext as tkst

import os
import time
import datetime
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
   # save log (current, all) using default filename (from config file)
   LogEndWorker(ConfigData['Terminal']['LogFile'])

def LogEndRightClick(event):
   # save log (current, all) and let user picks filename
   # default filename contains data and time prefix.
   # (do not use filename from config file, since that might
   #  contain path thus breaking the stamp prefixing)
   Filename = datetime.datetime.now().strftime("%Y%m%d %H.%M Logfile.txt")
   Filename = tk.filedialog.asksaveasfilename(title = "Select file for save",filetypes = (("log files","*.txt"),("all files","*.*")), initialfile=Filename)
   LogEndWorker(Filename)

def LogEndWorker(Filename):
   if LogStart.Flag:
      # we're logging => logend
      LogStart.File.close()

      if ConfigData['Terminal']['LogFile'] != Filename :
         # we have been logging with the default filename, so need to
         # rename/move this file to the selected filename.
         print("Rename ", ConfigData['Terminal']['LogFile'], "to", Filename)
         try:
             os.remove(Filename)
         except OSError:
             pass
         os.rename(ConfigData['Terminal']['LogFile'], Filename)

      # Change to non-logging state
      LogStart.Flag = False
      BtnLogStart["text"]  = "LogStart"
      BtnLogEnd["text"]    = "SaveAll"

   else :
      # not logging => SaveAll
      File = open(Filename, "w", encoding="cp1252", errors='ignore')
      t = Memo.get('1.0', tk.END) # line 1, position 0 => beginning of text
      File.write(t)
      File.close()

   # launch file
   import platform
   if platform.system() == 'Windows' :
      print("File to open: ", Filename)
      os.startfile(Filename)
   else :
      import subprocess
      subprocess.call(["xdg-open", Filename])

def Uploader():
   global UploaderLines
   global BtnAbort

   file_path = tk.filedialog.askopenfilename(title = "Select file to upload")
   if file_path != "":
      with open(file_path, encoding="cp1252", errors='ignore') as f:
          UploaderLines += f.readlines()

   BtnAbort['state'] = 'normal'

def Player():
   global PlayerLines
   global BtnAbort

   file_path = tk.filedialog.askopenfilename(title = "Select file to play")
   if file_path != "":
      with open(file_path, encoding="cp1252", errors='ignore') as f:
          PlayerLines += f.readlines()

   BtnAbort['state'] = 'normal'

def Abort():
   global PlayerLines
   global UploaderLines

   PlayerLines = []
   UploaderLines = []

def UpdateAbortState() :
   global PlayerLines
   global UploaderLines
   global BtnAbort

   if len(PlayerLines)   > 0 : return
   if len(UploaderLines) > 0 : return

   BtnAbort['state'] = 'disabled'


def UploaderSendLines():
   global UploaderCount
   master.after(ConfigData['Terminal']['UploaderDelay'], UploaderSendLines)
   if len(UploaderLines) > 0 :
      Data = UploaderLines.pop(0).encode("cp1252", 'ignore')
      Data = Data.replace(b'\n', b'\r')   # crlf on windows translates to \n, but robotlib expects \r (enter)
      if args.com == None :
         # no com-port => mqtt
         Rcc.Publish("Robotlib/ComRawTx", Data)
      else :
         ser.write(Data)
      UploaderCount += 1
      BtnUpload["text"]="Upload " + str(UploaderCount)
   else:
      UploaderCount = 0
      BtnUpload["text"]="Upload"

   UpdateAbortState()

def PlayerSendLines():
   global PlayerCount
   master.after(ConfigData['Terminal']['PlayerDelay'], PlayerSendLines)
   if len(PlayerLines) > 0 :
      Data = PlayerLines.pop(0).encode("cp1252", 'ignore')
      if args.com == None :
         # no com-port => mqtt
         Rcc.Publish("Robotlib/ComRawRx", Data)
      else :
         ser.write(Data)
      PlayerCount += 1
      BtnPlayer["text"]="Player " + str(PlayerCount)
   else:
      PlayerCount = 0
      BtnPlayer["text"]="Player"

   UpdateAbortState()

def LineInputEnterKey(event):
   Data = (LineInput.get() + "\r").encode("cp1252", 'ignore')
   if args.com == None :
      Rcc.Publish("Robotlib/ComRawTx", Data)
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
Memo.bind("<Key>", MemoKey)

# buttons
CbValueFrames  = tk.IntVar()
BtnQuit     = tk.Button(master, text='Quit',     command=master.quit)
BtnLogStart = tk.Button(master, text='LogStart', command=LogStart)   # LogStart doubles as LogRestart
BtnLogEnd   = tk.Button(master, text='SaveAll',  command=LogEnd)     # LogEnd doubles as SaveAll
BtnUpload   = tk.Button(master, text='Upload',   command=Uploader)
BtnPlayer   = tk.Button(master, text='Player',   command=Player)
BtnAbort    = tk.Button(master, text='Abort',    command=Abort)
BtnFrames   = tk.Checkbutton(master, text='Frames', variable=CbValueFrames, relief='raised')
BtnFrames.select()

BtnLogEnd.bind("<Button-3>", LogEndRightClick)

Memo['state']='disabled'
BtnAbort['state'] = 'disabled'

BtnQuit.grid(       row=0, column=0, sticky=tk.W, pady=4)
BtnLogStart.grid(   row=0, column=1, sticky=tk.W, pady=4)
BtnLogEnd.grid(     row=0, column=2, sticky=tk.W, pady=4)
BtnUpload.grid(     row=0, column=3, sticky=tk.W, pady=4)
BtnPlayer.grid(     row=0, column=4, sticky=tk.W, pady=4)
BtnAbort.grid(      row=0, column=5, sticky=tk.W, pady=4)
BtnFrames.grid(     row=0, column=6, sticky=tk.W, pady=4)

createToolTip(BtnQuit,     "Quit program")
createToolTip(BtnLogStart, "(Re)start logging")
createToolTip(BtnLogEnd,   "Close logfile & open in editor, right click for (stamped) filename selection")
createToolTip(BtnUpload,   "Upload file to serial)")
createToolTip(BtnPlayer,   "Replay (log)file (publish like received from serial)")
createToolTip(BtnAbort,    "Abort Upload and Replay")

def RxMessage(InPayLoad) :
   global FrameFilterState

   print(InPayLoad)

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
               FilteredPayload += InPayLoad
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
   Memo.see(tk.END) # Memo.see before Memo.configure sometimes flashes memo empty on linux...
   #print("FilteredPayload", FilteredPayload)

   if LogStart.Flag:
      LogStart.File.write(message.payload)

def RxTakt() :

   if (args.com != None) :

      if  ser.inWaiting() > 0:
         line = ser.read(ser.inWaiting())
         RxMessage(line)

   else :
      while len(Rcc.MsgQueue) :
         Message = Rcc.MsgQueue.pop(0)
         RxMessage(Message)

   master.after(10, RxTakt) # check again soon

if (args.com == None) :
   print("No com port -> MQTT")

   Rcc = rl.RlCommsClient(ConfigData['RlComms'])
   Rcc.Raw() # we want raw data, not frames

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

master.after(10, RxTakt) # check  again soon

tk.mainloop()
