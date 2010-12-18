# -*- encoding: utf-8 -*-
# scons_tools.py

import sys
import os
from SCons.Script import *

THIS_FILE_NAME = 'scons_tools.py'

# Aucune idee de comment ca marche (ca s'appelle "generator functions"...), mais
# ca definit une fonction new_id() qui renvoie une nouvelle valeur chaque fois que
# l'on l'appelle (1, 2, ...etc). On utilise ca pour eviter d'appeler Help()
# a chaque appel de createEnvironment() pour ne pas polluer la console lors d'un
# scons --help
def generator():
	i = 0
	while True:
		i += 1
		yield i

new_id = generator().next

# Renvoie '' ou '_d' suivant s'il y a eu "scons debug=1" ou pas.
def getSuffix():
	if ARGUMENTS.get('debug', 0):
		return '_d'
	else:
		return ''

# Fonction qui remonte l'arborescence depuis le repertoire en cours et renvoie le nom du repertoire qui
# contient THIS_FILE_NAME.
def getRootDir():
	final_dir = os.path.abspath(sys.path[0])	# NB : sys.path[0] ou os.getcwd()...
	while not THIS_FILE_NAME in os.listdir(final_dir):
		final_dir = os.path.abspath(final_dir + '/..')
	return final_dir

# Fonction renvoyant un nouvel Environment(), qui utilise les librairies que l'on lui indique en parametres.
# libs_list est une liste de librairies qu'utilise l'environnement renvoye.
# Les valeurs possibles sont :
# - 'pthreads'
# - 'sockets'
# - 'Robot'
# - 'Strategie'
# - 'Cartes'
# - 'Outils'
# - 'Robot2008'
# - 'Robot2009'
# - 'Robot2010'
# - 'Webcam'
# - 'Webcam2009'
# - 'Webcam2010'
# - 'glfw'
# - 'libcwiimote'
# - 'boost_python'
# - 'gtk'
# - 'zbar'
def createEnvironment(libs_list=[], force_debug=False):
	vars = Variables()
	vars.AddVariables(	#BoolVariable('mingw',    'Utiliser le compilateur MinGW a la place de VC++.' +
						#			'Utile si vous avez l\'erreur \'cl\' n\'est pas reconnu en tant que commande interne\'.', 1),
						BoolVariable('debug',     'Definit DEBUG et rajoute l\'option -g', 0),
						BoolVariable('python',    'Ajoute le support Python', 0),
						BoolVariable('gtk',       'Le simulateur utilise la fenetre de debug en GTK (sous Linux)', 0),
						BoolVariable('nobuiltin', 'Utiliser les librairies deja installees sur le systeme pour le simulateur', 0),
						BoolVariable('libv4l1', 'utiliser la libv4l1 pour la webcam sous Linux (encapsule V4L2 dans des appels ressemblant a V4L1 : utilise donc V4L2)', 0),
						BoolVariable('zbar', 'Utiliser la librairie ZBar (sous Linux) pour lire des codes barre avec la webcam', 0))
	env = Environment(variables = vars)

	# Generation du texte affiche lors du "scons --help", uniquement lors du premier appel a createEnvironment():
	if new_id() == 1:
		Help(vars.GenerateHelpText(env))
	else:
		Help('')

	root_dir = getRootDir()

	# Si on est sous Windows et que l'on nous a precise "scons mingw=1", on force l'utilisation de MinGW.
	#if sys.platform == 'win32':
	#	if ARGUMENTS.get('mingw'):
	#		Tool('mingw')(env)

	if sys.platform == 'win32':
		# Pour l'instant : on FORCE l'utilisation de MinGW (TODO : permettre VC++...)
		Tool('mingw')(env)

		# On evite l'inclusion par windows.h de choses inutiles dont winsock.h (conflit avec winsock2.h)
		env.Append(CPPDEFINES=['WIN32', 'WIN32_LEAN_AND_MEAN'])

	env.Append(CCFLAGS=['-Wall'])

	# Si "scons debug=1", alors on ajoute l'option "-g"
	if ARGUMENTS.get('debug') or force_debug:
		env.Append(CCFLAGS=['-g'])
		env.Append(CPPDEFINES=['DEBUG'])
	else:
		env.Append(CCFLAGS=['-O2'])

	# Parcours de toutes les libs passees en parametres et utilisation de celles-ci dans l'environnement cree.
	for lib in libs_list:
		if lib == 'pthreads':
			if sys.platform == 'win32':
				env.Append(CPPPATH=[root_dir + '/robot/pthreads-win32/include'])
				env.Append(LIBPATH=[root_dir + '/robot/pthreads-win32/lib'])
				env.Append(LIBS=['pthreadGC2'])
			else:
				env.Append(LIBS=['pthread'])

		elif lib == 'sockets':
			if sys.platform == 'win32':
				env.Append(LIBS=['ws2_32'])

		elif lib == 'Robot':
			env.Append(CPPPATH=[root_dir + '/robot/Robot'])
			env.Append(LIBPATH=[root_dir + '/robot/Robot'])
			env.Append(LIBS=['Robot'])

		elif lib == 'Strategie':
			env.Append(CPPPATH=[root_dir + '/robot/Strategie'])
			env.Append(LIBPATH=[root_dir + '/robot/Strategie'])
			env.Append(LIBS=['Strategie'])

		elif lib == 'Cartes':
			env.Append(CPPPATH=[root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/Cartes'])
			env.Append(LIBS=['Cartes'])

		elif lib == 'Outils':
			if sys.platform == 'win32':
				env.Append(LIBS=['ws2_32'])

			env.Append(CPPPATH=[root_dir + '/robot/Outils',
								root_dir + '/robot/Outils/Clock',
								root_dir + '/robot/Outils/debug',
								root_dir + '/robot/Outils/log',
								root_dir + '/robot/Outils/MathToolbox',
								root_dir + '/robot/Outils/tga',
								root_dir + '/robot/Outils/sockets'])
			env.Append(LIBPATH=[root_dir + '/robot/Outils'])
			env.Append(LIBS=['Outils'])

			# Ajout de GTK+, si demandé, pour CGTKwin
			if sys.platform == 'linux2' and str(ARGUMENTS.get('gtk')) == '1':
				env.ParseConfig('pkg-config gtk+-2.0 --libs --cflags')
			else:
				env.Append(CPPDEFINES=['GTKWIN_DISABLE'])

		elif lib == 'Robot2008':
			env.Append(CPPPATH=[root_dir + '/robot/2008/lib',
								root_dir + '/robot/2008/lib/Barillet',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2008/lib'])
			env.Append(LIBS=['Robot2008'])


		elif lib == 'Robot2009':
			env.Append(CPPPATH=[root_dir + '/robot/2009/libRobot2009',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2009/libRobot2009'])
			env.Append(LIBS=['Robot2009'])

		elif lib == 'Robot2010':
			env.Append(CPPPATH=[root_dir + '/robot/2010/libRobot2010',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2010/libRobot2010'])
			env.Append(LIBS=['Robot2010'])

		elif lib == 'Robot2011':
			env.Append(CPPPATH=[root_dir + '/robot/2011/titan/librobot2011',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2011/titan/librobot2011'])
			env.Append(LIBS=['Robot2011'])

		elif lib == 'Webcam':
			env.Append(CPPPATH=[root_dir + '/webcam'])
			env.Append(LIBPATH=[root_dir + '/webcam'])
			env.Append(LIBS=['Webcam'])

			# Ajout ou pas de la libv4l1 sous Linux (pour permettre d'utiliser V4L2 avec
			# l'API de V4L1)
			if sys.platform == 'linux2' and str(ARGUMENTS.get('libv4l1')) == '1':
				env.Append(CPPDEFINES=['USE_LIBV4L1'])
				env.ParseConfig('pkg-config libv4l1 --libs --cflags')

			# Sous Linux toujours : on utilise la libuvc
			if sys.platform == 'linux2':
				env.Append(LIBS=['uvc'])
				env.Append(LIBPATH=[root_dir + '/webcam/libuvc'])

		elif lib == 'Webcam2009':
			env.Append(CPPPATH=[root_dir + '/webcam/2009/libWebcam2009'])
			env.Append(LIBPATH=[root_dir + '/webcam/2009/libWebcam2009'])
			env.Append(LIBS=['Webcam2009'])

		elif lib == 'Webcam2010':
			env.Append(CPPPATH=[root_dir + '/webcam/2010/libWebcam2010'])
			env.Append(LIBPATH=[root_dir + '/webcam/2010/libWebcam2010'])
			env.Append(LIBS=['Webcam2010'])

		elif lib == 'glfw':
			env.Append(CPPPATH=[root_dir + '/simulateur/GLFW/include'])
			env.Append(LIBPATH=[root_dir + '/simulateur/GLFW/lib'])

			if sys.platform == 'win32':
				env.Append(LIBS=['glfw', 'opengl32', 'glu32'])
			else:
				env.Append(LIBS=['glfw', 'GL', 'GLU', 'm', 'X11', 'pthread', 'Xrandr'])
				env.Append(LIBPATH=['/usr/X11R6/lib'])

		elif lib == 'libcwiimote':
			if sys.platform == 'linux2':
				env.Append(CPPDEFINES=['_ENABLE_TILT', '_ENABLE_FORCE'])
				env.Append(CPPPATH=[root_dir + '/robot/libcwiimote/src'])
				env.Append(LIBPATH=[root_dir + '/robot/libcwiimote/src'])
				env.Append(LIBS=['cwiimote', 'bluetooth', 'm'])

		elif lib == 'boost_python':
			# Ajout de libpython et de Boost.Python
			if sys.platform == 'win32':
				env.Append(CPPPATH=[root_dir + '/robot/python/include', root_dir + '/robot/boost/include'])
				env.Append(LIBPATH=[root_dir + '/robot/python/lib-win32', root_dir + '/robot/boost/lib-win32'])
				env.Append(LIBS=['boost_python-mgw34-mt-1_35', 'python25'])

			elif sys.platform == 'linux2' and str(ARGUMENTS.get('python')) == '1':
				env.Append(LIBPATH=[root_dir + '/robot/boost/lib'])
				# NB : les headers de Boost.Python utilises sont ceux de /usr/include

				# /usr/lib/libboost_python[-mt].a est fourni par le paquet libboost-python-dev sous Ubuntu
				if os.path.isfile('/usr/lib/libboost_python-mt.a'):	# depend de la version d'Ubuntu...
					env.Append(LIBS=['boost_python-mt'])
				else:
					env.Append(LIBS=['boost_python'])
				env.ParseConfig('python-config --includes')
				env.ParseConfig('python-config --libs')
		elif lib == 'gtk':
			# Ajout de GTK+, si demandé, pour CGTKwin
			if sys.platform == 'linux2' and str(ARGUMENTS.get('gtk')) == '1':
				env.ParseConfig('pkg-config gtk+-2.0 --libs --cflags')
			else:
				env.Append(CPPDEFINES=['GTKWIN_DISABLE'])
		elif lib == 'zbar':
			if sys.platform == 'linux2' and str(ARGUMENTS.get('zbar')) == '1':
				env.ParseConfig('pkg-config zbar --libs --cflags')
	return env
