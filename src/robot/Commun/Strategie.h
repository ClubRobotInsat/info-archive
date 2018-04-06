/************************************************************
 * Strategie.h *
 * Classe de stratégie de match à durée limitée *
 * Version 0.1 - Guillaume Delamare 15/11/08 *
 ************************************************************/

#ifndef _STRATEGIE_H_
#define _STRATEGIE_H_

#include "../../commun/Commun.h"
#include "../../commun/RobotPrincipal/Constantes.h"
#include "Deplacement.h"
#include "Environnement/environment.h"
#include "Evitement.h"
#include "Robot.h"
#include <algorithm>
#include <memory>
#include <thread>

enum class EtatDeplacement { DEBUT, EN_COURS, TERMINE, BLOQUE };

namespace Commun {
	/// Stratégie de match à durée limitée
	class Strategie {
	public:
		Constantes::RobotColor getCouleurEquipe() const {
			return _couleurEquipe;
		}

		const repere::Repere& getReference() const {
			return _couleurEquipe == Constantes::RobotColor::Orange ? ConstantesPrincipal::REFERENCE_ORANGE :
			                                                          ConstantesPrincipal::REFERENCE_GREEN;
		}

		/// Démarre la stratégie pour un match d'une durée donnée (en millisecondes)
		void demarrer(Duration duree);

		/// Contient la logique de la stratégie elle-même
		virtual void executer() = 0;
		// Exécute la funny action à la fin du match
		virtual void funnyAction();
		/// Arrête tous les actionneurs pour la fin du match
		virtual void arreter();

		/// Renvoie le temps restant avant la fin du match
		Duration getTempsRestant() const;

		/// Renvoie le temps depuis le debut du match
		Duration getTempsDepuisDebut() const;

		// Placement du robot et définition des coordonnées avant le début du match
		virtual void recalageZoneDepart() = 0;

		/// Remet le compteur de match à zéro
		void resetTimer();

		void attendreTirette();
		/*
		Robot& getRobot() {
		    return *_robot;
		}
		 */
		Deplacement& getDeplacement() {
			return *_dep;
		}
		Environment& getEnvironnement() {
			return *_env;
		}
		Evitement& getEvitement() {
			return *_evitement;
		}

	protected:
		/// Initialise la stratégie
		Strategie(std::unique_ptr<Robot> robot, Constantes::RobotColor color);

		/// Obligatoire pour une gestion correcte de la suppression d'instances polymorphes des classes dérivées
		virtual ~Strategie() = default;

		/// Thread dans lequel s'exécute la stratégie
		std::thread _execution;
		std::unique_ptr<Robot> _robot;
		std::unique_ptr<Deplacement> _dep;
		std::unique_ptr<Environment> _env;
		std::unique_ptr<Evitement> _evitement;

		void threadLidar();

		/// Pointeurs vers l'environnement et le meca manager de l'année adéquate
		// std::unique_ptr<Environnement2015> _env;
		void creerEnvironement();

		/**
		 * @return Une liste de paires <position, jeSuisAmi>. Le booléen à true veut dire que c'est un robot ami.
		 */
		std::list<std::pair<Vector2m, bool>> tousLesRobots();

	private:
		std::mutex _robotsMutex;
		std::list<std::pair<Vector2m, bool>> _tousLesRobots;
		Constantes::RobotColor _couleurEquipe;

		std::thread _comLidar;

		/// Enregistre la durée totale du match
		Duration _dureeTotaleMatch = 0_s;

		/// Chronomètre pour connaître le temps depuis le début du match
		StopWatch _chronoMatch;

		virtual void exec() final;
	};
}

#endif
