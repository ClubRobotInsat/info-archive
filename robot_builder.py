#!/usr/bin/env python
# coding=utf8

import pygtk
import gtk
pygtk.require("2.0")

import os
import sys
sys.path.append('.')
from scons_help import *

#~ f = os.popen('scons --help')
#~ sconsHelp = f.read()
#~ f.close()

# http://stackoverflow.com/questions/2868694/python-vte-terminal-weirdness

class RobotBuilder(object):
	def __init__(self):
		# Lecture du fichier GtkBuilder
		builder = gtk.Builder()
		builder.add_from_file('robot_builder.glade')
		builder.connect_signals(self)

		# Remplissage de la liste des options
		self.tableOptions = builder.get_object('tableOptions')
		self.tableOptions.resize(len(sconsHelp), 2)
		i=0
		for line in sconsHelp:
			self.tableOptions.attach(gtk.CheckButton(line[0]), 0, 1, i, i+1)
			label = gtk.Label(line[1])
			label.set_alignment(0.0, 0.5)
			self.tableOptions.attach(label, 1, 2, i, i+1)
			i=i+1
		self.tableOptions.show_all()

		# Affichage de la fenêtre principale
		self.window = builder.get_object('window')
		self.window.show()

	# ----------------------- Callbacks GTK ----------------------------
	def onWindowDestroy(self,widget,data=None):
		gtk.main_quit()

if __name__ == "__main__":
	app = RobotBuilder()
	gtk.main()
