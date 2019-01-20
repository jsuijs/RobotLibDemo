#!/usr/bin/python3
# -*- coding: cp1252 -*-
# set encoding to allow 'slip' chars like
# ÁFILE  FCB aap 11À

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
import file_export as FileExport
import tkinter.scrolledtext as tkst

import os
import time
import sys
import datetime
import glob
import ntpath

MessageBuffer = []

# def LogStart():
#    if LogStart.Flag:
#       # we're logging => restart (close before open)
#       LogStart.File.close()
#
#    # actual start of logging.
#    LogStart.File = open(ConfigData['Terminal']['LogFile'],"wb")
#
#    # change (retain) logging state
#    LogStart.Flag = True
#    BtnLogStart["text"]  = "LogRestart"
#    BtnLogEnd["text"]    = "LogEnd"
# LogStart.Flag = False # on startup, we're not logging.
#
# def LogEnd():
#    if LogStart.Flag:
#       # we're logging => logend
#       # save
#       LogStart.File.close()
#
#       # Change to non-logging state
#       LogStart.Flag = False
#       BtnLogStart["text"]  = "LogStart"
#       BtnLogEnd["text"]    = "SaveAll"
#
#    else :
#       # not logging => SaveAll
#       File = open(ConfigData['Terminal']['LogFile'], "w", encoding="cp1252", errors='ignore')
#       t = Memo.get('1.0', tk.END) # line 1, position 0 => beginning of text
#       File.write(t)
#       File.close()
#
#    # launch file
#    import platform
#    if platform.system() == 'Windows' :
#       os.startfile(ConfigData['Terminal']['LogFile'])
#    else :
#       import subprocess
#       subprocess.call(["xdg-open", ConfigData['Terminal']['LogFile']])
#

def FilePath() :
   return ConfigData['FileService']['FileRoot'] + "\\" + LabelRobotName['text']

def DisableUpdate() :
   if CbValueUpdate.get() :
      MemoAdd("Disable update")
   CbValueUpdate.set(False)

def MemoAdd(data) :
   data = str(data).rstrip() + "\n"
   print("MemoAdd:", data)
   Memo.configure(state='normal')
   Memo.insert(tk.END, data)
   Memo.configure(state='disabled')
   Memo.see(tk.END) # Memo.see before Memo.configure sometimes flashes memo empty on linux...

def ClickLoad() :
   global MessageBuffer

   DisableUpdate()

   file_path = tk.filedialog.askopenfilename(initialdir=FilePath())
   if file_path != "":
      with open(file_path, encoding="cp1252", errors='ignore') as f:
          MessageBuffer = f.readlines()
      MemoAdd("Load " + file_path)
      MessageBuffer = [x.strip() for x in MessageBuffer]
      LabelCurrentFile['text'] = file_path
   #print(MessageBuffer)

def ClickSend() :
   global MessageBuffer

   DisableUpdate()

   MemoAdd("button Send\n")
   for msg in MessageBuffer:
      msg = '\xc1' + msg + '\xc0'   # add framing (but not yet escaping for binary files...)
      print(msg)
      mqttc.mqttc.publish("Robotlib/ComRawTx", msg.encode("cp1252"))
      time.sleep(0.15)  # Delays in seconds.

def ClickExport() :
   MemoAdd("button Export\n")
   FileExport.Dump(MessageBuffer)

#------------------------------------------------------------------------------
root = tk.Tk()
root.wm_title(os.path.basename(__file__))

# resize stuff
root.columnconfigure(8, weight = 3 )
root.rowconfigure(1, weight = 3 )

# MQtt ------------------------------------------------------------------------
# setup & connect MQtt client to receive messages from robot
ConfigData = rl.LoadCfg()
mqttc = rl.MQttClient(ConfigData['MqttIp'])
#-----

# nr of bytes input
tk.Label(root, text="Bytes").grid(row=0, column=4)
BytesInput = tk.Entry(root, width=3)
BytesInput.grid(row=0, column=5)
BytesInput.insert(0, 2)
createToolTip(BytesInput,  "Bytes per value (1, 2 or 4, for export)")

# nr of fields input
tk.Label(root, text="Fields").grid(row=0, column=6)
FieldsInput = tk.Entry(root, width=3)
FieldsInput.grid(row=0, column=7)
FieldsInput.insert(0, 2)
createToolTip(FieldsInput,  "Values per line (for export)")

def MemoKey(event):
   if (event.char != '') :
      pass
      # printable character in memo field => focus to LineInput
      # (non-printable chars like control, shift, cursor movements
      #  and ctrl-c are valid on the memo window for text selection)

tk.Label(root, text="Robot name: ").grid(  row=1, column=0,  columnspan=2, sticky=(tk.W))
tk.Label(root, text="Current file: ").grid(row=2, column=0,  columnspan=2, sticky=(tk.W))
LabelRobotName    = tk.Label(root, text=".")
LabelCurrentFile  = tk.Label(root, text="aap noot")
LabelRobotName.grid(    row=1, column=2,  columnspan=7, sticky=(tk.W))
LabelCurrentFile.grid(  row=2, column=2,  columnspan=7, sticky=(tk.W))

# text output window
Memo = tkst.ScrolledText(root, height=24, width=80, wrap="none")
Memo.grid(row=3, column=0, columnspan=9, sticky=(tk.N, tk.E, tk.S, tk.W))
Memo.configure(state='disabled')
Memo.bind("<Key>", MemoKey)

# buttons
CbValueUpdate  = tk.IntVar()
BtnUpdate      = tk.Checkbutton(root, text='Update', variable=CbValueUpdate, relief='raised')
BtnLoad        = tk.Button(root, text='Load',    command=ClickLoad)
BtnSend        = tk.Button(root, text='Send',    command=ClickSend)
BtnExport      = tk.Button(root, text='Export',  command=ClickExport)
BtnUpdate.select()

