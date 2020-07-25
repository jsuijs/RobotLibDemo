#!/usr/bin/python3

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code

# http://www.python-course.eu/tkinter_entry_widgets.php
import tkinter.scrolledtext as tkst

import os
import json

OutputLines    = []

def SendCmd():
   try:
      s = T.selection_get()
   except tk.TclError:
      # when no selection, get all data
      s = T.get("1.0", tk.END)

   if (not s.endswith("\n")):
      # add lf to end of string (will translate to cr later on)
      print("add cr\n")
      s += "\n"

   print("SendCmd: [%s]\n" % (s))
   global OutputLines
   OutputLines += s.splitlines(1)   # 1 = keep line separators

def SaveCmd():
   global CmdData
   print("Save")
   CmdData['CommandText'] = T.get("1.0", tk.END)
   with open('WinCommands.json', 'w') as fp:
      json.dump(CmdData, fp, sort_keys=True, indent=4)

def LoadCmd():
   global CmdData
   print("Load")
   with open('WinCommands.json', 'r') as fp:
      CmdData = json.load(fp)

   # load commands into widget
   s = CmdData['CommandText']
   s = s[:-1]  # remove last char (newline), since there is always one in the widget...
   T.delete('1.0', tk.END)
   T.insert(1.0, s)

def ClearCmd():
   T.delete('1.0', tk.END)

def SendOutputLines():
   global OutputLines
   master.after(100, SendOutputLines)
   if len(OutputLines) > 0 :
      s = OutputLines.pop(0).replace("\n", "\r")
      Rcc.Publish(s)
      print (":".join("{:02x}".format(ord(c)) for c in s))

def CtrlEnterKey(event):
   SendCmd()
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
ConfigData = rl.LoadCfg()

# load program-specific configuration file
try:
   LoadCmd()

except:
   # no config => load defaults and & save (template) config file
   CmdData = {  'CommandText' : "\n"}
   SaveCmd()

BntSend   = tk.Button(master, text='Send',  command=SendCmd)
BntLoad   = tk.Button(master, text='Load',  command=LoadCmd)
BntSave   = tk.Button(master, text='Save',  command=SaveCmd)
BntClear  = tk.Button(master, text='Clear', command=ClearCmd)

BntSend.grid( row=0, column=0, sticky=tk.W, pady=4)
BntLoad.grid( row=0, column=1, sticky=tk.W, pady=4)
BntSave.grid( row=0, column=2, sticky=tk.W, pady=4)
BntClear.grid(row=0, column=3, sticky=tk.W, pady=4)

createToolTip(BntSend,  "Ctrl-Enter - send selected commands to target")
createToolTip(BntLoad,  "Load commands & config")
createToolTip(BntSave,  "Save commands & config")
createToolTip(BntClear, "Clear commands")

# resize stuff
master.columnconfigure(3, weight = 3 )
master.rowconfigure(1, weight = 3 )

#create an mqtt client & connect
Rcc = rl.RlCommsClient(ConfigData['RlComms'])

# drive GUI
T.focus_force()
master.after(100, SendOutputLines)
tk.mainloop( )