#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../../commun/Commun.h"
#include <IConstantes.h>

#include "../../commun/MathToolbox/MathToolbox.h"
#include "../../commun/Trame.h"
#include "../Cartes/Carte.h"
#include "../Cartes/CarteDeplacementCommun.h"
#include "Communication/CAN.h"
#include "IDebugDessin.h"
#include "Communication/NamedPipe.h"
#include "Communication/RS232.h"
#include "Communication/Serie.h"
#include "Communication/TCPIP.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <sockets/Socket.h>
#include <thread>
#include <unordered_map>

namespace Commun {

	class Strategie;

	enum class ModeConnexion { RS232, LOCAL, PIPES, OTHERS };

	class Robot {
	public:
		/*******************************/
		/********* CONSTANTES **********/
		/*******************************/
		// Définit le nombre de demande d'actualisation de la carte contacteur pour être surs que la tirette est ok
		static const int NB_RETRY_TIRETTE = 3;

		static std::vector<std::string> makeArgs(const char* typeConnexion, const char* adresse, int port);

		/// Initialise le robot a partir des arguments passes au programme.
		Robot(std::unique_ptr<ConstantesCommunes> constantesCommunes,
		      std::unique_ptr<ConstantesRobot> constantes,
		      std::vector<std::string> const& args,
		      Speed vitesseDeplacement,
		      AngularSpeed);

		virtual ~Robot();

		virtual CarteDeplacement& getCarteDeplacement() = 0;

		// getter pour le CAN -> utile pour réinitialiser le numéro de
		// séquence à la sychronisation avec l'élec
		CAN& getCAN();
		/// Debug du CAN pour les elecs
		void setDebugCAN(bool active);

		bool isSimuConnected();
		Socket& getSocketSimu();

		Speed getVitesseDeplacement();
		AngularSpeed getVitesseAngulaire();


		virtual void attenteTirette() = 0;

		/////////////////////////////////////
		// Implémentation de ICapteurs //
		/////////////////////////////////////
		/// Demande l'actualisation des coordonnées du robot
		virtual void actualiserCoordonnees() = 0;

		/// Vérifie que les coordonnées du robot soient actualisées
		virtual bool coordonneesActualisees() = 0;

		/// Retourne les coordonnées du robot telles qu'on les connait
		/// Exprimées dans le repère absolu
		virtual Coordonnees lireCoordonnees() = 0;

		/// Demande l'actualisation des coordonnées, attend qu'elle ait eu lieu et renvoie les nouvelles coordonnées.
		virtual Coordonnees actualiserEtLireCoordonnees() {
			this->actualiserCoordonnees();
			return this->lireCoordonnees();
		}

		// *** CarteEvitement2012 ***
		/** Retourne l'angle avec l'adversaire donné par le capteur
		 * d'évitement. L'adversaire est devant notre robot lorsque sa
		 * position angulaire est 0 degré (le robot pointe vers les x
		 * croissants) et à droite lorsque sa position angulaire est
		 * 90 degrés (sens horaire).
		 * Si aucun adversaire n'est détecté, retourne 0 radians.*/
		/*virtual Angle getAngleAdversaireDetecte() = 0;
		virtual void setEnvoiPositionAdversaireAuto(bool value) = 0;
		virtual Vector2m getPositionTourelle() const;*/

		// *** CarteEvitement2018 ***
		/**
		* Retourne vrai si l'adversaire est détecté par le capteur
		* d'évitement. On ne prends pas en compte l'angle tant que
		* que les élecs ne fournissent pas
		*/
		virtual bool isAdversaireDetecte() = 0;
		virtual Vector2m getPositionTourelle() const;

		///////////////////////////
		// Fonctions utilitaires //
		///////////////////////////
		/**
		 * Fonction permettant de modifier l'angle de détection de l'adversaire
		 * Attention: il faut donner la taille de la demi-zone
		 *
		 * ex: pour avoir un cone de +/- 0.25_PI, donner 0.25_PI
		 * default : +/- 0.25_PI
		 */
		void setAngleDetectionAdv(Angle angle) {
			if(abs(angle) <= 0.5_PI) {
				this->_angleDetectionAdv = angle.toMilliRad();
			} else {
				logWarn("Angle de détection de l'adversaire trop grand ! ", angle);
			}
		}

		virtual void desactivation() = 0;

		void setStrategie(Strategie& strategie) {
			_strategie = &strategie;
		}

		const Strategie& getStrategie() const {
			return *_strategie;
		}

		const ModeConnexion& getModeConnexion() const {
			return _modeConnexion;
		}

		ConstantesRobot const& constantes() const {
			return *_constantes;
		}
		ConstantesCommunes const& constantesCommunes() const {
			return *_constantesCommunes;
		}

		Carte& getCarteWithID(uint8_t id);

		void setAllPinged(bool pinged) {
			_allPinged = pinged;
		}

		bool didAllPinged() {
			return _allPinged;
		}

		int getAngleDetectionAdv() {
			return _angleDetectionAdv;
		}

