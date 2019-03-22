//
//  Deplacement.h
//  Club Robot
//
//  Created by Rémi on 18/12/2014.
//

#ifndef __Club_Robot__Deplacement__
#define __Club_Robot__Deplacement__

#include "Commun.h"
//#include "../Cartes/CarteDeplacementCommun.h"
#include "../../../../src/robot/Robot.h"
#include "Evitement.h"
#include "environment.h"
#include <list>
#include <memory>
#include <stack>
#include <threadpool/ThreadPool.h>

// TODO : À REFAIRE ENTIÈREMENT
namespace Commun {

	// Tous les déplacements proposés ici sont bloquants
	class Deplacement final {
	public:
		using interfaced_type = Moving2019;

		Deplacement(Robot& robot, Environment& env, Evitement& evitement);
		virtual ~Deplacement() = default;

		/*
		 * États divers et variés
		 */

		Vector2m getOriginPosition();

		Angle getOriginAngle();

		bool getEvitementState() {
			return this->peekStack(_evitementState);
		}
		void pushEvitementState(bool suivi) {
			this->pushStack(_evitementState, suivi);
			this->updateEvitement();
		}
		void popEvitementState() {
			this->popStack(_evitementState);
			this->updateEvitement();
		}

		Speed getVitesseLineaire() {
			return this->peekStack(_vitesseLineaire);
		}
		void pushVitesseLineaire(Speed vitesse) {
			this->pushStack(_vitesseLineaire, vitesse);
			this->updateVitesseLineaire();
		}
		void popVitesseLineaire() {
			this->popStack(_vitesseLineaire);
			this->updateVitesseLineaire();
		}

		AngularSpeed getVitesseAngulaire() {
			return this->peekStack(_vitesseAngulaire);
		}
		void pushVitesseAngulaire(AngularSpeed vitesse) {
			this->pushStack(_vitesseAngulaire, vitesse);
			this->updateVitesseAngulaire();
		}
		void popVitesseAngulaire() {
			this->popStack(_vitesseAngulaire);
			this->updateVitesseAngulaire();
		}

		Distance getPrecisionLineaire() {
			return this->peekStack(_precisionLineaire);
		}
		void pushPrecisionLineaire(Distance precision) {
			this->pushStack(_precisionLineaire, precision);
			this->updatePrecisionLineaire();
		}
		void popPrecionLineaire() {
			this->popStack(_precisionLineaire);
			this->updatePrecisionLineaire();
		}

		Angle getPrecisionAngulaire() {
			return this->peekStack(_precisionAngulaire);
		}
		void pushPrecisionAngulaire(Angle precision) {
			this->pushStack(_precisionAngulaire, precision);
			this->updatePrecisionAngulaire();
		}
		void popPrecionAngulaire() {
			this->popStack(_precisionAngulaire);
			this->updatePrecisionAngulaire();
		}

		/*
		 * Actions tout aussi diverses et variées
		 */

		ResultatAction allerA(Vector2m const& destination, SensAvance sens) {
			return this->allerA(destination, sens, 25_s);
		}
		ResultatAction allerA(Vector2m const& destination, SensAvance sens, Duration timeout) {
			return ResultatAction::RATE;
		}

