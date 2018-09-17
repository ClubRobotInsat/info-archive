//
// Created by louis on 20/04/17.
//

#ifndef ROOT_EVITEMENT_H
#define ROOT_EVITEMENT_H

#include "../Robot.h"
#include "environment.h"

namespace Commun {
	class Evitement final {
	public:
		Evitement(Robot& robot, Environment& env);

		/** Définit la position du capteur de détection de l'adversaire.
		 * @param pos Le décalage du capteur par rapport au centre du
		 * robot. x caractérise le décalage vers l'avant, y le décalage vers
		 * la droite. */
		void setPositionCapteur(const Vector2m& pos);

		/** Retourne la position de l'adversaire déduite des informations
		 * données par le capteur d'évitement.
		 * Lorsqu'aucun adversaire n'est détecté, renvoie des coordonnées
		 * arbitraires. La méthode #adversaireDetecte() permet de savoir
		 * si un adversaire est détecté. */
		Coordonnees getPositionAdversaire();

		/** Retourne la position angulaire du robot adverse par rapport
		 * à notre robot. L'adversaire est devant notre robot lorsque sa
		 * position angulaire est 0 degré (le robot pointe vers les x
		 * croissants) et à droite lorsque sa position angulaire est
		 * 90 degrés (sens horaire).
		 * Si aucun adversaire n'est détecté, retourne 0 radians.*/
		Angle getPositionAngulaireAdversaire();

		/** @returns true si un adversaire est dans le champ de vision
		 * du robot, false sinon. */
		bool adversaireDetecte();

		/** @returns true si l'adversaire gêne potentiellement l'avance
		 * du robot dans le sens passé en paramètres. */
		bool adversairePresent(SensAvance sens);

		/** Modifie l'angle obtenu depuis le robot, pour prendre en compte
		 * le décalage de la tourelle. */
		Angle shiftAngle(Angle angle);

	private:
		Robot& _robot;
		Environment& _env;

		/** x : décalage vers l'avant du robot.
		 * y : décalage vers le côté droit du robot. */
		Vector2m _turretPosition;
	};
} // namespace Commun

#endif // ROOT_EVITEMENT_H
