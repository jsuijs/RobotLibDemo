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
def StatusMsg(data) :
   data = str(data).rstrip()
   print("StatusMsg:", data)
   LabelStatus['text']  = data

#------------------------------------------------------------------------------
def ClickPaste() :
   ClickPasteWorker(root.clipboard_get())

def ClickPasteWorker(Data) :
   global Msg

   Msg.LoadMsg(Data)

   print(blob.HexDump(Msg.RawData))
   if Msg.MsgOk :
      MetaDataFormat.delete(0,tk.END)
      MetaDataFormat.insert(0, Msg.MetaFormat)

      LineDataFormat.delete(0,tk.END)
      LineDataFormat.insert(0, Msg.LineFormat)

      MemoLoad(TopMemo, Msg.MsgString)

      StatusMsg("Message load succes")
   else :
      StatusMsg("Error loading message: " + Msg.Error)


#------------------------------------------------------------------------------
def ClickCopyTop() :

   Data = ""
   Lines = TopMemo.get(1.0, tk.END).splitlines()
   for L in Lines:
      if len(L) < 3 : continue
      Data += ('Á' + L + 'À\n')
   root.clipboard_clear()
   root.clipboard_append(Data)

   StatusMsg("Copy (framed) message to clipboard")

#------------------------------------------------------------------------------
def ClickCopyBottom() :
   root.clipboard_clear()
   root.clipboard_append(BottomMemo.get(1.0, tk.END))
   StatusMsg("Output data copied to clipboard")

#------------------------------------------------------------------------------
def ClickCData() :

   StatusMsg("LineData converted.")   # message at top so it can be replaced by errors

   MetaLen = blob.CountFormatString(MetaDataFormat.get())
   if MetaLen < 0 :
      StatusMsg("Error: invalid MetaDataFormat.")
      return

   if LineDataFormat.get() == 'h' :
      MemoLoad(BottomMemo, blob.HexDump(Msg.RawData[MetaLen:]))
      StatusMsg("Conversion to hex done.")
      return

#   print (CountFormatString(LineDataFormat.get()))
   Format = LineDataFormat.get()
   LineLen = blob.CountFormatString(Format)
   if LineLen == -2 :
      StatusMsg("Error: invalid LineDataFormat.")
      return

   if LineLen == 0 :
      StatusMsg("Error: LineDataFormat must be non-zero.")
      return
   r = ConvertToFormat(Msg.RawData[MetaLen:], Format)
#   print(r)
   MemoLoad(BottomMemo, r)

#------------------------------------------------------------------------------
def ClickCMeta() :
   StatusMsg("MetaData converted.") # message at the top, so it can be replaced by errors

   Format = MetaDataFormat.get()
   MetaLen = blob.CountFormatString(Format)
   if MetaLen < 0 :
      StatusMsg("Error: invalid MetaDataFormat.")
      return

   r = ConvertToFormat(Msg.RawData[:MetaLen], Format)
   MemoLoad(BottomMemo, r)

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
def ConvertToFormat(RawData, Format) :
   # convert binary data to requested format

   (Error, List) = blob.ConvertToFormat(RawData, Format)

   if Error != "" :
      StatusMsg(Error)

   # convert list of lists to tab-separated multi-line string
   OutBuffer = ''
   for L in List:
      for R in L :
         OutBuffer += str(R) + '\t'

      OutBuffer = OutBuffer[:-1] + "\n" # change last tab to line end, each time the format string has been processed

   return OutBuffer

#------------------------------------------------------------------------------
def MemoLoad(_Memo, Data) :
   _Memo.configure(state='normal')
   _Memo.delete(1.0, tk.END)
   _Memo.insert(tk.END, Data)
   _Memo.configure(state='disabled')
   #_Memo.see(tk.END) # Memo.see before Memo.configure sometimes flashes memo empty on linux...

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
Paned = tk.PanedWindow(orient=tk.VERTICAL)
Paned.grid(row=3, column=0, columnspan=8, sticky=(tk.N, tk.E, tk.S, tk.W))

TopMemo = tkst.ScrolledText(root, height=10, width=80, wrap="none", bg="palegreen3")
TopMemo.configure(state='disabled')
TopMemo.bind("<Key>", MemoKey)

BottomMemo = tkst.ScrolledText(root, height=10, width=80, wrap="none", bg="palegreen3")
BottomMemo.configure(state='disabled')
BottomMemo.bind("<Key>", MemoKey)

Paned.add(TopMemo)
Paned.add(BottomMemo)

#------------------------------------------------------------------------------
# bottom line
LabelStatus  = tk.Label(root, text="Startup ready.")
LabelStatus.grid(row=4, column=0,  columnspan=8, sticky=(tk.W))

Msg = blob.BlobMsg()

ClickPasteWorker(blob.TestMsg)


#------------------------------------------------------------------------------
# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
