#!/usr/bin/python
# coding=utf8

# importation de pyGtk pour l'interface graphique
try : 
	import gtk
	import pygtk
	pygtk.require("2.0")
except:
	print >> sys.stderr, 'Vous devez installer pyGtk'
	sys.exit(1)

# importation de vte pour pouvoir ajouter un terminal
try:
	import vte
except:
	error = gtk.MessageDialog(None, gtk.DIALOG_MODAL, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK,
	'Vous devez installer les bindings Python de libvte')
	error.run()
	sys.exit(1)

import os
import sys


class InterfaceDebug:
	def __init__(self):
		# interface du fichier GtkBuilder
		interface = gtk.Builder()
		interface.add_from_file('interface_debug_2015.glade')
		interface.connect_signals(self) # on relie les signaux aux fontions

		# récupération des widgets

		self.flecheHaut   = interface.get_object('flecheHaut')
		self.flecheBas    = interface.get_object('flecheBas')
		self.flecheGauche = interface.get_object('flecheGauche')
		self.flecheDroite = interface.get_object('flecheDroite')

		# ------ servos -------		
# 		self.pincesHFermees   = interface.get_object('pincesHFermees')		#new
# 		self.pincesHOuvertes  = interface.get_object('pincesHOuvertes')
# 		self.pincesBFermees   = interface.get_object('pincesBFermees')
# 		self.pincesBOuvertes  = interface.get_object('pincesBOuvertes')
# 		self.pincesTFermees   = interface.get_object('pincesTFermees')
# 		self.pincesTOuvertes  = interface.get_object('pincesTOuvertes')
# 		self.brasGOuvert      = interface.get_object('brasGOuvert')
# 		self.brasGFerme       = interface.get_object('brasGFerme')
# 		self.brasDOuvert      = interface.get_object('brasDOuvert')
# 		self.brasDFerme       = interface.get_object('brasDFerme')
# 		self.pinceCarteOuvert = interface.get_object('pinceCarteOuvert')
# 		self.pinceCarteFerme  = interface.get_object('pinceCarteFerme')
# 		self.brasCarteOuvert  = interface.get_object('brasCarteOuvert')
# 		self.brasCarteFerme   = interface.get_object('brasCarteFerme')
# 		self.coffreOuvert     = interface.get_object('coffreOuvert')
# 		self.coffreFerme      = interface.get_object('coffreFerme')
# 		self.servoBenneAvantBas = interface.get_object('servoBenneAvantBas')
# 		self.servoBenneAvantHaut = interface.get_object('servoBenneAvantHaut')

		# ------ asservissements ---------
		#self.moissoneuseMarche  = interface.get_object('moissoneuseMarche')
		#self.moissoneuseArret   = interface.get_object('moissoneuseArret')
# 		self.glissiereTOuverte  = interface.get_object('glissiereTOuverte')
# 		self.glissiereTFermee   = interface.get_object('glissiereTFermee')
# 		self.glissiereCOuverte  = interface.get_object('glissiereCOuverte')
# 		self.glissiereCFermee   = interface.get_object('glissiereCFermee')
		
		self.debugTrames   = interface.get_object('debugTrames')
		
		self.initialisation = interface.get_object('initialisationMeca')

		# ajout du terminal
		self.terminal = vte.Terminal()
		self.terminal.connect('child-exited', lambda term: gtk.main_quit())
		self.terminal.fork_command()
		
		self.vboxConsole = interface.get_object('vboxConsole')
		self.vboxConsole.pack_start(self.terminal, True, True)

		# affichage de la fenêtre principale
		self.interface = interface.get_object('graphique')
		[self.interfaceWidth, self.interfaceHeight] = self.interface.get_size()

		self.interface.resize(self.interfaceWidth, self.interfaceHeight)
		self.interface.show_all()
		
		self.console = interface.get_object('console')
		[self.consoleWidth, self.consoleHeight] = self.console.get_size()

		self.console.resize(self.consoleWidth, self.consoleHeight)
		self.console.show_all()

		# focus sur le terminal
		self.terminal.grab_focus()
		
		# on lance le programme robotTerm qui permet de lancer l'interface python/c++
		self.terminal.feed_child('./python_term\n')
		
		# pour l'auto-complétion
		self.terminal.feed_child('import rlcompleter \n')
		self.terminal.feed_child('import readline \n')
		
		# ajout de l'auto-complétion au terminal
		self.terminal.feed_child('readline.parse_and_bind("tab: complete")\n')

		# pour sleep
		self.terminal.feed_child('import time \n')

		# fonctions utilitaires
		self.terminal.feed_child('from utils import * \n')
