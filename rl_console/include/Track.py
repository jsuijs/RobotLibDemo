#!/usr/bin/python3

import tkinter as tk
import random
import math

import os
import time

import rl_comms as rl      # RobotLib common code
from   rl_gui  import *    # RobotLib common code
import blob_msg as blob


class MyScreen(tk.Frame):
   RobotPoints = (
      (-19,118),
      (-56, 106), (0, -118), (-56, 106), (0, 0), (-56, 106),
      (-87,82), (-109,  50),  (-88,  50), (-88, -50), (-109, -50),
      (-87,-82), (-56,-106), (-19,-118), (19, -118), (56, -106), (87, -82),
      (109, -50),  (88, -50),  (88,  50), (109,  50), (87,82),
      (56,106), (0, -118), (56,106), (0, 0), (56,106),
      (19,118)
   )
   ScaleFactor  = 1.0   # current scale (will be changed on starup, based on WinScale param)
   TrackOriginX = 100
   TrackOriginY = 2500
   PointTag     = ['Trail', 'Mark', 'Group1', 'Group2', 'Group3']
   PointColor   = ['blue',  'red',  'green',  'red', 'orange']
   TrailPoints  = dict()
   t1 = 0
   t2 = 0

   def __init__(self, master):
      tk.Frame.__init__(self, master)

      # create objects
      self.canvas = tk.Canvas(master,
         width=ConfigData['Track']['WinSizeX']*ConfigData['Track']['WinScale'],
         height=ConfigData['Track']['WinSizeY']*ConfigData['Track']['WinScale'],
         background="bisque")
      self.xsb = tk.Scrollbar(master, orient="horizontal", command=self.canvas.xview)
      self.ysb = tk.Scrollbar(master, orient="vertical", command=self.canvas.yview)
      self.StatusBar = tk.Label(master, text="[0 - 0]", relief=tk.SUNKEN)

      self.BtnClear = tk.Button(master, text="Clear", width=16, command=self.CmdClear)

      self.CbValuePointGroups  = [tk.IntVar(), tk.IntVar(), tk.IntVar(), tk.IntVar(), tk.IntVar()] # note: these are Tk vars, not Python ones
      # list ordered in 'Nr' order of AddPoint
      self.CbTrail  = tk.Checkbutton(master, text="Trail   ",   variable=self.CbValuePointGroups[0], command=self.CmdTrail)
      self.CbMarks  = tk.Checkbutton(master, text="Marks   ",   variable=self.CbValuePointGroups[1], command=self.CmdMarks)
      self.CbGroup1 = tk.Checkbutton(master, text="Group 1   ", variable=self.CbValuePointGroups[2], command=self.CmdGroup1)
      self.CbGroup2 = tk.Checkbutton(master, text="Group 2   ", variable=self.CbValuePointGroups[3], command=self.CmdGroup2)
      self.CbGroup3 = tk.Checkbutton(master, text="Group 3   ", variable=self.CbValuePointGroups[4], command=self.CmdGroup3)
      self.CbTrail.select()
      self.CbMarks.select()
      self.CbGroup1.select()
      self.CbGroup2.select()
      self.CbGroup3.select()

      self.canvas.configure(yscrollcommand=self.ysb.set, xscrollcommand=self.xsb.set)
      self.canvas.configure(scrollregion=(-90000,-90000,90000,90000))

      # place objects on grid
      self.xsb.grid(       row=2, column=0, columnspan = 7, sticky="ew")
      self.ysb.grid(       row=1, column=7, sticky="ns")
      self.canvas.grid(    row=1, column=0, columnspan=7, sticky="nsew")
      self.BtnClear.grid(  row=0, column=0)
      self.CbTrail.grid(   row=0, column=1)
      self.CbGroup1.grid(  row=0, column=2)
      self.CbGroup2.grid(  row=0, column=3)
      self.CbGroup3.grid(  row=0, column=4)
      self.CbMarks.grid(   row=0, column=5)
      self.StatusBar.grid( row=3, column=0, columnspan=7, sticky="nsew")

      master.columnconfigure(6, weight = 3 )
      master.rowconfigure(1, weight = 3 )

      # Draw Roborama track
      self.canvas.create_line(0    + self.TrackOriginX, self.TrackOriginY - 0, 3600    + self.TrackOriginX, self.TrackOriginY - 0   )
      self.canvas.create_line(0    + self.TrackOriginX, self.TrackOriginY - 0, 0       + self.TrackOriginX, self.TrackOriginY - 1200)
      self.canvas.create_line(3600 + self.TrackOriginX, self.TrackOriginY - 0, 3600    + self.TrackOriginX, self.TrackOriginY - 1200)
      self.canvas.create_line(0    + self.TrackOriginX, self.TrackOriginY - 1200, 1200 + self.TrackOriginX, self.TrackOriginY - 1200)
      self.canvas.create_line(2400 + self.TrackOriginX, self.TrackOriginY - 1200, 3600 + self.TrackOriginX, self.TrackOriginY - 1200)
      self.canvas.create_line(1200 + self.TrackOriginX, self.TrackOriginY - 1200, 1200 + self.TrackOriginX, self.TrackOriginY - 2400)
      self.canvas.create_line(2400 + self.TrackOriginX, self.TrackOriginY - 1200, 2400 + self.TrackOriginX, self.TrackOriginY - 2400)
      self.canvas.create_line(1200 + self.TrackOriginX, self.TrackOriginY - 2400, 2400 + self.TrackOriginX, self.TrackOriginY - 2400)

      # Draw robot
      self.RobotPolygon = self.canvas.create_polygon(self.RobotPoints, fill="", outline="black",  tags = "Robot")
      self.RobotPosition(0, 0, 0)

      # scale back...
      self.canvas.scale("all", 0, 0, ConfigData['Track']['WinScale'], ConfigData['Track']['WinScale'])
      self.ScaleFactor *= ConfigData['Track']['WinScale']

      # This is what enables using the mouse:
      self.canvas.bind("<ButtonPress-1>", self.move_start)
      self.canvas.bind("<B1-Motion>", self.move_move)
      self.canvas.bind("<MouseWheel>",self.zoomer)

   def CmdClear(self):
      print("Clear")
      for tag in self.PointTag:
         self.canvas.delete(tag)
      self.TrailPoints = dict()

   def CmdTrail(self):
      print("Trail")
      if self.CbValuePointGroups[0].get() :  # note: Tk var, not Python one
         self.canvas.itemconfig('Trail', state='normal')
      else :
         self.canvas.itemconfig('Trail', state='hidden')

   def CmdMarks(self):
      print("Marks")
      if self.CbValuePointGroups[1].get() :  # note: Tk var, not Python one
         self.canvas.itemconfig('Mark', state='normal')
      else :
         self.canvas.itemconfig('Mark', state='hidden')

   def CmdGroup1(self):
      print("Group1")
      if self.CbValuePointGroups[2].get() :  # note: Tk var, not Python one
         self.canvas.itemconfig('Group1', state='normal')
      else :
         self.canvas.itemconfig('Group1', state='hidden')

   def CmdGroup2(self):
      print("Group2")
      if self.CbValuePointGroups[3].get() :  # note: Tk var, not Python one
         self.canvas.itemconfig('Group2', state='normal')
      else :
         self.canvas.itemconfig('Group2', state='hidden')

   def CmdGroup3(self):
      print("Group3")
      if self.CbValuePointGroups[4].get() :  # note: Tk var, not Python one
         self.canvas.itemconfig('Group3', state='normal')
      else :
         self.canvas.itemconfig('Group3', state='hidden')

   #move
   def move_start(self, event):
      self.canvas.scan_mark(event.x, event.y)
      #print("move_start", event.x, event.y, "\n")

   def move_move(self, event):
      self.canvas.scan_dragto(event.x, event.y, gain=1)
      #print("move_move", event.x, event.y)

   #windows zoom
   def zoomer(self,event):
      if (event.delta > 0):
         factor = 1.1
      elif (event.delta < 0):
         factor = 1/1.1

      StartTijd = time.clock()

      # get currrent cursor position (both in pixel and canvas coordinates)
      abs_coord_x = master.winfo_pointerx() - master.winfo_rootx()
      abs_coord_y = master.winfo_pointery() - master.winfo_rooty()
      crx = self.canvas.canvasx(abs_coord_x) / self.ScaleFactor - self.TrackOriginX
      cry = self.TrackOriginY - self.canvas.canvasy(abs_coord_y) / self.ScaleFactor

      # Actualy zoom:
      self.canvas.scale("all", 0, 0, factor, factor)  # offset other than 0,0 moves coordinates
                                                      # of objects relative to origin
      self.ScaleFactor *= factor

      # calculate new pixel position for same canvas coordinates.
      #
      # note 1 : sorry for the loops but there are just too many things
      #          to worry about when this is to be converted to a formula.
      #          (maybe it would be easier to use the mouse location as a
      #           scale origin and move all objects on the canvas afterwards?)
      # note 2 : at maximum zoom, the view shifts towards the origin (left top)
      #          and zooming out again does not correct for this...
      #          Extend the canvas 'scrollregion' if you need more resolution on
      #          large distance from the origin (now about 100mm full screen
      #          at x=3600 / track end).
      #
      NewX = abs_coord_x
      NewY = abs_coord_y
      while True:
         Mx = self.canvas.canvasx(NewX) / self.ScaleFactor - self.TrackOriginX
         if (Mx < crx) :
            NewX += 1
            continue
         break;
      while True:
         Mx = self.canvas.canvasx(NewX) / self.ScaleFactor - self.TrackOriginX
         if (Mx > crx) :
            NewX -= 1
            continue
         break;
      while True:
         My = self.TrackOriginY - self.canvas.canvasy(NewY) / self.ScaleFactor
         if (My > cry) :
            NewY += 1
            continue
         break;
      while True:
         My = self.TrackOriginY - self.canvas.canvasy(NewY) / self.ScaleFactor
         if (My < cry) :
            NewY -= 1
            continue
         break;

      # the actual move of the canvas
      #print("move from", NewX, NewY, "to", abs_coord_x, abs_coord_y)
      self.canvas.scan_mark(NewX, NewY)
      self.canvas.scan_dragto(abs_coord_x, abs_coord_y, gain=1)

      print("tijd (ms) : ", 1000 * (time.clock() - StartTijd))

   # place the robot
   def RobotPosition(self, InX, InY, Angle):
      #print("Angle", Angle)
      InX += self.TrackOriginX
      InY = self.TrackOriginY - InY
      cplx = complex(math.sin(Angle) * self.ScaleFactor, math.cos(Angle) * self.ScaleFactor)
      newxy = []
      for x, y in self.RobotPoints:
         v = cplx * complex(x, y)
         newxy.append(v.real + InX * self.ScaleFactor)
         newxy.append(v.imag + InY * self.ScaleFactor)
      self.canvas.coords(self.RobotPolygon, *newxy)

   # erase point in group NR
   def ClearPoints(self, Nr):
      if Nr < 0 or Nr > 4:
         printf("AddPoint - Illegal point nr", Nr)
         return;

      self.canvas.delete(self.PointTag[Nr])

   # add point (or MARK) on location x,y to group NR
   def AddPoint(self, Nr, x, y):
      print("AddPoint", Nr, x, y)
      if Nr < 0 or Nr > 4:
         print("AddPoint - Illegal point list nr", Nr, "(point param ", Nr-1, ")")
         return;

      if Nr == 0 :
         # Trail point => ignore points at same location
         # to postpone performance degradation.
         Key = "%d_%d" % (x,y) # generate key for dictionary
         if Key in self.TrailPoints:
            # point already on canvas, abort.
            #self.t1 += 1;
            return;
         self.TrailPoints[Key] = 1; # add point to dictionary
         #self.t2 += 1;
         #print("Filter key: %s, %d of %d points written" % (Key, self.t2, self.t1 + self.t2))

      # convert from mm to canvas coordinates
      x = (x + self.TrackOriginX) * self.ScaleFactor
      y = (self.TrackOriginY - y) * self.ScaleFactor
      #print(x, y, self.TrackOriginY, self.TrackOriginX)

      if Nr == 1:
         # mark / cross
         size = 50 # +/- mm from center in each direction
         self.canvas.create_line(x-size*self.ScaleFactor, y, x+size*self.ScaleFactor, y, fill=self.PointColor[Nr], tag=self.PointTag[Nr])
         self.canvas.create_line(x, y-size*self.ScaleFactor, x, y+size*self.ScaleFactor, fill=self.PointColor[Nr], tag=self.PointTag[Nr])
      else :
         # dot
         self.canvas.create_oval(x-1*self.ScaleFactor , y-1*self.ScaleFactor, x+1*self.ScaleFactor, y+1*self.ScaleFactor, outline=self.PointColor[Nr], fill=self.PointColor[Nr], tag=self.PointTag[Nr])

      if not self.CbValuePointGroups[Nr].get() :  # note: CbValuePointGroups are Tk vars, not Python ones
         print("Hide item", Nr)
         self.canvas.itemconfig(self.PointTag[Nr], state='hidden')
      else :
         print("show item", Nr)

   # -------------------------------------------------------------
   # PointMessage -
   # -------------------------------------------------------------
   # Process POINTS messages => show position data on track.
   #
   # Points groups numbers:
   #   -1: Trail  - not intended for normal points, only with Erase
   #    0: Marks; - crossheads instead of points
   #    1: Group1
   #    2: Group2
   #    3: Group3
   # Note: internal representation is n+1 (so trail == 0)
   #
   # Commands:
   #  POINT Erase <groups>       - erase all points in <list>
   #
   #  POINT <groups> [<x> <y>]*  - add point(s) to <list>
   #  POINT <groups> => this is valid, but can be ignored
   #                    (no points provided)
   #  Note: comma's are treated as spaces. Use comma as separator
   #        between x and y to improve human readibilty.
   # -------------------------------------------------------------
   def PointMessage(self, InString):

      print("In: ", InString)

      # remove (for our purpose) irrelevant chars
      InString = InString.replace(",", " ")     # allow for comma separate x,y
      InString = InString.replace(" +", " ")    # remove multiple spaces
      fields   = InString.split()               # whitespace separted

      try:
         if fields[1] == "Erase" :
            self.ClearPoints(int(fields[2])+1)
            return

         # first word not a known keyword => assume point-list
         fields.pop(0)  # POINTS
         GroupNr = int(fields.pop(0))+1

         while len(fields) > 1 :
            # process XY pairs
            x = int(fields.pop(0))
            y = int(fields.pop(0))
            self.AddPoint(GroupNr, x, y)
         return

      except Exception as ex:
         # ignore errors like missing or non-numeric values for int()
         template = "== An exception of type {0} occurred. =="
         message = template.format(type(ex).__name__)
         print(message)
         import traceback
         print(traceback.format_exc())
         print("Processing of line failed, no further processing of the line.")

