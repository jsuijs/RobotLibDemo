"""
==========================
Line plot of MQTT data
==========================
"""

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code

import os
import tkinter as tk
root = CreateTkRoot()

import numpy as np
import matplotlib.pyplot as plt

ScreenUpdate = 0

#https://hardsoftlucid.wordpress.com/various-stuff/realtime-plotting/

#https://matplotlib.org/api/pyplot_api.html#module-matplotlib.pyplot
# js: 111 betekent 1 x 1 grid, 1e plot (meerdere plots zijn mogelijk).
# shorthand voor 3 params: subplot(nrows, ncols, plot_number)
#ax = plt.subplot(111)
fig = plt.figure(1)           # fig is placed on canvas later
ax = fig.add_subplot(111)     # subplot is added to fig
plt.subplots_adjust(left=0.1, bottom=0.1, right=0.95, top=0.95)

#p = plt.plot(0, 20, 10, 2)  # print full range zodat plot niet herschaalt

# b    blue
# g    green
# r    red
# c    cyan
# m    magenta
# y    yellow
# k    black
# w    white

lines = ax.plot([], [], 'b-', [], [], 'r-', [], [], 'g-', [], [], 'm-', [], [], 'c-', [], [], 'y-', [], [], 'k-') # Returns a tuple of line objects, thus the comma
NrOfPoints = 0
ax.grid(True)

# Button actions
def ClickClear():
   print("Clear data")
   for MyLine in lines :
      MyLine.set_xdata([])
      MyLine.set_ydata([])
      print(MyLine)
   global ScreenUpdate, NrOfPoints
   ScreenUpdate = 1
   NrOfPoints   = 0

PauseFlag = -1
def ClickPause():
   global PauseFlag
   PauseFlag *= -1

def ClickFile():

   # get list of columns
   MyData = []
   for MyLine in lines :
      if len(MyLine.get_ydata()) > 0 :
         MyData.append(MyLine.get_ydata().tolist())

   if len(MyData) > 0:
      # there is data to export...
      FileName = tk.filedialog.asksaveasfilename(defaultextension=".txt")
      if FileName:
         print("Export to " + FileName)

         # collect rows & write them to file
         f = open(FileName, 'w')
         while len(MyData[0]) > 0:
            Row = [];
            for Col in MyData :
               Row.append(Col.pop(0))
            f.write('\t'.join(map(str,Row)) + '\n')
         f.close()
         print("Export done")
      else:
         print("Export canceled")
   else:
      print("No data to export")

def ClickClipBoard():

   # get list of columns
   MyData = []
   for MyLine in lines :
      if len(MyLine.get_ydata()) > 0 :
         MyData.append(MyLine.get_ydata().tolist())

   if len(MyData) > 0:
      # there is data to export...
      # collect rows
      Lines = ""
      while len(MyData[0]) > 0:
         Row = [];
         for Col in MyData :
            Row.append(Col.pop(0))
         Line = '\t'.join(map(str,Row)) + '\n'
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
root.wm_title(os.path.basename(__file__) + " - Data of " + args.msg)

# MQtt ------------------------------------------------------------------------
# setup & connect MQtt client to receive messages from robot
ConfigData = rl.LoadCfg()
mqttc = rl.MQttClient(ConfigData['MqttIp'])
#-----

def DataTakt():
   #print("DataTakt")

   global PauseFlag, ScreenUpdate
   if PauseFlag != 1 :
      # process messages
      while len(mqttc.MsgQueue) > 0 :
         Message = mqttc.MsgQueue.pop(0)
         fields = Message.split()             # whitespace separated
         if (fields[0] == args.msg) :
            print("msg:", len(fields), fields)

            global NrOfPoints
            #print("d ", NrOfPoints, len(lines[0].get_xdata()), len(lines[0].get_ydata()))
            MyLines = list(lines) # copy list
            for field in fields[1:] :
               # assign each value to its corresponding line. Should work with
               # any number of lines. So plot command at the top determins the max
               # number of graphs shown.
               try :
                  MyLine = MyLines.pop(0)
                  FieldValue = float(field)
                  MyLine.set_xdata(np.append(MyLine.get_xdata(), NrOfPoints))
                  MyLine.set_ydata(np.append(MyLine.get_ydata(), FieldValue))
               except:
                  pass
            NrOfPoints += 1
            ScreenUpdate = 1

   if ScreenUpdate == 1 :
      ScreenUpdate = 0
      ax.relim()
      ax.autoscale()
      canvas.draw()

   root.after(100, DataTakt)

BBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
BBar.pack(fill=tk.X, padx=5, pady=5)

tk.Button(master=BBar, text='Quit',       command=quit            ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='Clear',      command=ClickClear      ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='Pause',      command=ClickPause      ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='File',       command=ClickFile       ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='ClipBoard',  command=ClickClipBoard  ).pack(side=tk.LEFT)

canvas = AddFigToCanvas(fig)

root.after(1000, DataTakt)
tk.mainloop()