	protected:
		std::unique_ptr<ConstantesCommunes> _constantesCommunes;
		std::unique_ptr<ConstantesRobot> _constantes;

		std::condition_variable _cartesInitialisees;
		std::atomic_bool _cartesInitNotified{false};
		/// Connexion à la main à partir des arguments passés au programme.
		void connecter(std::vector<std::string> const& args);

		void ajouterCarte(std::unique_ptr<Carte> carte);

		virtual void initialisation() = 0;
		std::atomic_bool _executionEnCours = {true};

		/// Liste des cartes inserees triées par ID
		std::unique_ptr<Carte> _cartes[Trame::NB_CARTES_MAX];

		/// Bus CAN à utiliser pour communiquer avec l'électronique
		std::unique_ptr<CAN> _busCAN;

		std::unique_ptr<Socket> _socketSimu;

		/// État du bus CAN de l'electronique
		bool _connecte;

		/// Mode de connexion du CAN : local / RS232
		ModeConnexion _modeConnexion;

		/// Indique si la connection au socket du simulateur est réussie
		bool _simuConnected;

		/// Thread de réception des messages
		std::thread _reception;

		std::mutex _mutexTraiterMessages;

		/// Envoie aux bonnes cartes les messages reçus pour traitement
		void traiterMessage();

		std::mutex _mutexVitesse;
		Speed _vitesseDeplacement;
		AngularSpeed _vitesseAngulaire;

		// Indexe les ID physique de carte par les ID virtuels (enum)
		std::unordered_map<uint32_t, uint8_t> _idMap;

		std::atomic_int _angleDetectionAdv = {250}; // mrad
		bool _allPinged;
		Strategie* _strategie = nullptr;
	};

	/// Interface de contrôle d'un robot générique à cartes modulaires sur bus CAN
	template <typename _Strategie, typename _IDCartes, template <_IDCartes> class _CarteInfo, typename Contacteurs>
	class RobotAnnee : public Robot {
	public:
		using Robot::Robot;

		~RobotAnnee() = default;

		/// Retourne la carte d'ID donne
		Carte& getCarte(_IDCartes id);
		/// Retourne true si le robot possède la carte passée en paramètres
		bool hasCarte(_IDCartes id);

		template <_IDCartes ID>
		auto& getCarte() {
			static_assert(std::is_same<std::underlying_type_t<decltype(_CarteInfo<ID>::idCarte)>, std::uint8_t>::value,
			              "Type de l'ID invalide.");
			static_assert(static_cast<std::uint8_t>(_CarteInfo<ID>::idCarte) < Trame::NB_CARTES_MAX,
			              "ID Carte trop grand !!");
			return static_cast<typename _CarteInfo<ID>::typeCarte&>(this->RobotAnnee::getCarteWithID(_CarteInfo<ID>::idCarte));
		}

		virtual CarteDeplacement& getCarteDeplacement() override {
			return this->getCarte<_IDCartes::DEPLACEMENT>();
		}

		/////////////////////////////////////
		// Implémentation de ICapteurs //
		/////////////////////////////////////
		/// Demande l'actualisation des coordonnées du robot
		virtual void actualiserCoordonnees() override { /*this->getCarteDeplacement().actualiserCoordonnees();*/
		}

		/// Vérifie que les coordonnées du robot soient actualisées
		virtual bool coordonneesActualisees() override {
			return true; /*return this->this->getCarteDeplacement().coordonneesActualisees();*/
		}

		/// Retourne les coordonnées du robot telles qu'on les connait
		virtual Coordonnees lireCoordonnees() override {
			return this->getCarteDeplacement().lireCoordonnees();
		}

		// Actualise les données de debug demandées
		void demanderMesure(typename _CarteInfo<_IDCartes::DEPLACEMENT>::typeCarte::TypeGrandeur grandeur) {
			this->getCarteDeplacement().demanderMesure(grandeur);
		}

		// Vérifie que les données de debug soient actualisées
		bool mesureActualisee() {
			return this->getCarte<_IDCartes::DEPLACEMENT>().mesureActualisee();
		}

		// Lit une donnée de debug
		std::int16_t lireMesure(typename _CarteInfo<_IDCartes::DEPLACEMENT>::typeCarte::TypeMesure type) {
			return this->getCarteDeplacement().lireMesure(type);
		}

		/*// *** CarteEvitement2012 ***
		virtual Angle getAngleAdversaireDetecte() override;
		virtual void setEnvoiPositionAdversaireAuto(bool value) override;*/


		// *** CarteEvitement2018 ***
		virtual bool isAdversaireDetecte() override;

		/// Tirette
		virtual void attenteTirette() override;

	protected:
		void synchroniserCAN();
		void pinguerCAN();
		bool pinguerCartes();
		// Attend que la tirette soit mise en place : bloquant
		void miseEnPlaceTirette();
		// Vérifie que la tirette est tirée (non bloquant)
		bool tiretteTiree();

		template <_IDCartes ID, typename... T>
		void assignerCarteHelper(T... params);
		virtual void assignerCartes() = 0;
	};
}

#include "Robot.hpp"

#endif
