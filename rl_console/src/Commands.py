#!/usr/bin/python3

import sys
sys.path.append('..')
import include.rl_comms as rl      # RobotLib common code
from   include.rl_gui  import *    # RobotLib common code

# http://www.python-course.eu/tkinter_entry_widgets.php
import tkinter.scrolledtext as tkst

import paho.mqtt.client as mqtt
import os
import time
import json

OutputLines    = []

def Send():
   try:
      s = T.selection_get()
   except tk.TclError:
      # when no selection, get all data
      s = T.get("1.0", tk.END)

   if (not s.endswith("\n")):
      # add lf to end of string (will translate to cr later on)
      print("add cr\n")
      s += "\n"

   print("Send: [%s]\n" % (s))
   global OutputLines
   OutputLines += s.splitlines(1)   # 1 = keep line separators

def SaveCmdData():
   global CmdData
   print("Save")
   CmdData['CommandText'] = T.get("1.0", tk.END)
   with open('WinCommands.json', 'w') as fp:
      json.dump(CmdData, fp, sort_keys=True, indent=4)

def LoadCmdData():
   global CmdData
   print("Load")
   with open('WinCommands.json', 'r') as fp:
      CmdData = json.load(fp)

   # load commands into widget
   s = CmdData['CommandText']
   s = s[:-1]  # remove last char (newline), since there is always one in the widget...
   T.delete('1.0', tk.END)
   T.insert(1.0, s)

def SendOutputLines():
   global OutputLines
   master.after(100, SendOutputLines)
   if len(OutputLines) > 0 :
      s = OutputLines.pop(0).replace("\n", "\r")
      mqttc.publish("Robotlib/ComRawTx", s.encode("cp1252", 'ignore'))
      print (":".join("{:02x}".format(ord(c)) for c in s))

def CtrlEnterKey(event):
   Send()
   return "break"    # no more event handling, so TEXT widget does not processes ctrl-enter.

#------------------------------------------------------------------------------
master = tk.Tk()
master.wm_title(os.path.basename(__file__))

# text output window
T = tkst.ScrolledText(master, height=24, width=80, wrap="none")
T.grid(row=1, column=0, columnspan=6, sticky=(tk.N, tk.E, tk.S, tk.W))
T.mark_set("insert", "1.0") # set cursor to first line, column 0 (yeah, 1-based and 0-based...)
T.bind('<Control-Return>', CtrlEnterKey) # bind function to keystroke

# load generic configuration file
ConfigData = rl.LoadCfg("C:\\MyRobot\\arm\\ConsoleShortcuts\\rl_cfg.json")

# load program-specific configuration file
try:
   LoadCmdData()

except:
   # no config => load defaults and & save (template) config file
   CmdData = {  'CommandText' : "\n"}
   SaveCmdData()

BntSend  = tk.Button(master, text='Send', command=Send)
BntLoad  = tk.Button(master, text='Load', command=LoadCmdData)
BntSave  = tk.Button(master, text='Save', command=SaveCmdData)

BntSend.grid(row=0, column=0, sticky=tk.W, pady=4)
BntLoad.grid(row=0, column=1, sticky=tk.W, pady=4)
BntSave.grid(row=0, column=2, sticky=tk.W, pady=4)

createToolTip(BntSend, "Ctrl-Enter - send selected commands to target")
createToolTip(BntLoad, "Load commands & config")
createToolTip(BntSave, "Save commands & config")

# resize stuff
master.columnconfigure(2, weight = 3 )
master.rowconfigure(1, weight = 3 )

#create an mqtt client & connect
mqttc = mqtt.Client(os.path.basename(__file__)+str(os.getpid()))
mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)

# run mqtt client in separate thread
mqttc.loop_start()

# drive GUI
T.focus_force()
master.after(100, SendOutputLines)
tk.mainloop( )