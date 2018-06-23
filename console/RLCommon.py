try:
    # Python 3.x
    import tkinter as tk
except ImportError:
    # Python 2.x
    import Tkinter as tk

# Tooltip ---------------------------------------------------------------------
# http:#www.voidspace.org.uk/python/weblog/arch_d7_2006_07_01.shtml
class ToolTip(object):

    def __init__(self, widget):
        self.widget = widget
        self.tipwindow = None
        self.id = None
        self.x = self.y = 0

    def showtip(self, text):
        "Display text in tooltip window"
        self.text = text
        if self.tipwindow or not self.text:
            return
        x, y, cx, cy = self.widget.bbox("insert")
        x = x + self.widget.winfo_rootx() + 27
        y = y + cy + self.widget.winfo_rooty() +27
        self.tipwindow = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(1)
        tw.wm_geometry("+%d+%d" % (x, y))
        try:
            # For Mac OS
            tw.tk.call("::tk::unsupported::MacWindowStyle",
                       "style", tw._w,
                       "help", "noActivates")
        except tk.TclError:
            pass
        label = tk.Label(tw, text=self.text, justify=tk.LEFT,
                      background="#ffffe0", relief=tk.SOLID, borderwidth=1,
                      font=("tahoma", "8", "normal"))
        label.pack(ipadx=1)

    def hidetip(self):
        tw = self.tipwindow
        self.tipwindow = None
        if tw:
            tw.destroy()

def createToolTip(widget, text):
    toolTip = ToolTip(widget)
    def enter(event):
        toolTip.showtip(text)
    def leave(event):
        toolTip.hidetip()
    widget.bind('<Enter>', enter)
    widget.bind('<Leave>', leave)
# Tooltip ---------------------------------------------------------------------


# MQtt ------------------------------------------------------------------------
import paho.mqtt.client as mqtt
#import osimport 
import sys

def on_connect(client, userdata, flags, rc):
   print("Connection returned result: ", rc)
   client.subscribe("Robotlib/MsgFromRobot", qos=0)
   if rc == 0:
      #rc 0 successful connect
      print("Connected")
   else:
      raise Exception

def LoadCfg() :
   # load configuration file
   import json
   try:
      with open('RlCommonCfg.json', 'r') as fp:
         CfgData = json.load(fp)

   except:
      # no config => load defaults and & save (template) config file
      CfgData = {  'MqttIp'    : "127.0.0.1",
                   'MqttPort'  : 1883}
      with open('RlCommonCfg.json', 'w') as fp:
         json.dump(CfgData, fp, sort_keys=True, indent=4)

   return CfgData

def MQttClient(on_message) :

   ConfigData = LoadCfg()

   #create an mqtt client
   mypid = os.getpid()
   client_uniq = "pc_pub_"+str(mypid)
   mqttc = mqtt.Client(client_uniq)

   #attach MQTT callbacks
   mqttc.on_connect = on_connect
   mqttc.on_message = on_message

   #connect to broker
   print("Connect to mqtt server at " + ConfigData['MqttIp'])
   mqttc.connect(ConfigData['MqttIp'], ConfigData['MqttPort'], 60)

   mqttc.loop_start()   # run mqtt client in separate thread

   return mqttc         # return client in case it needed (e.g. to publish messages)

MsgQueue=[]

def on_message(client, userdata, message):
#    print("Received message '" + message.payload.decode("cp1252", 'ignore') + "' on topic '"
#        + message.topic + "' with QoS " + str(message.qos))
    MsgData = message.payload.decode("cp1252", 'ignore')
    fields = MsgData.split(' ')             # space separted
    global MsgQueue
    MsgQueue.append(MsgData)
# MQtt ------------------------------------------------------------------------


def quit():
    root.quit()     # stops mainloop
    root.destroy()  # this is necessary on Windows to prevent
                    # Fatal Python Error: PyEval_RestoreThread: NULL tstate

# combine tkinter & plots:
# https://hardsoftlucid.wordpress.com/various-stuff/realtime-plotting/https://hardsoftlucid.wordpress.com/various-stuff/realtime-plotting/
#import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg

def CreateTkRoot() :
   global root    # create copy of root in this namespace for quit
   root = tk.Tk()
   root.protocol("WM_DELETE_WINDOW", quit)  #thanks aurelienvlg
   return root

def AddFigToCanvas(fig):
   global root
   canvas = FigureCanvasTkAgg(fig, master=root)
   canvas.show()
   canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

   toolbar = NavigationToolbar2TkAgg( canvas, root )
   toolbar.update()
   canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=1)
   return canvas

# FrameDecoder ---------------------------------------------------------------------
# SLIP decoder by Roman Haefeli (with some modifications)
# https://github.com/reduzent/pyslip
class FrameDecoder():

   def __init__(self):
      self.escaped = False
      self.packet = ''
      self.SLIP_END = '\xc0'     # dec: 192
      self.SLIP_ESC = '\xdb'     # dec: 219
      self.SLIP_ESC_END = '\xdc' # dec: 220
      self.SLIP_ESC_ESC = '\xdd' # dec: 221

   def decode(self, stream):
      packetlist = []
      if (sys.version_info > (3, 0)):
     		data = stream.decode('iso-8859-1')
      else:
      	data = stream
      
      for char in data:
      	 # SLIP_END
         if char == self.SLIP_END:

            # On top of SLIP: packets need to contain a
            # readable ASCII char on the first position
            # (33 ... 126)
            # This allows to combine frames in a stream with
            # other data, if a SLIP_END is sent before the
            # frame and SLIPSTART + LF (or alike) at the end.
            if (len(self.packet) > 0) and (self.packet[0]  > ' ') and (self.packet[0]  <= '~') :
               packetlist.append(self.packet)
            self.packet = ''
         # SLIP_ESC
         elif char == self.SLIP_ESC:
            self.escaped = True
         # SLIP_ESC_END
         elif char == self.SLIP_ESC_END:
            if self.escaped:
               self.packet += self.SLIP_END
               self.escaped = False
            else:
               self.packet += char
         # SLIP_ESC_ESC
         elif char == self.SLIP_ESC_ESC:
            if self.escaped:
               self.packet += self.SLIP_ESC
               self.escaped = False
            else:
               self.packet += char
         # all others
         else:
            if self.escaped:
               raise Exception('SLIP Protocol Error')
               self.packet = ''
               self.escaped = False
            else:
               self.packet += char
      #self.stream = ''
      #self.started = False
      return (packetlist)