		ResultatAction allerA(Vector2m const& destination) {
			return this->allerA(destination, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction allerA(Vector2m const& destination, Duration timeout) {
			return this->allerA(destination, SensAvance::Avant, timeout);
		}

		ResultatAction avancer(Distance distance, SensAvance sens) {
			return this->avancer(distance, sens, _robot.constantes().getTimeoutMoveDefault());
		}
		/** Bloquant */
		ResultatAction avancer(Distance distance, SensAvance sens, Duration timeout) {
			return ResultatAction::RATE;
		}
		ResultatAction avancerInfini(SensAvance sens) {
			return this->avancerInfini(sens, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction avancerInfini(SensAvance sens, Duration timeout) {
			return ResultatAction::RATE;
		}
		// ResultatAction avancerA(Vector2m position, SensAvance sens);
		ResultatAction avancerDe(Distance distance, SensAvance sens, Duration timeout) {
			return ResultatAction::RATE;
		}
		ResultatAction avancerDe(Distance distance, SensAvance sens) {
			return this->avancerDe(distance, sens, 5_s);
		}

		ResultatAction pointerVers(Distance x, Distance y) {
			return this->pointerVers(x, y, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction pointerVers(Distance x, Distance y, Duration timeout) {
			return ResultatAction::RATE;
		}
		ResultatAction pointerVers(Distance x, Distance y, SensRotation sens) {
			return this->pointerVers(x, y, sens, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction pointerVers(Distance x, Distance y, SensRotation sens, Duration timeout) {
			return ResultatAction::RATE;
		}
		/**
		 * Tourne selon le sens le plus rapide qui permet d'orienter le robot à l'angle absolu donné.
		 */
		ResultatAction tournerAbsolu(Angle angle) {
			return this->tournerAbsolu(angle, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction tournerAbsolu(Angle angle, Duration timeout) {
			return ResultatAction::RATE;
		}

		/**
		 * Tourne selon le sens spécifié qui permet d'orienter le robot à l'angle absolu donné.
		 */
		ResultatAction tournerAbsoluOriente(Angle angle, SensRotation sens) {
			return this->tournerAbsoluOriente(angle, sens, 2_s);
		}
		ResultatAction tournerAbsoluOriente(Angle angle, SensRotation sens, Duration timeout) {
			return ResultatAction::RATE;
		}

		ResultatAction tournerRelatif(Angle angle) {
			return this->tournerRelatif(angle, 2_s);
		}
		ResultatAction tournerRelatif(Angle angle, Duration timeout) {
			return ResultatAction::RATE;
		}
		ResultatAction tournerInfini(SensRotation sens) {
			return this->tournerInfini(sens, _robot.constantes().getTimeoutMoveDefault());
		}
		ResultatAction tournerInfini(SensRotation sens, Duration timeout) {
			return ResultatAction::RATE;
		}

		void arreter() {}
		void arretUrgence() {}

		/**
		 * Retourne l'angle absolu vers lequel doit s'orienter le robot pour être capable de se rendre en ligne droite
		 * vers la cibe.
		 */
		Angle deltaCap(Vector2m const& cible) {
			return 0_deg;
		}

		/**
		 * Définit le repère sur lequel sont basés les allerA, tournerAbsolu etc.
		 * Attention, pour permettre à la carte de rester dans un état valide, cet appel arrête tout mouvement en cours.
		 * En même temps, faut être logique et changer le repère quand le robot est en mouvement n'est pas des plus
		 * pertinent ;)
		 */
		void setRepere(Vector2m const& origine, Angle angle) {}

		void setRepere(Coordonnees const& newCoords) {}

		/**
		 * Fonction relative à l'affichage de la trajectoire en débug
		 */
		void setDebugState(bool state);

		SensRotation sensRotationOptimal(Orientation from, Orientation to) {
			return SensRotation::Horaire;
		}

	private:
		template <typename T>
		T peekStack(std::stack<T>& stack) {
			std::lock_guard<std::mutex> lk(_mutexEtats);
			return stack.top();
		}

		template <typename T>
		void pushStack(std::stack<T>& stack, T value) {
			std::lock_guard<std::mutex> lk(_mutexEtats);
			if(stack.size() > 1000) {
				logError("Trop de valeurs dans la stack !");
				throw std::runtime_error("Trop de valeurs dans la stack !");
			}
			stack.push(value);
		}

		template <typename T>
		void popStack(std::stack<T>& stack) {
			std::lock_guard<std::mutex> lk(_mutexEtats);
			if(stack.size() == 1) {
				logError("Tentative de suppression en trop dans la stack !");
				throw std::runtime_error("Stack vidée !");
			}
			stack.pop();
		}

		struct ComposanteTrajectoire {
			ComposanteTrajectoire(const Coordonnees& coord, SensAvance sens) : coordonnees(coord), sens(sens) {}

			Coordonnees coordonnees;
			SensAvance sens;
		};

		using Trajectoire = std::list<ComposanteTrajectoire>;

		void updateEvitement();
		void updateVitesseLineaire() {}
		void updateVitesseAngulaire() {}
		void updatePrecisionLineaire() {}
		void updatePrecisionAngulaire() {}

		std::function<bool()> testDeplacementUnitaireTermine();
		std::function<bool()> testDeplacementUnitairePrecisionAtteinte();

		ResultatAction attendreFinTrajectoire(std::function<bool()> const& conditionFinTrajectoire,
		                                      TimePoint timeoutDate,
		                                      bool arret = true,
		                                      bool checkAdversaire = true,
		                                      SensAvance sensAvance = SensAvance::Avant);

		ResultatAction suivreTrajectoire(TimePoint timeoutDate);
		ResultatAction allerATrajectoire(ComposanteTrajectoire const& nextPoint, bool finTrajectoire, TimePoint timeoutDate);

		Trajectoire calculerTrajectoire(Coordonnees const& destination, SensAvance sens, bool suiviAdversaire);

		ResultatAction setTrajectoire(Trajectoire&& trajectoire, TimePoint const& timeoutDate);

		bool verifierBlocage(size_t& compteurBlocages, size_t min);

		void afficherTrajectoire(Vector2m departReel, Vector2m destinationReelle, std::vector<Vector2m>& trajectoire);

		/** Cette fonction contient la logique du déplacement, et permet d'éviter un appel récursif sans
		 * fin dans le cas où on est bloqué par un adversaire devant et un adversaire derrière (improbable
		 * mais possible) */
		ResultatAction allerAInternal(Coordonnees const& destination, SensAvance sens, bool escape, Duration timeout);
		/** Propose une position de repli en cas de bloquage par l'adversaire. Choisit un point possible
		 * parmi un nombre fini de possibilités autour de la position présente du robot */
		Distance computeBackupDistance(SensAvance sensRobot, Distance distance);

		enum class CarteAction { Action, Lecture };

		std::unique_lock<std::recursive_timed_mutex> tryGetLockForAction(CarteAction action) {
			auto& mut = action == CarteAction::Action ? _mutexCarteAction : _mutexCarteLecture;
			auto lock = std::unique_lock<std::recursive_timed_mutex>(mut, std::defer_lock);
			lock.try_lock();
			return lock;
		}

		std::unique_lock<std::recursive_timed_mutex>
		    getLockForAction(CarteAction action, TimePoint const& tryUntil = TimePoint::distantFuture()) {
			auto& mut = action == CarteAction::Action ? _mutexCarteAction : _mutexCarteLecture;
			return std::unique_lock<std::recursive_timed_mutex>(mut, tryUntil.value());
		}

		std::unique_lock<std::recursive_timed_mutex> getLockForAction(CarteAction action, std::defer_lock_t) {
			auto& mut = action == CarteAction::Action ? _mutexCarteAction : _mutexCarteLecture;
			return std::unique_lock<std::recursive_timed_mutex>(mut, std::defer_lock);
		}

		Robot& _robot;
		Evitement& _evitement;
		Commun::Moving2019& _module;

		std::mutex _mutexEtats;
		std::stack<bool> _evitementState;
		std::stack<Speed> _vitesseLineaire;
		std::stack<AngularSpeed> _vitesseAngulaire;
		std::stack<Distance> _precisionLineaire;
		std::stack<Angle> _precisionAngulaire;

		std::recursive_timed_mutex _mutexCarteAction, _mutexCarteLecture;
		Environment& _env;
		Trajectoire _trajectoire;
		Coordonnees _dernierePositionAdversaire;
		bool _advPositionConnue = false;
		ThreadPool::ThreadPool<ResultatAction> _suiviTrajectoire;
		std::mutex _mutexTrajectoire;
		std::atomic_bool _isDebugActive = {true};

		Vector2m _originPosition;
		Angle _originAngle;
	};
} // namespace Commun

#endif /* defined(__Club_Robot__Deplacement__) */