##############################################				
		# Creation du robot
		self.terminal.feed_child('robot = Robot2015("RS232", "/dev/ttyUSB0", 1234, 600, 6500)\n')
		#self.terminal.feed_child('robot = Robot2015("TCPIP", "127.0.0.1", 1234, 600, 6500)\n')
		self.terminal.feed_child('dep = robot.getCarteDeplacement()\n')
# 		self.terminal.feed_child('ihm = robot.getCarteIHM()\n')
		# Meca
		self.terminal.feed_child('meca = MecaManager2015(robot, CouleurRobot.COULEUR_ROBOT_ROUGE)\n')
		# servos
		#self.terminal.feed_child('servoAvantDroit = robot.getCarteServosNovaAvantDroit()\n')
		#self.terminal.feed_child('servoAvantGauche = robot.getCarteServosNovaAvantGauche()\n')
		#self.terminal.feed_child('servoAutres = robot.getCarteServosNovaAutres()\n')
		# asservissements
		#self.terminal.feed_child('asserMoi = robot.getCarteAsservissementMoissoneuse()\n')
		#self.terminal.feed_child('asserCoffre = robot.getCarteAsservissementCoffre()\n')
		#self.terminal.feed_child('asserLingot = robot.getCarteAsservissementLingotTotem()\n')
		#self.terminal.feed_child('carteEvitement = robot.getCarteEvitement()\n')
		
	# callbacks Gtk
	
	def onWindowsDestroy(self, widget, date=None):
		gtk.main_quit()
		
	# ------ button events ------
	def onButtonPressed(self, widget, data=None):
		if widget == self.flecheHaut:
			self.avancer()
		elif widget == self.flecheBas:
			self.reculer()
		elif widget == self.flecheGauche:
			self.tournerGauche()
		elif widget == self.flecheDroite:
			self.tournerDroite()
		# ----- servos -------
# 		elif widget == self.pincesHFermees:
# 			self.fermerPincesHaut()
# 		elif widget == self.pincesHOuvertes:
# 			self.ouvrirPincesHaut()
# 		elif widget == self.pincesBFermees:
# 			self.fermerPincesBas()
# 		elif widget == self.pincesBOuvertes:
# 			self.ouvrirPincesBas()
# 		elif widget == self.pincesTFermees:
# 			self.fermerPincesTotem()
# 		elif widget == self.pincesTOuvertes:
# 			self.ouvrirPincesTotem()
# 		elif widget == self.brasGOuvert:
# 			self.ouvrirBrasGauche()
# 		elif widget == self.brasGFerme:
# 			self.fermerBrasGauche()
# 		elif widget == self.brasDOuvert:
# 			self.ouvrirBrasDroit()
# 		elif widget == self.brasDFerme:
# 			self.fermerBrasDroit()
# 		elif widget == self.pinceCarteOuvert:
# 			self.ouvrirPinceCarte()
# 		elif widget == self.pinceCarteFerme:
# 			self.fermerPinceCarte()
# 		elif widget == self.brasCarteOuvert:
# 			self.ouvrirBrasCarte()
# 		elif widget == self.brasCarteFerme:
# 			self.fermerBrasCarte()
# 		elif widget == self.coffreOuvert:
# 			self.ouvrirCoffre()
# 		elif widget == self.coffreFerme:
# 			self.fermerCoffre()
# 		elif widget == self.servoBenneAvantBas:
# 			self.baisserBenneAvant()
# 		elif widget == self.servoBenneAvantHaut:
# 			self.monterBenneAvant()
		# ----- asservissements ------
		#elif widget == self.moissoneuseMarche:
		#	self.gestionMoissoneuse('START')
		#elif widget == self.moissoneuseArret:
		#	self.gestionMoissoneuse('STOP')
# 		elif widget == self.glissiereTOuverte:
# 			self.gestionGlissiere('SORTIE')
# 		elif widget == self.glissiereTFermee:
# 			self.gestionGlissiere('RENTREE')
# 		elif widget == self.glissiereCOuverte:
# 			self.gestionCoffre('OUVERT')
# 		elif widget == self.glissiereCFermee:
# 			self.gestionCoffre('FERME')
		# ---- autres -----