BtnUpdate.grid(row=0, column=0, sticky=tk.W, pady=4)
BtnLoad.grid(  row=0, column=1, sticky=tk.W, pady=4)
BtnSend.grid(  row=0, column=2, sticky=tk.W, pady=4)
BtnExport.grid(row=0, column=3, sticky=tk.W, pady=4)

createToolTip(BtnUpdate,"Update memory with received frames")
createToolTip(BtnLoad,  "Load file from disk into memory")
createToolTip(BtnSend,  "Send file from memory to robot")
createToolTip(BtnExport,"Export (convert) file from memory to disk")


#------------------------------------------------------------------------------

def SaveMsgToFile(FileRoot, FileName, FileData) :

   FileName = FileName.lower()
   Full1 = FileRoot + "\\" + FileName + ".mfsm"    # MicroFileSystem Message
   FileList = glob.glob(Full1)
   #print(Full1, FileList)

   if FileList != []:
      # file exists -> backup first
      print("Backup old file")
      Full2 = FileRoot + "\\" + FileName + "_???.bak"    # Backup, with underscore and 3 digit number
      FileList = glob.glob(Full2)
      #print(Full2, FileList)

      BnList = [] # BaseName of all files (no path)
      for fn in FileList:
         bn = ntpath.basename(fn.lower())
         BnList.append(bn)
         #print("## ", fn, "-", bn)

      #print(BnList)

      # get first unused versioned name
      Version =  1
      while True:
         NewName = "{0}_{1:0=3d}.bak".format(FileName, Version)
         if not NewName in BnList :
            break;
         Version += 1

      NewName = FileRoot + "\\" + NewName    # MicroFileSystem Backup, with underscore and 3 digit number
      #print(Full1, NewName)
      os.rename(Full1, NewName)
      MemoAdd("Old file renamed to '" + NewName + "'.")

   print("Save")

   try:
      fh = open(Full1,"w")
   except:
      MemoAdd("Error opening file '" + Full1 + "' for write.")
      MemoAdd(sys.exc_info()[1])
      return FileName   # return short filename if save failed

   for l in FileData :
      fh.write(l + '\n')
   fh.close()
   MemoAdd("File '" + Full1 + "' saved.")
   return Full1   # return full path + filename of file saved
#------------------------------------------------------------------------------


def DataTakt():
   #print("DataTakt")

#   global PauseFlag, ScreenUpdate

   # process messages
   while len(mqttc.MsgQueue) > 0 :
      Message = mqttc.MsgQueue.pop(0)
      fields = Message.split()             # whitespace separated
      if fields[0] == "STAMP" :
         print("msg:", len(fields), fields)
         LabelRobotName['text'] = fields[1]
         if fields[2] == "2017-01-01" :
            # clockset  dd mm yyyy hh mm ss - set clock date & time
            Data = datetime.datetime.now().strftime("clockset %d %m %Y %H %M %S\r")
            MemoAdd("Send time: " + Data)
            mqttc.mqttc.publish("Robotlib/ComRawTx", Data)
         continue

      if fields[0] == "FILE" :
         if fields[1] == "FCB" :
            MemoAdd("*****************************************")
            MemoAdd("Backup file '" + fields[2] + "' (" + fields[3] + " blocks)")
            if DataTakt.FileCounter != 0 :
               MemoAdd("Error: we were still working on a file, now aborted.")
            DataTakt.FileName    = fields[2]
            DataTakt.FileCounter = int(fields[3])
            DataTakt.FileBuffer  = [Message]
            continue

         if fields[1] == "IMG" :
            MemoAdd("*****************************************")
            MemoAdd("Backup Image (" + fields[2] + " blocks)")
            if DataTakt.FileCounter != 0 :
               MemoAdd("Error: we were still working on a file, now aborted.")
            DataTakt.FileName    = "disk-image"
            DataTakt.FileCounter = int(fields[2])
            DataTakt.FileBuffer  = [Message]
            continue

         if fields[1] == "BLK" :
            print("File BLK msg", DataTakt.FileCounter)
            if DataTakt.FileCounter == 0 :
               MemoAdd("Error: unexpected BLK file, ignored.")
            else :
               DataTakt.FileBuffer.append(Message)
               DataTakt.FileCounter -= 1
               if DataTakt.FileCounter == 0 :
                  MemoAdd("File received...")
                  #for l in DataTakt.FileBuffer:
                  #   print(l)
                  Name = SaveMsgToFile(FilePath(), DataTakt.FileName, DataTakt.FileBuffer)
                  if CbValueUpdate.get() :
                     # save copy of file to global buffer (for export, send etc).
                     MessageBuffer = DataTakt.FileBuffer
                     LabelCurrentFile['text'] = Name
            # end of BLK processing
            continue

         MemoAdd("Invalid FILE message subtype: " + fields[1])
         continue

#            global NrOfPoints
#            #print("d ", NrOfPoints, len(lines[0].get_xdata()), len(lines[0].get_ydata()))
#            MyLines = list(lines) # copy list
#            for field in fields[1:] :
#               # assign each value to its corresponding line. Should work with
#               # any number of lines. So plot command at the top determins the max
#               # number of graphs shown.
#               try :
#                  MyLine = MyLines.pop(0)
#                  FieldValue = float(field)
#                  MyLine.set_xdata(np.append(MyLine.get_xdata(), NrOfPoints))
#                  MyLine.set_ydata(np.append(MyLine.get_ydata(), FieldValue))
#               except:
#                  pass

   root.after(100, DataTakt)
   # end of DataTakt
DataTakt.FileCounter = 0


# drive GUI
root.after(1000, DataTakt)
tk.mainloop()
