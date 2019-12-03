"""
==========================
Polar plot of MQTT data
==========================
"""

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
import blob_msg as blob

import os
import tkinter as tk
root = CreateTkRoot()

import numpy as np
import matplotlib.pyplot as plt

ScreenUpdate = 0

# Compute areas and colors
#N = 150
#r = 2 * np.random.rand(N)
#theta = 2 * np.pi * np.random.rand(N)
#area = 200 * r**2
#colors = theta

#https://matplotlib.org/api/pyplot_api.html#module-matplotlib.pyplot
# js: 111 betekent 1 x 1 grid, 1e plot (meerdere plots zijn mogelijk).
# shorthand voor 3 params: subplot(nrows, ncols, plot_number)
#ax = plt.subplot(111)
fig = plt.figure(1)                             # fig is placed on canvas later
ax = fig.add_subplot(111, projection='polar')   # subplot is added to fig
#plt.subplots_adjust(left=0.1, bottom=0.1, right=0.95, top=0.95)

# s => grootte
# cmap = kleurvolgorde
# alpha = transparantie
#c = ax.scatter(theta, r, c=colors, s=area, cmap='hsv', alpha=0.75)
#c = ax.scatter(theta, r)

#plt.scatter(0, 0, 10, 2)   # nulpunt printen niet nodig...
p = plt.scatter(0, 20, 10, 2)  # print full range zodat plot niet herschaalt
#ax.set_rlabel_position(-22.5)  # get radial labels away from plotted line
ax.grid(True)

# Button actions
def ClickClear():
   global NrOfPoints
   ax.lines = []
   NrOfPoints = 0
   global ScreenUpdate
   ScreenUpdate = 1

PauseFlag = 1
def ClickPause():
   global PauseFlag
   PauseFlag *= -1
   if PauseFlag == 1 :
      root.wm_title(os.path.basename(__file__) + " - Data of " + args.msg + " (Paused)")
   else :
      root.wm_title(os.path.basename(__file__) + " - Data of " + args.msg)

def ClickFile():
   if len(ax.lines) > 0 :
      FileName = tk.filedialog.asksaveasfilename(defaultextension=".txt")
      if FileName:
         print("Export to " + FileName)
         MyData = ax.lines # get list of 2d datapoint lists (with one point each)
         Lines = ""
         # collect rows & write them to file
         f = open(FileName, 'w')
         for point in MyData :
            x = point.get_xdata(orig=True)[0]   # get the only element from the list
            y = point.get_ydata(orig=True)[0]
            Line = str(x) + '\t' + str(y) + '\n'
            Lines += Line
            f.write(Line)
         f.close()
         print("Export done")
      else:
         print("Export canceled")
   else:
      print("No data to export")

def ClickClipBoard():
   if len(ax.lines) > 0 :
      MyData = ax.lines # get list of 2d datapoint lists (with one point each)
      Lines = ""
      # collect rows
      for point in MyData :
         x = point.get_xdata(orig=True)[0]   # get the only element from the list
         y = point.get_ydata(orig=True)[0]
         Line = str(x) + '\t' + str(y) + '\n'
         Lines += Line
      root.clipboard_clear()
      root.clipboard_append(Lines)
      print("Export to ClipBoard done")
   else:
      print("No data for ClipBoard")

#------------------------------------------------------------------------------
# create parser
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('msg', help='Message name to process')
args = parser.parse_args()
ClickPause()   # sets windows title

# MQtt ------------------------------------------------------------------------
# setup & connect MQtt client to receive messages from robot

ConfigData = rl.LoadCfg()
mqttc = rl.MQttClient(ConfigData['MqttIp'])
#-----
BlobMsg = blob.BlobMsg()

DotColor = 'blue'

def DataTakt():
   #print("DataTakt")
   global PauseFlag, ScreenUpdate, BlobString, DotColor

   if PauseFlag != 1 :
      # process messages
      while len(mqttc.MsgQueue) > 0 :
         Message = mqttc.MsgQueue.pop(0)

         if BlobMsg.add(Message) :
            print(BlobMsg.MsgString)

         fields = Message.split()             # whitespace separated

         if fields[0] == args.msg :
            print("msg:", len(fields), fields)

            fields.pop(0)

            if fields[0] == "CLEAR" :
               ClickClear()

            if fields[0] == "COLOR" :
               DotColor = fields[1]

            if fields[0] == "VPAIRS" :
               fields.pop(0)
               try :
                  while len(fields) > 1 :
                     field1 = int(fields.pop(0))
                     field2 = int(fields.pop(0))

                     plt.plot(field1 / 57.2958, field2,  'o', color=DotColor)   # variant op plot
                     #print(field1 / 57.2958, field2, 10, 2)   # variant op plot

                  ScreenUpdate = 1

               except Exception as ex:
                  # try/except prevents the monitor to block on a corrupted message...
                  message = "== An exception of type {0} occurred. ==".format(type(ex).__name__)
                  print(message)
                  import traceback
                  print(traceback.format_exc())

   if ScreenUpdate == 1 :
      ScreenUpdate = 0
      ax.relim()
      ax.autoscale()
      # override Autoscale when Range is a valid float, larger than 0
      try:
         R = float(Range.get())
         if R > 0 :
            ax.set_ylim(0, R)
      except :
         pass
      canvas.draw()
      print("Update screen")

   root.after(100, DataTakt)

BBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
BBar.pack(fill=tk.X, padx=5, pady=5)

tk.Button(master=BBar, text='Clear',      command=ClickClear      ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='Pause',      command=ClickPause      ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='File',       command=ClickFile       ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='ClipBoard',  command=ClickClipBoard  ).pack(side=tk.LEFT)

def RangeCallback(*args):
   global ScreenUpdate
   ScreenUpdate = 1

# Range uses tkinter var to enable callback on change
tk.Label(master=BBar, text="Range").pack(side=tk.LEFT)
RangeVar = tk.StringVar()
RangeVar.trace('w', RangeCallback)
Range = tk.Entry(master=BBar, width=8, textvariable=RangeVar)
Range.pack(side=tk.LEFT)
createToolTip(Range,  "Max Y value")

canvas = AddFigToCanvas(fig)

root.after(1000, DataTakt)

tk.mainloop()
