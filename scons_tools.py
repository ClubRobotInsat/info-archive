
# -*- encoding: utf-8 -*-
# scons_tools.py

import sys
import os
from SCons.Script import *

sys.path.append('.')
from scons_help import *

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
# - 'Robot2008', 'Robot2009', 'Robot2010', 'RobotAtlas2011', 'RobotTitan2011', 'Robot2012', 'Robot2012APP', 'Robot2013', 'RobotToumai2014', 'RobotLucy2014'
# - 'Webcam'
# - 'Webcam2009'
# - 'Webcam2010'
# - 'glfw'
# - 'libcwiimote'
# - 'boost_python'
# - 'gtk'
# - 'zbar'
# - 'libfreenect'
# - 'freeglut'
def createEnvironment(libs_list=[], force_debug=False):
	vars = Variables()
	for line in sconsHelp:
		vars.AddVariables(BoolVariable(line[0], line[1], 0))
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

		# Ça c'est pour le bug de SCons signalé ici :http://markmail.org/message/6d37krhdrypuwmhj
		# Et qu'on a déjà expérimenté ici : http://www.etud.insa-toulouse.fr/~club_robot/forum/viewtopic.php?f=7&t=13&p=240&hilit=%2Fnologo#p240
		# et ici : http://www.etud.insa-toulouse.fr/~club_robot/forum/viewtopic.php?f=7&t=88
		env['CCFLAGS'] = SCons.Util.CLVar('')

		# On evite l'inclusion par windows.h de choses inutiles dont winsock.h (conflit avec winsock2.h)
		env.Append(CPPDEFINES=['WIN32', 'WIN32_LEAN_AND_MEAN'])

	env.Append(CCFLAGS=['-Wall'])
	#env.Append(CXXFLAGS=['-std=c++11'])
	env.Append(CXXFLAGS=['-std=c++11', '-m32'])
	env.Append(LINKFLAGS=['-m32'])
	#env.Append(LIBS=['libusb-1.0'])
	if sys.platform == 'darwin':
		env.Append(CXXFLAGS=['-ferror-limit=0'])

	# Si "scons debug=1", alors on ajoute l'option "-g"
	if ARGUMENTS.get('debug', 0) or force_debug:
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
				env.Append(CPPFLAGS=['-L/usr/lib/i386-linux-gnu/'])

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

			env.Append(CPPPATH=[root_dir + '/commun',
								root_dir + '/commun/Clock',
								root_dir + '/commun/debug',
								root_dir + '/commun/log',
								root_dir + '/commun/MathToolbox',
								root_dir + '/commun/tga',
								root_dir + '/commun/sockets'])
			env.Append(LIBPATH=[root_dir + '/commun'])
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

		elif lib == 'RobotAtlas2011':
			env.Append(CPPPATH=[root_dir + '/robot/2011/atlas/libRobot2011',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2011/atlas/libRobot2011'])
			env.Append(LIBS=['RobotAtlas2011'])

		elif lib == 'RobotTitan2011':
			env.Append(CPPPATH=[root_dir + '/robot/2011/titan/librobot2011',
								root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2011/titan/librobot2011'])
			env.Append(LIBS=['RobotTitan2011'])

		elif lib == 'Robot2012':
			env.Append(CPPPATH=[root_dir + '/robot/2012/Hyperion/librobot2012',
					    root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2012/Hyperion/librobot2012'])
			env.Append(LIBS=['Robot2012'])

		elif lib == 'Robot2012APP':
			env.Append(CPPPATH=[root_dir + '/robot/2012/Hyperion/tp_app',
					    root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2012/Hyperion/tp_app'])
			env.Append(LIBS=['Robot2012APP'])

		elif lib == 'Robot2013':
			env.Append(CPPPATH=[root_dir + '/robot/2013/Chronos/librobot2013',
					    root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2013/Chronos/librobot2013'])
			env.Append(LIBS=['Robot2013'])

		elif lib == 'RobotPan2013':
			env.Append(CPPPATH=[root_dir + '/robot/2013/Pan/librobot2013',
						root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2013/Pan/librobot2013'])
			env.Append(LIBS=['RobotPan2013'])

		elif lib == 'RobotToumai2014':
			env.Append(CPPPATH=[root_dir + '/robot/2014/Toumai/librobot2014',
			    			root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2014/Toumai/librobot2014', root_dir + '/commun/2014'])
			env.Append(LIBS=['RobotToumai2014', 'Constantes2014'])

		elif lib == 'RobotLucy2014':
			env.Append(CPPPATH=[root_dir + '/robot/2014/Lucy/librobot2014',
			    			root_dir + '/robot/Cartes'])
			env.Append(LIBPATH=[root_dir + '/robot/2014/Lucy/librobot2014', root_dir + '/commun/2014'])
			env.Append(LIBS=['RobotLucy2014', 'Constantes2014'])

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

		elif lib == 'Webcam2013':
			env.Append(CPPPATH=[root_dir + '/webcam/2013/libWebcam2013'])
			env.Append(LIBPATH=[root_dir + '/webcam/2013/libWebcam2013'])
			env.Append(LIBS=['Webcam2013'])

		elif lib == 'glfw':
			env.Append(CPPPATH=[root_dir + '/simulateur/GLFW/include'])
			env.Append(LIBPATH=[root_dir + '/simulateur/GLFW/lib'])

			if sys.platform == 'win32':
				env.Append(LIBS=['glfw', 'opengl32', 'glu32'])
			elif sys.platform == 'darwin':
				env.Append(LIBS=['glfw', 'm', 'pthread'])
				env.Append(FRAMEWORKS=['Cocoa', 'OpenGL', 'GLUT', 'IOKit'])
			else:
				env.Append(LIBS=['glfw', 'GL', 'GLU', 'm', 'X11', 'pthread', 'Xrandr'])
				env.Append(LIBPATH=['/usr/X11R6/lib'])
				if ARGUMENTS.get('shadow_maps', 0):
					env.ParseConfig('pkg-config glew --libs --cflags')
					env.Append(CPPDEFINES=['USE_SHADOW_MAPS'])

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

			elif sys.platform == 'darwin' and str(ARGUMENTS.get('python')) == '1':
				env.Append(CPPPATH=['/usr/local/include'])
				env.Append(LIBPATH=['/usr/local/lib', '/opt/local/lib'])
				env.Append(LIBS=['boost_python-mt', 'vte'])
				env.ParseConfig('python-config --includes')
				env.ParseConfig('python-config --libs')
                    
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
		elif lib == 'libfreenect':
			if sys.platform == 'linux2':
				env.Append(CPPPATH=[root_dir + '/libfreenect/include'])
				env.Append(LIBPATH=[root_dir + '/libfreenect/src'])
				env.Append(LIBS=['freenect', 'usb-1.0', 'Xmu', 'Xi'])
		elif lib == 'libfreenect_sync':
			if sys.platform == 'linux2':
				env.Append(CPPPATH=[root_dir + '/libfreenect/include'])
				env.Append(CPPPATH=[root_dir + '/libfreenect/wrappers/c_sync'])
				env.Append(LIBPATH=[root_dir + '/libfreenect/wrappers/c_sync'])
				env.Append(LIBS=['freenect_sync'])
				
				# On depend de libfreenect
				env.Append(
				LIBPATH=[root_dir + '/libfreenect/src'])
				env.Append(LIBS=['freenect', 'usb-1.0', 'Xmu', 'Xi'])
		elif lib == 'freeglut':
			if sys.platform == 'linux2':
				env.Append(LIBS=['glut', 'GL', 'GLU', 'm', 'X11', 'pthread', 'Xrandr'])
				env.Append(LIBPATH=['/usr/X11R6/lib'])
	return env
