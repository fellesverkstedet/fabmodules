#
# panel_path_gcc.py
#    make .gcc from .path
# Based on panel_path_epi.py
#
# Neil Gershenfeld 8/19/13
# (c) Massachusetts Institute of Technology 2013
# Jon Nordby 9/1/14
# (c) Jon Nordby, Fellesverkstedet 2014
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge the fab modules project. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

#
# imports
#
import wx,os,string
#
# panel class
#
class path_gcc_panel(wx.Panel):
   def __init__(self,parent):
      self.parent = parent
      self.parent.path_file = ''
      #
      # make gcc
      #
      def make_gcc(event):
         if (self.parent.path_file == ''):
            print 'panel_path_gcc: oops -- need path file'
            return
         self.parent.gcc_file = self.parent.tmp+self.parent.rootname+'.gcc'
         if (string.find(self.parent.path_type,"2D") != -1):
            power = self.power_2D.GetValue()
            focus = '0'

            ox = self.origin_x_2D.GetValue()
            oy = self.origin_y_2D.GetValue()
            if self.top_left_2D.GetValue():
               loc = 'L'
            elif self.top_right_2D.GetValue():
               loc = 'R'
            elif self.bottom_left_2D.GetValue():
               loc = 'l'
            elif self.bottom_right_2D.GetValue():
               loc = 'r'
            speed = self.speed_2D.GetValue()
            rate = self.rate_2D.GetValue()
            command = 'path_gcc '+'\"'+self.parent.path_file+'\"'+' '+'\"'+self.parent.gcc_file+'\"'+' '+power+' '+speed+' '+focus+' '+ox+' '+oy+' '+loc+' '+' '+rate
            print command
            os.system(command)
            self.button.Show()
            self.parent.Layout()
            self.parent.Fit()
         elif (string.find(self.parent.path_type,"3D") != -1):
            min_power = self.min_power_3D.GetValue()
            max_power = self.max_power_3D.GetValue()
            focus = '0'
            ox = self.origin_x_3D.GetValue()
            oy = self.origin_y_3D.GetValue()
            if self.top_left_3D.GetValue():
               loc = 'L'
            elif self.top_right_3D.GetValue():
               loc = 'R'
            elif self.bottom_left_3D.GetValue():
               loc = 'l'
            elif self.bottom_right_3D.GetValue():
               loc = 'r'
            speed = self.speed_3D.GetValue()
            rate = self.rate_3D.GetValue()
            command = 'path_gcc '+'\"'+self.parent.path_file+'\"'+' '+'\"'+self.parent.gcc_file+'\"'+' '+min_power+' '+speed+' '+focus+' '+ox+' '+oy+' '+loc+' '+' '+rate+' '+max_power
            print command
            os.system(command)
            self.button.Show()
            self.parent.Layout()
            self.parent.Fit()
         else:
            print "panel_path_gcc: oops -- don't recognize path type"
            return
      #
      # send
      #
      def fab_send(event):
         command = 'fab_send '+'\"'+self.parent.gcc_file+'\"'
         print command
         os.system(command)
      #
      # panel
      #
      wx.Panel.__init__(self,parent)
      self.sizer = wx.GridBagSizer(10,10)
      self.SetSizer(self.sizer)
      #
      # label
      #
      label = wx.StaticText(self,label='to: gcc')
      bold_font = wx.Font(10,wx.DEFAULT,wx.NORMAL,wx.BOLD)
      label.SetFont(bold_font)
      self.sizer.Add(label,(0,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      # send
      #
      self.button = wx.Button(self,label='send it!')
      self.button.Bind(wx.EVT_BUTTON,fab_send)
      self.button.SetFont(bold_font)
      self.sizer.Add(self.button,(1,0),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.button.Hide()
      #
      # controls
      #
      make = wx.Button(self,label='make .gcc')
      make.Bind(wx.EVT_BUTTON,make_gcc)
      self.sizer.Add(make,(2,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      # 2D panel
      #
      self.panel_2D = wx.Panel(self)
      sizer_2D = wx.GridBagSizer(10,10)
      self.panel_2D.SetSizer(sizer_2D)
      #
      sizer_2D.Add(wx.StaticText(self.panel_2D,label='remember to focus!'),(0,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      sizer_2D.Add(wx.StaticText(self.panel_2D,label='power (%)'),(1,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      self.power_2D = wx.TextCtrl(self.panel_2D,-1,'25')
      sizer_2D.Add(self.power_2D,(2,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      set_panel_2D = wx.Panel(self.panel_2D)
      set_sizer_2D = wx.GridBagSizer(10,10)
      set_panel_2D.SetSizer(set_sizer_2D)
      #
      set_sizer_2D.Add(wx.StaticText(set_panel_2D,label='speed (%)'),(0,0),flag=wx.ALIGN_RIGHT)
      set_sizer_2D.Add(wx.StaticText(set_panel_2D,label='rate'),(0,1),flag=wx.ALIGN_LEFT)
      #
      self.speed_2D = wx.TextCtrl(set_panel_2D,-1,'75')
      set_sizer_2D.Add(self.speed_2D,(1,0),flag=wx.ALIGN_RIGHT)
      self.rate_2D = wx.TextCtrl(set_panel_2D,-1,'500')
      set_sizer_2D.Add(self.rate_2D,(1,1),flag=wx.ALIGN_LEFT)
      #
      sizer_2D.Add(set_panel_2D,(3,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      #
      origin_panel_2D = wx.Panel(self.panel_2D)
      origin_sizer_2D = wx.GridBagSizer(10,10)
      origin_panel_2D.SetSizer(origin_sizer_2D)
      #
      self.top_left_2D = wx.RadioButton(origin_panel_2D,-1,'left',(10,10),style=wx.RB_GROUP)
      origin_sizer_2D.Add(self.top_left_2D,(0,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      origin_sizer_2D.Add(wx.StaticText(origin_panel_2D,label='top'),(0,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.top_right_2D = wx.RadioButton(origin_panel_2D,-1,'right',(10,10))
      origin_sizer_2D.Add(self.top_right_2D,(0,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      self.origin_x_2D = wx.TextCtrl(origin_panel_2D,-1,'10')
      origin_sizer_2D.Add(self.origin_x_2D,(1,0),flag=wx.ALIGN_RIGHT)
      origin_sizer_2D.Add(wx.StaticText(origin_panel_2D,label='x  origin (mm)  y'),(1,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.origin_y_2D = wx.TextCtrl(origin_panel_2D,-1,'10')
      origin_sizer_2D.Add(self.origin_y_2D,(1,2),flag=wx.ALIGN_LEFT)
      #
      self.bottom_left_2D = wx.RadioButton(origin_panel_2D,-1,'left',(10,10))
      origin_sizer_2D.Add(self.bottom_left_2D,(2,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      origin_sizer_2D.Add(wx.StaticText(origin_panel_2D,label='bottom'),(2,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.bottom_right_2D = wx.RadioButton(origin_panel_2D,-1,'right',(10,10))
      #
      origin_sizer_2D.Add(self.bottom_right_2D,(2,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      sizer_2D.Add(origin_panel_2D,(4,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      #
      self.sizer.Add(self.panel_2D,(3,0),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      self.path_type = "2D"
      #
      # 3D panel
      #
      self.panel_3D = wx.Panel(self)
      sizer_3D = wx.GridBagSizer(10,10)
      self.panel_3D.SetSizer(sizer_3D)
      #
      set_panel_3D = wx.Panel(self.panel_3D)
      set_sizer_3D = wx.GridBagSizer(10,10)
      set_panel_3D.SetSizer(set_sizer_3D)
      #
      set_sizer_3D.Add(wx.StaticText(set_panel_3D,label='min z power (%)'),(0,0),flag=wx.ALIGN_RIGHT)
      set_sizer_3D.Add(wx.StaticText(set_panel_3D,label='max z power (%)'),(0,1),flag=wx.ALIGN_LEFT)
      #
      self.min_power_3D = wx.TextCtrl(set_panel_3D,-1,'25')
      set_sizer_3D.Add(self.min_power_3D,(1,0),flag=wx.ALIGN_RIGHT)
      self.max_power_3D = wx.TextCtrl(set_panel_3D,-1,'25')
      set_sizer_3D.Add(self.max_power_3D,(1,1),flag=wx.ALIGN_LEFT)
      #
      set_sizer_3D.Add(wx.StaticText(set_panel_3D,label='speed (%)'),(2,0),flag=wx.ALIGN_RIGHT)
      set_sizer_3D.Add(wx.StaticText(set_panel_3D,label='rate'),(2,1),flag=wx.ALIGN_LEFT)
      #
      self.speed_3D = wx.TextCtrl(set_panel_3D,-1,'75')
      set_sizer_3D.Add(self.speed_3D,(3,0),flag=wx.ALIGN_RIGHT)
      self.rate_3D = wx.TextCtrl(set_panel_3D,-1,'500')
      set_sizer_3D.Add(self.rate_3D,(3,1),flag=wx.ALIGN_LEFT)
      #
      sizer_3D.Add(set_panel_3D,(1,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      #
      origin_panel_3D = wx.Panel(self.panel_3D)
      origin_sizer_3D = wx.GridBagSizer(10,10)
      origin_panel_3D.SetSizer(origin_sizer_3D)
      #
      self.top_left_3D = wx.RadioButton(origin_panel_3D,-1,'left',(10,10),style=wx.RB_GROUP)
      origin_sizer_3D.Add(self.top_left_3D,(0,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      origin_sizer_3D.Add(wx.StaticText(origin_panel_3D,label='top'),(0,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.top_right_3D = wx.RadioButton(origin_panel_3D,-1,'right',(10,10))
      origin_sizer_3D.Add(self.top_right_3D,(0,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      self.origin_x_3D = wx.TextCtrl(origin_panel_3D,-1,'10')
      origin_sizer_3D.Add(self.origin_x_3D,(1,0),flag=wx.ALIGN_RIGHT)
      origin_sizer_3D.Add(wx.StaticText(origin_panel_3D,label='x  origin (mm)  y'),(1,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.origin_y_3D = wx.TextCtrl(origin_panel_3D,-1,'10')
      origin_sizer_3D.Add(self.origin_y_3D,(1,2),flag=wx.ALIGN_LEFT)
      #
      self.bottom_left_3D = wx.RadioButton(origin_panel_3D,-1,'left',(10,10))
      origin_sizer_3D.Add(self.bottom_left_3D,(2,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      origin_sizer_3D.Add(wx.StaticText(origin_panel_3D,label='bottom'),(2,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.bottom_right_3D = wx.RadioButton(origin_panel_3D,-1,'right',(10,10))
      origin_sizer_3D.Add(self.bottom_right_3D,(2,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      sizer_3D.Add(origin_panel_3D,(2,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      #
      self.panel_3D.Hide()
   #
   # parent call to update panel
   #
   def update_panel(self):
      if (string.find(self.parent.path_type,"3D") != -1):
         self.sizer.Detach(self.panel_2D)
         self.panel_2D.Hide()
         self.sizer.Add(self.panel_3D,(3,0),flag=(wx.ALIGN_CENTER_HORIZONTAL))
         self.panel_3D.Show()
      elif (string.find(self.parent.path_type,"2D") != -1):
         self.sizer.Detach(self.panel_3D)
         self.panel_3D.Hide()
         self.sizer.Add(self.panel_2D,(3,0),flag=(wx.ALIGN_CENTER_HORIZONTAL))
         self.panel_2D.Show()
      self.Layout()
      self.Fit()
