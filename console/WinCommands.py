from tkinter import *
# http://www.python-course.eu/tkinter_entry_widgets.php
import tkinter.scrolledtext as tkst

import paho.mqtt.client as mqtt
import os
import time      
import json       
from RLCommon import *    # RobotLib common code

OutputLines    = []   

def Send():                
   try:
      s = T.selection_get()
   except TclError:
      # when no selection, get all data
      s = T.get("1.0",END)                      

   if (not s.endswith("\n")): 
      # add lf to end of string (will translate to cr later on)
      print("add cr\n")
      s += "\n"
      
   print("Send: [%s]\n" % (s))
   global OutputLines
   OutputLines += s.splitlines(1)   # 1 = keep line separators
   
def Save():                 
   global ConfigData
   print("Save")  
   ConfigData['CommandText'] = T.get("1.0",END)
   with open('WinCommands.json', 'w') as fp:
      json.dump(ConfigData, fp, sort_keys=True, indent=4)   
   
def Load():  
   global ConfigData
   print("Load")
   with open('WinCommands.json', 'r') as fp:
      ConfigData = json.load(fp)

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
master = Tk()     
master.wm_title(os.path.basename(__file__))

# text output window
T = tkst.ScrolledText(master, height=24, width=80, wrap="none")
T.grid(row=1, column=0, columnspan=6, sticky=(N, E, S, W))
T.mark_set("insert", "1.0") # set cursor to first line, column 0 (yeah, 1-based and 0-based...)
T.bind('<Control-Return>', CtrlEnterKey) # bind function to keystroke

# load configuration file
try:
   Load()

except:   
   # no config => load defaults and & save (template) config file
   ConfigData = {  'MqttIp'      : "127.0.0.1", 
                   'MqttPort'    : 1883,
                   'CommandText' : "\n"}
   Save()

# load commands into widget
s = ConfigData['CommandText']
s = s[:-1]  # remove last char (newline), since there is always one in the widget...
T.insert(1.0, s)  

BntSend     = Button(master, text='Send',     command=Send)
BntLoad     = Button(master, text='Load',     command=Load)
BntSave     = Button(master, text='Save',     command=Save)

BntSend.grid(row=0, column=0, sticky=W, pady=4)
BntLoad.grid(row=0, column=1, sticky=W, pady=4)
BntSave.grid(row=0, column=2, sticky=W, pady=4)

createToolTip(BntSend, "Ctrl-Enter - send selected commands to target")   
createToolTip(BntLoad, "Load commands & config")   
createToolTip(BntSave, "Save commands & config")   

# resize stuff
master.columnconfigure(2, weight = 3 )
master.rowconfigure(1, weight = 3 )

def on_connect(client, userdata, flags, rc):
   print("Connected with result code "+str(rc))
   mqttc.subscribe("Robotlib/ComRawRx", qos=0)
    
#create an mqtt client & connect
mypid = os.getpid()
client_uniq = os.path.basename(__file__)+str(mypid)
mqttc = mqtt.Client(client_uniq)
mqttc.on_connect = on_connect  # handles subscribe

mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)

# run mqtt client in separate thread
mqttc.loop_start()

# drive GUI
T.focus_force() 
master.after(100, SendOutputLines) 
mainloop( )