# 		elif widget == self.initialisation:
# 			self.initialisationMeca()

	def onButtonReleased(self, widget, data=None):
		self.arreter()
		
	# ------ checkbutton events ------
	def onCheckbuttonToggled(self, widget, data=None):
		if widget == self.debugTrames:
			self.setDebugTrames(self.debugTrames.get_active())

	# ------ window key events ------
	def onWindowKeyPressedEvent(self, widget, event, data=None):
		if event.keyval == gtk.keysyms.Up:
			self.avancer()
		elif event.keyval == gtk.keysyms.Down:
			self.reculer()
		elif event.keyval == gtk.keysyms.Left:
			self.tournerGauche()
		elif event.keyval == gtk.keysyms.Right:
			self.tournerDroite()
		# ----- servos -------
# 		elif event.keyval == gtk.keysyms.q:
# 			self.fermerPincesHaut()
# 		elif event.keyval == gtk.keysyms.a:
# 			self.ouvrirPincesHaut()
# 		elif event.keyval == gtk.keysyms.s:
# 			self.fermerPincesBas()
# 		elif event.keyval == gtk.keysyms.z:
# 			self.ouvrirPincesBas()
# 		elif event.keyval == gtk.keysyms.d:
# 			self.fermerPincesTotem()
# 		elif event.keyval == gtk.keysyms.e:
# 			self.ouvrirPincesTotem()
# 		elif event.keyval == gtk.keysyms.r:
# 			self.ouvrirBrasGauche()
# 		elif event.keyval == gtk.keysyms.f:
# 			self.fermerBrasGauche()
# 		elif event.keyval == gtk.keysyms.t:
# 			self.ouvrirBrasDroit()
# 		elif event.keyval == gtk.keysyms.g:
# 			self.fermerBrasDroit()
# 		elif event.keyval == gtk.keysyms.y:
# 			self.ouvrirPinceCarte()
# 		elif event.keyval == gtk.keysyms.h:
# 			self.fermerPinceCarte()
# 		elif event.keyval == gtk.keysyms.u:
# 			self.ouvrirBrasCarte()
# 		elif event.keyval == gtk.keysyms.j:
# 			self.fermerBrasCarte()
# 		elif event.keyval == gtk.keysyms.i:
# 			self.ouvrirCoffre()
# 		elif event.keyval == gtk.keysyms.k:
# 			self.fermerCoffre()
# 		elif event.keyval == gtk.keysyms.o:
# 			self.baisserBenneAvant()
# 		elif event.keyval == gtk.keysyms.l:
# 			self.monterBenneAvant()
		# ----- asservissements ------
# 		elif event.keyval == gtk.keysyms.p:
# 			self.gestionGlissiere('SORTIE')
# 		elif event.keyval == gtk.keysyms.m:
# 			self.gestionGlissiere('RENTREE')
# 		elif event.keyval == gtk.keysyms.w:
# 			self.gestionCoffre('OUVERT')
# 		elif event.keyval == gtk.keysyms.x:
# 			self.gestionCoffre('FERME')
		# ---- autres -----
# 		elif event.keyval == gtk.keysyms.exclam:
# 			self.initialisationMeca()

	def onWindowKeyReleaseEvent(self, widget, event, data=None):
		self.arreter()
	# ------------------------------------------------------------------

	# ------ actions ------
	def avancer(self):
		self.terminal.feed_child('dep.avancerInfini(1000, SensAvance.SENS_AVANT) \n')

	def reculer(self):
		self.terminal.feed_child('dep.avancerInfini(1000, SensAvance.SENS_ARRIERE) \n')
	
	def tournerGauche(self):
		self.terminal.feed_child('dep.tournerInfini(4000, SensRotation.SENS_TRIGO) \n')
		
	def tournerDroite(self):
		self.terminal.feed_child('dep.tournerInfini(4000, SensRotation.SENS_HORAIRE) \n')
	
	def arreter(self):
		self.terminal.feed_child('dep.arretUrgence() \n')
	
	# ------ asservissements ----------
	#def gestionMoissoneuse(self, valeur):
	#	if valeur=='START':
	#		self.terminal.feed_child('meca.GestionMoissoneuse(vitesseRotationMoissoneuse, True) \n')
	#	elif valeur=='STOP':
	#		self.terminal.feed_child('meca.GestionMoissoneuse(vitesseRotationMoissoneuse, False) \n')
