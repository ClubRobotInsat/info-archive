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

# Fonction qui remonte l'arborescence depuis le repertoireen cours et renvoie le nom du repertoire qui
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
# - 'Robot2008'
# - 'Webcam'
# - 'glfw'
# - 'libcwiimote'
# NB : l'argument "root_dir" ne doit JAMAIS etre passe en parametre par l'utilisateur ; il s'agit juste
# d'une facon un peu bidouillee de recuperer le nom du repertoire ou se trouve scons_tools.py
# (si on utilise directement sys.path[0], on recupere le dossier ou se trouve le SConstruct qui a appele
# createEnvironment())
def createEnvironment(libs_list=[], force_debug=False):
	command_line_options = Options()
	#command_line_options.AddOptions(['mingw', 'Utiliser le compilateur MinGW a la place de VC++.' +
	#								'Utile si vous avez l\'erreur \'cl\' n\'est pas reconnu en tant que commande interne.'])
	command_line_options.AddOptions(['debug', 'Compiler en mode debug'])

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

	return env
