// ICapteurs.h

#ifndef ICAPTEURS_H
#define ICAPTEURS_H

#include "../Cartes/CarteDeplacementCommun.h"

namespace Commun {

	/// Interface implémentée par Robot2015.
	/// Elle est utilisée pour être passée en paramètre à Environnement, lequel ne doit pouvoir
	/// avoir accès qu'aux informations des capteurs, et non aux actionneurs.
	class ICapteurs {
	public:
		/// Actualise les coordonnées du robot
		virtual void actualiserCoordonnees() = 0;

		/// Vérifie que les coordonnées du robot soient actualisées
		virtual bool coordonneesActualisees() = 0;

		/// Retourne les coordonnées du robot
		virtual Coordonnees lireCoordonnees() = 0;

		/// Demande l'actualisation des coordonnées, attend qu'elle ait eu lieu et renvoie les nouvelles coordonnées.
		virtual Coordonnees actualiserEtLireCoordonnees() = 0;
	};
} // namespace Commun

#endif
