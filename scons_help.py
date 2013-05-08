#!/usr/bin/env python
# coding=utf8
# scons_help.py - inclus par scons_tools.py et par robot_builder.py
# Ce fichier définit les options que l'on peut passer à SCons

sconsHelp = [
	#['mingw',     'Utiliser le compilateur MinGW a la place de VC++. Utile si vous avez l\'erreur \'cl\' n\'est pas reconnu en tant que commande interne\'.'],
	['debug',       'Definit DEBUG et rajoute l\'option -g'],
	['python',      'Ajoute le support Python'],
	['gtk',         'Le simulateur utilise la fenetre de debug en GTK (sous Linux)'],
	['nobuiltin',   'Utiliser les librairies deja installees sur le systeme pour le simulateur'],
	['libv4l1',     'Utiliser la libv4l1 pour la webcam sous Linux (encapsule V4L2 dans des appels V4L1-style)'],
	['zbar',        'Utiliser la librairie ZBar (sous Linux) pour lire des codes barre avec la webcam'],
	['shadow_maps', 'Ajouter le support des shadow maps dans le simu (nécessite GLEW)'],
	['fakenect',    'Utiliser FakeNect a la place de libfreenect (il faut remplir la variable d\'environnement FAKENECT_PATH pour preciser ou se trouve le dump)']
]

if __name__ == "__main__":
	for line in sconsHelp:
		print line[0], ':', line[1]
