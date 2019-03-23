#!/usr/bin/python3
# -*- coding: cp1252 -*-
# set encoding to allow 'slip' chars like
# ÁFILE  FCB aap 11À

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
from   blob_msg import *
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
   MetaLen = CountFormatString(MetaDataFormat.get())
   if MetaLen < 0 :
      StatusMsg("Error: invalid MetaDataFormat")
      return

   if LineDataFormat.get() == 'h' :
      MemoLoad(BottomMemo, HexDump(M.RawData[MetaLen:]))
      StatusMsg("Conversion to hex done.")
      return

#   print (CountFormatString(LineDataFormat.get()))
   Format = LineDataFormat.get()
   LineLen = CountFormatString(Format)
   if LineLen == -2 :
      StatusMsg("Error: invalid LineDataFormat")
      return

   if LineLen == 0 :
      StatusMsg("Error: LineDataFormat must be non-zero")
      return

   MemoLoad(BottomMemo, ConvertToFormat(M.RawData[MetaLen:], Format))

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

#------------------------------------------------------------------------------
def HexDump(Data) :
   OutData = ""
   n = 0
   while len(Data) :
      #process in 16 byte chunks
      b = Data[:16]
      Data = Data[16:]

      s1 = " ".join([f"{i:02x}" for i in b])
      s1 = s1[0:23] + " " + s1[23:]
      s2 = "".join([chr(i) if 32 <= i <= 126 else "." for i in b])
      OutData += ((f"{n * 16:04x}  {s1:<{48}}  |{s2}|") + "\n")
      n += 1
   return OutData

#------------------------------------------------------------------------------
def ConvertToFormat(RawData, Format) :
   # convert binary data to requested format

   Format = ExpandFormatString(Format)
   OutBuffer = ''
   while len(RawData) :
      for c in Format :
         c = c.lower()

         if c == 'b' :     # byte values
            Value = RawData[0]
            RawData = RawData[1:]
            OutBuffer += str(Value) + '\t'
            continue
         if c == 'w' :     # word values
            Value = RawData[0] + 256 * RawData[1]
            RawData = RawData[2:]
            OutBuffer += str(Value) + '\t'
            continue

         print("Error: unknown format char (", c, ")")
         return
      OutBuffer = OutBuffer[:-1] + "\n" # change last tab to line end, each time the format string has been processed

   return OutBuffer

#------------------------------------------------------------------------------
def MemoLoad(_Memo, Data) :
   _Memo.configure(state='normal')
   _Memo.delete(1.0, tk.END)
   _Memo.insert(tk.END, Data)
   _Memo.configure(state='disabled')
   _Memo.see(tk.END) # Memo.see before Memo.configure sometimes flashes memo empty on linux...

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
# start of main code
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def main(): # dummy for Ultraedit function list
   pass
# main at root level.

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

M = BlobMsg(TestMsg)
#raw = MsgGetRawData(TestMsg)
print(HexDump(M.RawData))
if M.MsgOk :
   MetaDataFormat.delete(0,tk.END)
   MetaDataFormat.insert(0, M.MetaFormat)

   LineDataFormat.delete(0,tk.END)
   LineDataFormat.insert(0, M.LineFormat)

   MemoLoad(TopMemo, M.MsgString)

   StatusMsg("Message load succes")

#------------------------------------------------------------------------------
# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
