#!/usr/bin/env python
# coding=utf8

CONFIG_FILE = 'robot_builder.cfg'
CONFIG_FILE_VERSION = '1.0'

# import PyGTK
try:
	import gtk
	import pygtk
	import gtk
	pygtk.require("2.0")
except:
	print >> sys.stderr, 'Vous devez installer PyGTK'
	sys.exit(1)

# import VTE
try:
	import vte
except:
	error = gtk.MessageDialog (None, gtk.DIALOG_MODAL, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK,
	'Vous devez installer les bindings Python de libvte')
	error.run()
	sys.exit (1)

import os
import sys
sys.path.append('.')
from scons_help import *
import string
from string import *

class RobotBuilder(object):
	def __init__(self):
		# Lecture du fichier GtkBuilder
		builder = gtk.Builder()
		builder.add_from_file('robot_builder.glade')
		builder.connect_signals(self)

		self.entryCommand = builder.get_object('entryCommand')
		self.expanderOptions = builder.get_object('expanderOptions')

		# Remplissage de la liste des options
		self.tableOptions = builder.get_object('tableOptions')
		self.tableOptions.resize(len(sconsHelp), 2)
		i=1
		self.checkbuttons = []
		for line in sconsHelp:
			# Ajout de la checkbutton
			checkbutton = gtk.CheckButton(line[0])
			self.checkbuttons.append(checkbutton)
			self.tableOptions.attach(checkbutton, 0, 1, i, i+1)
			checkbutton.connect('toggled', self.onCheckbuttonToggled)

			# Ajout du label
			label = gtk.Label(line[1])
			label.set_alignment(0.0, 0.5)
			self.tableOptions.attach(label, 1, 2, i, i+1)

			i=i+1

		# Nombre de processeurs
		f = os.popen('cat /proc/cpuinfo | grep processor | wc -l', 'r')
		self.spinbuttonNbProcesses = builder.get_object('spinbuttonNbProcesses')
		self.spinbuttonNbProcesses.set_value(string.atof(f.read()))
		f.close()

		# Ajout du terminal
		self.vte = vte.Terminal()
		self.vte.connect('child-exited', lambda term: gtk.main_quit())
		self.vte.fork_command()

		self.vboxMain = builder.get_object('vboxMain')
		self.vboxMain.pack_start(self.vte, True, True)

		# Affichage de la fenêtre principale
		self.window = builder.get_object('window')
		[self.winWidth, self.winHeight] = self.window.get_size()

		# Chargement de la config, si config il y a
		self.loadConfig()

		# Redimensionnement de la fenêtre et affichage
		self.window.resize(self.winWidth, self.winHeight)
		self.window.show_all()

		# Focus sur le terminal
		self.vte.grab_focus()

		# Mise à jour de la commande
		self.refreshCommand()

	# ----------------------- Callbacks GTK ----------------------------
	def onWindowDestroy(self, widget, data=None):
		self.saveConfig()
		gtk.main_quit()

	def onButtonSaveConfigClicked(self, widget, data=None):
		self.saveConfig()

	def onCheckbuttonToggled(self, widget, data=None):
		self.refreshCommand()

	def onSpinbuttonNbProcessesValueChanged(self, widget, data=None):
		self.refreshCommand()

	def onButtonBuildClicked(self, widget, data=None):
		self.vte.feed_child(self.entryCommand.get_text() + '\n')

	def onButtonCleanClicked(self, widget, data=None):
		self.vte.feed_child('scons -c\n')

	def onWindowConfigureEvent(self, widget, event):
		[self.winWidth, self.winHeight] = [event.width, event.height]

	# ------------------------------------------------------------------
	def refreshCommand(self):
		command = 'scons '
		nbProcesses = int(self.spinbuttonNbProcesses.get_value())
		if nbProcesses > 1:
			command += '-j' + str(nbProcesses)
		for checkbutton in self.checkbuttons:
			if checkbutton.get_active():
				command += ' ' + checkbutton.get_label() + '=1'

		self.entryCommand.set_text(command)

	def saveConfig(self):
		f = open(CONFIG_FILE, 'w')
		f.write('version=' + CONFIG_FILE_VERSION + '\n')
		f.write('width=' + str(self.winWidth) + '\n')
		f.write('height=' + str(self.winHeight) + '\n')
		if self.expanderOptions.get_expanded():
			f.write('expanded=1\n')
		else:
			f.write('expanded=0\n')
		f.write('nb_processes=' + str(int(self.spinbuttonNbProcesses.get_value())) + '\n')

		for checkbutton in self.checkbuttons:
			s = '0'
			if checkbutton.get_active():
				s = '1'
			f.write(checkbutton.get_label() + '=' + s + '\n')

		f.close()

	def loadConfig(self):
		try:
			f = open(CONFIG_FILE, 'r')

			# Vérification du numéro de version du fichier de config (ne rien charger
			# si le numéro est différent)
			version = f.readline().split('=')[1][0:-1]
			if version != CONFIG_FILE_VERSION:
				print 'Numéro de version du fichier de configuration différent (' +	\
					version + ' vs ' + CONFIG_FILE_VERSION + '), abandon du chargement'
				f.close()
				return

			self.winWidth = int(f.readline().split('=')[1][0:-1])
			self.winHeight = int(f.readline().split('=')[1][0:-1])
			if f.readline().split('=')[1][0:-1] != '0':
				self.expanderOptions.set_expanded(True)
			else:
				self.expanderOptions.set_expanded(False)
			self.spinbuttonNbProcesses.set_value(atof(f.readline().split('=')[1][0:-1]))

			for checkbutton in self.checkbuttons:
				digit = f.readline().split('=')[1][0:-1]
				if digit == '0':
					checkbutton.set_active(False)
				else:
					checkbutton.set_active(True)

			f.close()
		except IOError:
			print 'Pas de fichier de configuration "' + CONFIG_FILE + '"'
			return

if __name__ == "__main__":
	app = RobotBuilder()
	gtk.main()
