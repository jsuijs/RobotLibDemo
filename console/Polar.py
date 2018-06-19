"""
==========================
Polar plot of MQTT data 
==========================
"""                                                                     
# import RobotLib common code
import RLCommon 
import os                     
import tkinter as tk 
root = RLCommon.CreateTkRoot()

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
   print("Clear data")
#   for MyLine in lines :  
#      MyLine.set_xdata([])
#      MyLine.set_ydata([])       
#      print(MyLine)       
   NrOfPoints = 0      
   global ScreenUpdate
   ScreenUpdate = 1 

PauseFlag = -1
def ClickPause():
   global PauseFlag
   PauseFlag *= -1

#------------------------------------------------------------------------------    
# create parser     
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('msg', help='Message name to process')
args = parser.parse_args()
root.wm_title(os.path.basename(__file__) + " - Data of " + args.msg)

# MQtt ------------------------------------------------------------------------ 
# setup & connect MQtt client to receive messages from robot
RLCommon.MQttClient(RLCommon.on_message)
#-----

def DataTakt():
   #print("DataTakt") 
   
   global PauseFlag, ScreenUpdate
   if PauseFlag != 1 :
      # process messages     
      while len(RLCommon.MsgQueue) > 0 : 
         Message = RLCommon.MsgQueue.pop(0)
         fields = Message.split(' ')             # space separted 
         if (fields[0] == args.msg) :
            print("msg:", len(fields), fields)    

            fields.pop(0)
            while len(fields) > 1 :
               field1 = fields.pop(0)
               field2 = fields.pop(0)
               plt.scatter(int(field1) / 57.2958, int(field2), 10, 2)   # variant op plot 
       
            ScreenUpdate = 1    

   if ScreenUpdate == 1 :      
      ScreenUpdate = 0
      ax.relim() 
      ax.autoscale()
      canvas.draw() 

   RLCommon.root.after(100, DataTakt) 

BBar = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
BBar.pack(fill=tk.X, padx=5, pady=5)

tk.Button(master=BBar, text='Quit',  command=RLCommon.quit       ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='Clear', command=ClickClear  ).pack(side=tk.LEFT)
tk.Button(master=BBar, text='Pause', command=ClickPause  ).pack(side=tk.LEFT)
#tk.Button(master=BBar, text='Save'                       ).pack(side=tk.LEFT)

canvas = RLCommon.AddFigToCanvas(fig)
   
root.after(1000, DataTakt)

tk.mainloop()
