/* TP1.c */
/* Fichier à compléter pour votre premier programme */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	// Affichage du message de bienvenue
	printf("Je m'appelle Igor, je suis votre mousse et je vais nettoyer le pont!\n");

	// Initialisation du nombre de planches
	int nombre_de_planches = 50;
	// Affichage de la valeur de la variable
	printf("Qu'il est grand ce pont! Il ne compte pas moins de %d planches !\n", nombre_de_planches);

	// Boucle while avec décrémentation
	while(nombre_de_planches > 0) {
		nombre_de_planches--;
		printf("Plus que %d planches ! ça avance !\n", nombre_de_planches);
		// Toutes les 10 planches :
		if(nombre_de_planches % 10 == 0)
			printf("Encore %d planches !\n", nombre_de_planches);
		// Au milieu
		if(nombre_de_planches == 25)
			printf("Ce n'est que la moitié? Mais c'est long ! Il me reste encore %d planches !\n", nombre_de_planches);
	}
	// Message de fin
	printf("Ah c'était long, mais c'est fini maintenant !\n");

	return EXIT_SUCCESS;
}