# 	def gestionCoffre(self, valeur):
# 		if valeur=='OUVERT':
# 			self.terminal.feed_child('meca.PosGlissiereCoffreSec(PositionsLongitudinalesGlissiereCoffre.COFFRE_OUVERT) \n')
# 		elif valeur=='FERME':
# 			self.terminal.feed_child('meca.PosGlissiereCoffreSec(PositionsLongitudinalesGlissiereCoffre.COFFRE_FERME) \n')
# 
# 	def gestionGlissiere(self, valeur):
# 		if valeur=='SORTIE':
# 			self.terminal.feed_child('meca.PosGlissiereLingotSec(PositionsLongitudinalesGlissiereLingot.GLISSIERE_SORTIE) \n')
# 		elif valeur=='RENTREE':
# 			self.terminal.feed_child('meca.PosGlissiereLingotSec(PositionsLongitudinalesGlissiereLingot.GLISSIERE_RANGEE) \n')

	# ------ servos --------------
# 	def fermerPincesHaut(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_HAUT_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_FERME) \n')
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_HAUT_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_FERME) \n')
# 	def ouvrirPincesHaut(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_HAUT_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_OUVERT) \n')
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_HAUT_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_OUVERT) \n')
# 	
# 	def fermerPincesBas(self):
# 		self.terminal.feed_child('meca.PosServoADSecBloquant(ServosAvantDroit.SERVO_BAS_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_MOITIE) \n')
# 		self.terminal.feed_child('meca.PosServoAGSecBloquant(ServosAvantGauche.SERVO_BAS_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_MOITIE) \n')
# 	def ouvrirPincesBas(self):
# 		self.terminal.feed_child('meca.PosServoADSecBloquant(ServosAvantDroit.SERVO_BAS_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_OUVERT) \n')
# 		self.terminal.feed_child('meca.PosServoAGSecBloquant(ServosAvantGauche.SERVO_BAS_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_OUVERT) \n')
# 
# 	def fermerPincesTotem(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_PINCE_TOTEM_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_FERME) \n')
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_PINCE_TOTEM_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_FERME) \n')
# 	def ouvrirPincesTotem(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_PINCE_TOTEM_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_OUVERT) \n')
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_PINCE_TOTEM_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_OUVERT) \n')
# 
# 	def fermerBrasDroit(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_BRAS_CALE_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_FERME) \n')
# 	def ouvrirBrasDroit(self):
# 		self.terminal.feed_child('meca.PosServoADSec(ServosAvantDroit.SERVO_BRAS_CALE_DROIT, PositionServosAvantDroit.POSITION_SERVO_AD_OUVERT) \n')
# 
# 	def fermerBrasGauche(self):
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_BRAS_CALE_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_FERME) \n')
# 	def ouvrirBrasGauche(self):
# 		self.terminal.feed_child('meca.PosServoAGSec(ServosAvantGauche.SERVO_BRAS_CALE_GAUCHE, PositionServosAvantGauche.POSITION_SERVO_AG_OUVERT) \n')
# 
# 	def fermerBrasCarte(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_BRAS_CARTE, PositionServosAutres.POSITION_SERVO_AUTRES_FERME) \n')
# 	def ouvrirBrasCarte(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_BRAS_CARTE, PositionServosAutres.POSITION_SERVO_AUTRES_OUVERT) \n')
# 
# 	def fermerPinceCarte(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_PINCE_CARTE, PositionServosAutres.POSITION_SERVO_AUTRES_FERME) \n')
# 	def ouvrirPinceCarte(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_PINCE_CARTE, PositionServosAutres.POSITION_SERVO_AUTRES_OUVERT) \n')
# 
# 	def fermerCoffre(self):
# 		self.terminal.feed_child('meca.OuvrirCoffreSec(False) \n')
# 	def ouvrirCoffre(self):
# 		self.terminal.feed_child('meca.OuvrirCoffreSec(True) \n')
# 
# 	def baisserBenneAvant(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_BENNE_AVANT, PositionServosAutres.POSITION_SERVO_AUTRES_FERME) \n')
# 	def monterBenneAvant(self):
# 		self.terminal.feed_child('meca.PosServoAutreSec(ServosAutres.SERVO_BENNE_AVANT, PositionServosAutres.POSITION_SERVO_AUTRES_OUVERT) \n')


	# ------ autres ------
	def setDebugTrames(self, active):
		self.terminal.feed_child('robot.setDebugCAN(' + str(active) + ') \n')
		
# 	def initialisationMeca(self):
# 		self.terminal.feed_child('meca.Initialisation() \n')
	
if __name__ == "__main__":
	app = InterfaceDebug()
	gtk.main()
