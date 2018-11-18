# intended for use with python3

import tkinter as tk
#except ImportError:
#    # Python 2.x
#    import Tkinter as tk

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
