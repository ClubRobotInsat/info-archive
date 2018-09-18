#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../../commun/Commun.h"
#include <IConstantes.h>

#include "MathToolbox/MathToolbox.h"
#include "MathToolbox/Repere.h"

#include "Modules/ModuleManager.h"

#include "Communication/ElecCommunicator.h"

/*#include "../../commun/communication/Trame.h"
#include "../Cartes/Carte.h"
#include "../Cartes/CarteDeplacementCommun.h"*/

#include "IDebugDessin.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace Commun {

	// class Strategie;


	class Robot {
	public:
		/*******************************/
		/********* CONSTANTES **********/
		/*******************************/
		// TODO : déplacer ce code dans la partie 'stratégie'
		// Définit le nombre de demande d'actualisation de la carte contacteur pour être surs que la tirette est ok
		// static const int NB_RETRY_TIRETTE = 3;

		/// Initialise le robot a partir des arguments passes au programme.
		Robot(std::shared_ptr<ModuleManager> module_manager,
		      std::unique_ptr<ConstantesCommunes> constantesCommunes,
		      std::unique_ptr<ConstantesRobot> constantes,
		      std::vector<std::string> const& args);

		virtual ~Robot();

		template <typename Module>
		Module& get_module() const {
			return _module_manager->get_module<Module>();
		}

		Moving2019& get_module_move() const;

		repere::Coordonnees read_coordinates() const {
			return get_module_move().get_coordonnees();
		}

		// TODO : déplacer ce code dans la partie 'stratégie'
		/// Attend la tirette au départ
		// void wait_for_tirette() const;

		///////////////////////////
		// Fonctions utilitaires //
		///////////////////////////


		// virtual void deactivation() = 0;

		/*void setStrategie(Strategie& strategie) {
		    _strategie = &strategie;
		}*/

		/*const & getStrategie() const {
		    return *_strategie;
		}*/

		ConstantesRobot const& constantes() const {
			return *_constantes;
		}

		ConstantesCommunes const& constantesCommunes() const {
			return *_constantesCommunes;
		}

		ElecCommunicator& get_communicator() {
			return *_communicator;
		}

	protected:
		std::shared_ptr<ModuleManager> _module_manager;
		std::unique_ptr<ConstantesCommunes> _constantesCommunes;
		std::unique_ptr<ConstantesRobot> _constantes;

		// virtual void initialize() = 0;

		// virtual void assign_modules() = 0;
		// Strategie* _strategie = nullptr;

	private:
		std::unique_ptr<ElecCommunicator> _communicator;

		/*void setting_up_tirette() const;
		bool is_tirette_pulled() const;*/
	};

	/**
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
	    virtual void actualiserCoordonnees() override {
	        this->getCarteDeplacement().actualiserCoordonnees();
	    }

	    /// Vérifie que les coordonnées du robot soient actualisées
	    virtual bool coordonneesActualisees() override {
	        return this->getCarteDeplacement().coordonneesActualisees();
	    }

	    /// Retourne les coordonnées du robot telles qu'on les connait
	    virtual Coordonnees read_coordinates() override {
	        return this->getCarteDeplacement().read_coordinates();
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
	};*/
} // namespace Commun

//#include "Robot.hpp"

#endif