# Keep function UpdateStatusBar here (not in class) to allow maximal flexibility on what we want to show.
def UpdateStatusBar():

   master.after(100, UpdateStatusBar)  # update again in 100 ms

   # Check if mouse is on window
   x, y = master.winfo_pointerx(), master.winfo_pointery()
   x_r, y_r = master.winfo_x(), master.winfo_y()
   if x > x_r + master.winfo_width() or x < x_r or y > y_r + master.winfo_height() or y < y_r:
      return # no -> do not update status bar

   abs_coord_x = master.winfo_pointerx() - master.winfo_rootx() - Screen.canvas.winfo_x()
   abs_coord_y = master.winfo_pointery() - master.winfo_rooty() - Screen.canvas.winfo_y()
   crx = Screen.canvas.canvasx(abs_coord_x) / Screen.ScaleFactor - Screen.TrackOriginX
   cry = Screen.TrackOriginY - Screen.canvas.canvasy(abs_coord_y) / Screen.ScaleFactor

   Screen.StatusBar.config(text="[%d, %d]" % (crx, cry))

if __name__ == "__main__":

   ConfigData = rl.LoadCfg()

   # setup window / screen
   master = tk.Tk()
   master.wm_title(os.path.basename(__file__))
   Screen = MyScreen(master)

   #------------------------------------------
   # MQTT START (after Screen & Data setup)
   mqttc = rl.MQttClient(ConfigData['MqttIp'])
   BlobMsg = blob.BlobMsg()

   def DataTakt():
      UpdatePosition = 0
      if len(mqttc.MsgQueue) > 0 :
         while len(mqttc.MsgQueue) > 0 :
            Message = mqttc.MsgQueue.pop(0)

            if BlobMsg.add(Message) :

               if BlobMsg.Destination == "POINTS" :

                  # get datapoints
                  (String, Data) = BlobMsg.GetMetaData()
                  if String != "" :
                     print("BlobMsg.GetMetaData:", String)

                  GroupNr = 1
                  print(Data)
                  if len(Data) :
                     Data = Data[0]
                     if len(Data) :
                        GroupNr = Data[0]
                        print("GroupNr:", GroupNr)
                  GroupNr += 1 # internal representation = external + 1

                  # get datapoints
                  (String, Data) = BlobMsg.GetLineData()
                  if String != "" :
                     print("BlobMsg.GetLineData:", String)

                  # add points
                  for s in Data :
                     Screen.AddPoint(GroupNr, s[0], s[1])

               continue # end of BlobMsg processing

            fields = Message.split()             # whitespace separated
            #print("msg: ", Message, "fields:", fields)

            #---------------
            # POSITION message
            if fields[0] == "POSITION" :
               try :
                  # add TRACK point & save pose
                  Screen.AddPoint(0, float(fields[1]), float(fields[2]))
                  PosX     = float(fields[1])
                  PosY     = float(fields[2])
                  PosAngle = float(fields[3])
                  UpdatePosition = 1
               except Exception as e:
                  print("error 180102"+str(e))

            #---------------
            # POINTS message
            elif fields[0] == "POINTS" :
               Screen.PointMessage(Message)

         # update Robot pose if any POSITION message were processed
         if UpdatePosition == 1:
            Screen.RobotPosition(PosX, PosY, PosAngle)

      master.after(100, DataTakt)

   master.after(1000, DataTakt)
   master.after(1000, UpdateStatusBar)

   # Test msgs
   #RLCommon.MsgQueue.append("POSITION 100 100 3.14159")
   #RLCommon.MsgQueue.append("POINTS 1 100 100")
   #
   #RLCommon.MsgQueue.append("POINTS Erase 1")
   #
   #RLCommon.MsgQueue.append("POINTS 1 200,100 200,200")
   #RLCommon.MsgQueue.append("POINTS 0 300 100")
   #RLCommon.MsgQueue.append("POINTS 1 300 200")
   #RLCommon.MsgQueue.append("POINTS 2 300 300")
   #RLCommon.MsgQueue.append("POINTS 3 300 400")
   #
   #RLCommon.MsgQueue.append("POINTS 1")

   master.mainloop()
