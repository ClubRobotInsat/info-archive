import time
from os import system

pi = 3.14159265

# Renvoie le resultat du ping d'une carte
def Pinguer(carte):
	carte.envoyerPing()
	time.sleep(0.01) # sleep 10ms
	return carte.verifierPong()

# Renvoie le parametre d'une carte asservissement
def getParametre(carte, numParam, dt=0.01, boucle=False):
	carte.demanderParametre(numParam)
	time.sleep(dt)
	mes = carte.lireParametre()
	while(boucle):
		carte.demanderParametre(numParam)
		time.sleep(dt)
		mes = carte.lireParametre()
		print mes

	return mes

# Affiche l'evolution d'une grandeur au cours du temps
def PlotDataVsTime(function, duree=30, dt=0.1, fichier='data.txt'):
	# function : fonction sans argument qui renvoie une donnees qui evolue
	#            au cours du temps

	f = open(fichier, 'w')
	t = 0.0
	while(t < duree):
		data = function()
		line = str(t) + ' ' + str(data) + '\n'
		f.write(line)
		print(line)
		time.sleep(dt)
		t += dt

	f.flush()
	f.close()
	system('gnuplot -e "plot \'data.txt\'; pause -1"')

# Affiche l'evolution d'un parametre d'une carte asservissement au cours du temps
def PlotParamVsTime(carte, numParam, duree=30, dt=0.1, fichier='data.txt'):
	f = open(fichier, 'w')
	t = 0.0
	while(t < duree):
		data = getParametre(carte, numParam, dt)
		line = str(t) + ' ' + str(data) + '\n'
		f.write(line)
		print(line)
		#time.sleep(dt) # le sleep est fait dans getParametre
		t += dt

	f.flush()
	f.close()
	system('gnuplot -e "plot \'data.txt\'; pause -1"')

