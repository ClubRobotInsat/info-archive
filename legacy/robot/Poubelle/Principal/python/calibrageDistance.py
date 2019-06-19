from math import sqrt
from os import system

offset = 315.0 # distance initiale balise <-> catadioptre, en mm
timerAcquisition = 0.01 # en secondes

# Ouverture du fichier de sauvegarde
f = open('calib_data.dat', 'w')
t = 0.0
distanceParcourue = 0.0

# Coefficients de la fonction de fit ax**b
COEFF_FIT_MULT=16.761998
COEFF_FIT_POW=-1.468074

# Ordre de deplacement en ligne droite
dep.avancerInfini(70, SensAvance.SENS_ARRIERE)

while(distanceParcourue < 1000):
	dep.actualiserCoordonnees()
	coord = dep.lireCoordonnees()
	deplaX = coord.x # en mm
	deplaY = coord.y # en mm
	distanceParcourue = sqrt(deplaX**2 + deplaY**2)+offset # distance parcourue calculee par l'odometrie (en mm)
	angleCatadioptre = carteEvitement.getDiametreAngulaire() # diametre angulaire du catadioptre (en radian)
	if angleCatadioptre > 0.0:
		Distanceesuree = COEFF_FIT_MULT*angleCatadioptre**COEFF_FIT_POW
	else:
		Distanceesuree = 0.0
	line = 	  str(t) + ' ' \
		+ str(deplaX) + ' ' \
		+ str(deplaY) + ' ' \
		+ str(distanceParcourue) + ' ' \
		+ str(angleCatadioptre) + ' ' \
		+ str(Distanceesuree) + '\n'
	if(angleCatadioptre > 0.0):
		f.write(line)
		print(line)
	time.sleep(timerAcquisition)
	t += timerAcquisition

# Arret
dep.arreter()

# Ecriture sur disque et fermeture du fichier de sauvegarde
f.flush()
f.close()

# Lance la regression avec gnuplot
system('./regression.gnuplot')
