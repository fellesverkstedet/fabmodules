#
# panel_path_camm.py
#    make .camm from .path
#
# Neil Gershenfeld
# CBA MIT 2/19/11
#
# (c) Massachusetts Institute of Technology 2011
# Permission granted for experimental and personal use;
# license for commercial sale available from MIT.
#
# imports
#
import wx,os
#
# panel class
#
class path_camm_panel(wx.Panel):
   def __init__(self,parent):
      self.parent = parent
      self.parent.path_file = ''
      #
      # make file
      #
      def make_file(event):
         if (self.parent.path_file == ''):
            print 'panel_path_camm: oops -- need path file'
            return
         self.parent.camm_file = self.parent.tmp+self.parent.rootname+'.camm'
         force = self.force.GetValue()
         velocity = self.velocity.GetValue()
         ox = self.origin_x.GetValue()
         oy = self.origin_y.GetValue()
         if self.top_left.GetValue():
            loc = 'L'
         elif self.top_right.GetValue():
            loc = 'R'
         elif self.bottom_left.GetValue():
            loc = 'l'
         elif self.bottom_right.GetValue():
            loc = 'r'
         command = 'path_camm '+'\"'+self.parent.path_file+'\"'+' '+'\"'+self.parent.camm_file+'\"'+' '+force+' '+velocity+' '+ox+' '+oy+' '+loc
         print command
         os.system(command)
         self.button.Show()
         self.parent.Layout()
         self.parent.Fit()
      #
      # send
      #
      def fab_send(event):
         command = 'fab_send '+'\"'+self.parent.camm_file+'\"'
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
      label = wx.StaticText(self,label='to: camm')
      bold_font = wx.Font(10,wx.DEFAULT,wx.NORMAL,wx.BOLD)
      label.SetFont(bold_font)
      self.sizer.Add(label,(0,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      # send
      #
      self.button = wx.Button(self,label='send it!')
      self.button.Bind(wx.EVT_BUTTON,fab_send)
      self.button.SetFont(bold_font)
      self.sizer.Add(self.button,(1,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.button.Hide()
      #
      # controls
      #
      make = wx.Button(self,label='make .camm')
      make.Bind(wx.EVT_BUTTON,make_file)
      self.sizer.Add(make,(2,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      force_panel = wx.Panel(self)
      force_sizer = wx.GridBagSizer(10,10)
      force_panel.SetSizer(force_sizer)
      force_sizer.Add(wx.StaticText(force_panel,label='force (g)'),(0,0),flag=(wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL))
      self.force = wx.TextCtrl(force_panel,-1,'45')
      force_sizer.Add(self.force,(0,1),flag=wx.ALIGN_LEFT)
      #
      force_sizer.Add(wx.StaticText(force_panel,label='velocity (cm/2)'),(1,0),flag=(wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL))
      self.velocity = wx.TextCtrl(force_panel,-1,'2')
      force_sizer.Add(self.velocity,(1,1),flag=wx.ALIGN_LEFT)
      self.sizer.Add(force_panel,(3,0),span=(1,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      origin_panel = wx.Panel(self)
      origin_sizer = wx.GridBagSizer(10,10)
      origin_panel.SetSizer(origin_sizer)
      self.top_left = wx.RadioButton(origin_panel,-1,'left',(10,10),style=wx.RB_GROUP)
      origin_sizer.Add(self.top_left,(0,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      origin_sizer.Add(wx.StaticText(origin_panel,label='top'),(0,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.top_right = wx.RadioButton(origin_panel,-1,'right',(10,10))
      origin_sizer.Add(self.top_right,(0,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      #
      self.origin_x = wx.TextCtrl(origin_panel,-1,'0')
      origin_sizer.Add(self.origin_x,(1,0),flag=wx.ALIGN_RIGHT)
      origin_sizer.Add(wx.StaticText(origin_panel,label='x  origin (mm)  y'),(1,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.origin_y = wx.TextCtrl(origin_panel,-1,'0')
      origin_sizer.Add(self.origin_y,(1,2),flag=wx.ALIGN_LEFT)
      #
      self.bottom_left = wx.RadioButton(origin_panel,-1,'left',(10,10))
      origin_sizer.Add(self.bottom_left,(2,0),flag=wx.ALIGN_CENTER_HORIZONTAL)
      self.bottom_left.SetValue(True)
      origin_sizer.Add(wx.StaticText(origin_panel,label='bottom'),(2,1),flag=(wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL))
      self.bottom_right = wx.RadioButton(origin_panel,-1,'right',(10,10))
      origin_sizer.Add(self.bottom_right,(2,2),flag=wx.ALIGN_CENTER_HORIZONTAL)
      self.sizer.Add(origin_panel,(4,0),span=(1,2),flag=(wx.ALIGN_CENTER_HORIZONTAL))
      #
      # fit
      #
      self.Fit()
