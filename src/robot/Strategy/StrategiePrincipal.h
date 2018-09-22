// Strategie2015.h

#ifndef STRATEGIE_PRINCIPAL_2015_H
#define STRATEGIE_PRINCIPAL_2015_H

#include "RobotPrincipal/Constantes.h"
#include "Strategie.h"
#include <memory>

class RobotPrincipal;
class MecaManagerPrincipal;

class StrategiePrincipal : public Commun::Strategie {
public:
	/*******************************/
	/********* CONSTANTES **********/
	/*******************************/

	static const Duration TEMPS_PRECISION_VIGIE_MAX;
	static const Duration TEMPS_DEPLACEMENT_MAX;

	static const int INCREMENT_BLOCAGE_LOGICIEL;
	static const int INCREMENT_BLOCAGE_PHYSIQUE;

	static const int NIVEAU_DANGER_LIGNE_DROITE;

	static const Angle TOLERANCE_ANGLE_TRAJECTOIRE_DEBUT;
	static const Angle TOLERANCE_ANGLE_EN_MOUVEMENT;
	static const Angle TOLERANCE_ANGLE_LINEARISATION;
	static const Angle ERREUR_ANGLE_TRAJECTOIRE_MAX;
	static const std::size_t DISTANCE_LINEARISATION_MIN;

	static const Duration DUREE_ATTENTE_ADVERSAIRE;
	// temps maximum entre la précision de fin de mouvement atteinte et le mouvement terminé
	static const Duration TIMEOUT_FIN_DEPLACEMENT;
	static const Duration TIMEOUT_MAX;
	static const Duration TIMEOUT_DEFAULT;


	// ///////////////////////////////
	// /////////// BASE  /////////////
	// ///////////////////////////////

	/// Initialisation de la strategie
	StrategiePrincipal(std::vector<std::string> const& args, Constantes::RobotColor const& color);
	~StrategiePrincipal();

	/// Placement du robot et définition des coordonnées avant le début du match
	virtual void recalageZoneDepart() override;

	/// Exécute la funny action à la fin du match
	virtual void funnyAction() override;

	/// Retourne vrai si l'adversaire est sur le chemin (ligne droite) menant à la cible xCible, yCible
	bool adversaireVersCible(Vector2m const& cible);

	MecaManagerPrincipal& getMecaManager() {
		return *_meca;
	}

	RobotPrincipal& getRobot();

	void setPoints(int);
	int getPoints() const;

protected:
	std::unique_ptr<MecaManagerPrincipal> _meca;

	std::atomic_int _nbr_points;
};

#endif
