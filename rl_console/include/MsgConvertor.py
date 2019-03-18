#!/usr/bin/python3
# -*- coding: cp1252 -*-
# set encoding to allow 'slip' chars like
# ÁFILE  FCB aap 11À

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
import file_export as FileExport
import tkinter.scrolledtext as tkst

import os, time, sys

#------------------------------------------------------------------------------
def StatusMsg(data) :
   data = str(data).rstrip()
   print("StatusMsg:", data)
   LabelStatus['text']  = data

#------------------------------------------------------------------------------
def ClickPaste() :
   StatusMsg("ClickPaste")

   Data = root.clipboard_get().replace('Á', '').replace('À', '')
   # todo: checks - is dit een valide frame?

   TopMemo.configure(state='normal')
   TopMemo.delete(1.0, tk.END)
   TopMemo.insert(tk.END, Data)
   TopMemo.configure(state='disabled')
   TopMemo.see(tk.END) # Memo.see before Memo.configure sometimes flashes memo empty on linux...

#------------------------------------------------------------------------------
def ClickCopyTop() :
   StatusMsg("ClickCopyTop")
   # todo: add framing to msg
   Data = ""
   Lines = TopMemo.get(1.0, tk.END).splitlines()
   for L in Lines:
      if len(L) < 3 : continue
      Data += ('Á' + L + 'À\n')
   root.clipboard_clear()
   root.clipboard_append(Data)

#------------------------------------------------------------------------------
def ClickCopyBottom() :
   StatusMsg("ClickCopyBottom")
   root.clipboard_clear()
   root.clipboard_append(BottomMemo.get(1.0, tk.END))

#------------------------------------------------------------------------------
def ClickCData() :
   StatusMsg("ClickCData")

#------------------------------------------------------------------------------
def ClickCMeta() :
   StatusMsg("ClickCMeta")

#------------------------------------------------------------------------------
def MemoKey(event):
   if (event.char != '') :
      pass
      # printable character in memo field => focus to LineInput
      # (non-printable chars like control, shift, cursor movements
      #  and ctrl-c are valid on the memo window for text selection)

#------------------------------------------------------------------------------
def DataTakt():
   #print("DataTakt")

   root.after(100, DataTakt)
   # end of DataTakt

DataTakt.FileCounter = 0   # init var



#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
# start of main code
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def main(): # dummy for Ultraedit function list
   pass
# main at root level.

MessageBuffer = []

root = tk.Tk()
root.wm_title(os.path.basename(__file__))

# resize stuff
root.columnconfigure(7, weight = 3 )
root.rowconfigure(3, weight = 3 )


ConfigData = rl.LoadCfg()

# print path - implies check of config data
print("File root: ", ConfigData['FileService']['FileRoot'])

mqttc = rl.MQttClient(ConfigData['MqttIp']) # setup & connect MQtt client to receive messages from robot

#------------------------------------------------------------------------------
# Horizontal (top) bar + Conversion button & formats
HBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
HBBar.grid(row=0, column=0, columnspan=10, sticky=(tk.N, tk.E, tk.S, tk.W))

BtnCData      = tk.Button(HBBar, text='C-Data',  command=ClickCData)
BtnCData.pack(side=tk.LEFT)
createToolTip(BtnCData,"Convert data of message in top window, result in lower window")

BtnCMeta      = tk.Button(HBBar, text='C-Meta',  command=ClickCMeta)
BtnCMeta.pack(side=tk.LEFT)
createToolTip(BtnCMeta,"Convert meta-data of message in top window, result in lower window")

tk.Label(HBBar, text="  Conversion formats:").pack(side=tk.LEFT)

MetaDataFormat = tk.Entry(HBBar, width=10)
MetaDataFormat.pack(side=tk.LEFT)
MetaDataFormat.insert(0, "-")
createToolTip(MetaDataFormat,  "MetaData Conversion format (b, w, i, f, number as multiplier)")

LineDataFormat = tk.Entry(HBBar, width=10)
LineDataFormat.pack(side=tk.LEFT)
LineDataFormat.insert(0, "h")
createToolTip(LineDataFormat,  "Conversion format per line (b, w, i, f, number as multiplier)")

#------------------------------------------------------------------------------
# Vertical (right hand) bar + buttons
VBBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
VBBar.grid(row=1, column=9, rowspan=3, sticky=(tk.N, tk.E, tk.S, tk.W))

B = tk.Button(VBBar, text='Paste',   command=ClickPaste)
B.pack(side=tk.TOP)
createToolTip(B,"Paste from clipboard to top (message) window.")

B = tk.Button(VBBar, text='Copy',    command=ClickCopyTop)
B.pack(side=tk.TOP)
createToolTip(B,"Copy (encoded) message from top window to clipboard.")

B = tk.Button(VBBar, text='Copy',    command=ClickCopyBottom)
B.pack(side=tk.BOTTOM)
createToolTip(B,"Copy converted data from bottom windows to clipboard.")

#------------------------------------------------------------------------------
# Paned (resizable) window with 2 memo fields
m = tk.PanedWindow(orient=tk.VERTICAL)
m.grid(row=3, column=0, columnspan=8, sticky=(tk.N, tk.E, tk.S, tk.W))

TopMemo = tkst.ScrolledText(root, height=10, width=80, wrap="none", bg="palegreen3")
TopMemo.configure(state='disabled')
TopMemo.bind("<Key>", MemoKey)

BottomMemo = tkst.ScrolledText(root, height=10, width=80, wrap="none", bg="palegreen3")
BottomMemo.configure(state='disabled')
BottomMemo.bind("<Key>", MemoKey)

m.add(TopMemo)
m.add(BottomMemo)

#------------------------------------------------------------------------------
# bottom line
LabelStatus  = tk.Label(root, text="Startup ready.")
LabelStatus.grid(row=4, column=0,  columnspan=8, sticky=(tk.W))

#------------------------------------------------------------------------------
# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
