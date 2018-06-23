import tkinter as tk
import random
import os

class App(tk.Tk):
   def __init__(self, *args, **kwargs):
      tk.Tk.__init__(self, *args, **kwargs)
      self.wm_title(os.path.basename(__file__))        
      self.rows         = 20
      self.columns      = 20
      self.cellheight   = 25  
      self.cellwidth    = 25
      self.canvas = tk.Canvas(self, width=(self.columns*self.cellwidth), height=(self.rows*self.cellheight), borderwidth=0, highlightthickness=0)
      self.canvas.pack(side="top", fill="both", expand="true")

      self.rect = {}
      for column in range(20):
         for row in range(20):
            x1 = column*self.cellwidth
            y1 = row * self.cellheight
            x2 = x1 + self.cellwidth
            y2 = y1 + self.cellheight
            self.rect[row,column] = self.canvas.create_rectangle(x1,y1,x2,y2, fill="white", tags="rect")

   def Draw(self, col, row, value):
#        self.canvas.itemconfig("rect", fill=('#%02x%02x%02x' % (value,value,value)))  

      # map value from 0..100 -> 255..0 & clip)
      value = int(255 - value * 2.55) 
      value = sorted((0, value, 255))[1] # clip

      row = self.rows    - 1 - row # (x == 0) is bottom row
      print("draw---", row, col, value);   
      try :
         item_id = self.rect[row,col]         
         self.canvas.itemconfig(item_id, fill=('#%02x%02x%02x' % (value, value, value)))
      except KeyError as e:
         print('GRID INDEX out of range - error msg: "%s"' % str(e))         

   
# MQtt ------------------------------------------------------------------------ 
# import RobotLib common code
import RLCommon

# define function to handle incomming messages
def on_message(client, userdata, message):
    print("Received message '" + message.payload.decode() + "' on topic '"
        + message.topic + "' with QoS " + str(message.qos))   
    fields = message.payload.decode().split(' ')             # space separted 
    if (fields[0] == "GRID") :
      print("msg:", len(fields), fields)    
      
      fields.pop(0)
      while len(fields) > 2 :
         field1 = fields.pop(0)
         field2 = fields.pop(0)
         field3 = fields.pop(0)
         app.Draw(int(field1), int(field2), int(field3))
   
if __name__ == "__main__": 
   
   # setup & connect MQtt client to receive messages from robot
   RLCommon.MQttClient(on_message)
   #-----
   
   app = App()
   app.mainloop()