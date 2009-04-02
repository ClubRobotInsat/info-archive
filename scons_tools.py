# scons_tools.py

import sys
import os
from SCons.Script import *

THIS_FILE_NAME = 'scons_tools.py'

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
# - 'Webcam'
# - 'glfw'
# - 'libcwiimote'
# - 'boost_python'
def createEnvironment(libs_list=[], force_debug=False):
	command_line_options = Options()
	#command_line_options.AddOptions(['mingw', 'Utiliser le compilateur MinGW a la place de VC++.' +
	#								'Utile si vous avez l\'erreur \'cl\' n\'est pas reconnu en tant que commande interne.'])
	command_line_options.AddOptions(['debug', 'Compiler en mode debug'])

	if sys.platform == 'linux2':
		command_line_options.AddOptions(['no_libv4l1',
			'Ne pas utiliser la libv4l1 (pour la libWebcam : utilise alors V4L1 plutot que V4L2'])
		command_line_options.AddOptions(['with_python',
			'Ajoute l\'exportation vers Python et la creation d\'un terminal Python l\'utilisant'])

	env = Environment(options = command_line_options)

	# Generation du texte affiche lors du "scons --help".
	Help(command_line_options.GenerateHelpText(env))

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
			env.Append(CPPPATH=[root_dir + '/robot/Outils',
								root_dir + '/robot/Outils/Clock',
								root_dir + '/robot/Outils/log',
								root_dir + '/robot/Outils/MathToolbox'])
			env.Append(LIBPATH=[root_dir + '/robot/Outils'])
			env.Append(LIBS=['Outils'])

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

		elif lib == 'Webcam':
			env.Append(CPPPATH=[root_dir + '/webcam'])
			env.Append(LIBPATH=[root_dir + '/webcam'])
			env.Append(LIBS=['Webcam'])
			# N'existe pas et inutile sous Windows
			if sys.platform == 'linux2' and str(ARGUMENTS.get('no_libv4l1')) != '1':
				env.Append(CPPDEFINES=['USE_LIBV4L1'])
				env.ParseConfig('pkg-config libv4l1 --libs --cflags')

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

			elif sys.platform == 'linux2' and str(ARGUMENTS.get('with_python')) == '1':
				env.Append(LIBPATH=[root_dir + '/robot/boost/lib'])
				# NB : les headers de Boost.Python utilises sont ceux de /usr/include

				# /usr/lib/libboost_python.a est fourni par le paquet libboost-python-dev sous Ubuntu
				env.Append(LIBS=['boost_python'])
				env.ParseConfig('python-config --includes')
				env.ParseConfig('python-config --libs')
	return env
