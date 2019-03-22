// Robot.h

#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <vector>

#include "Commun.h"
#include "cartes/Carte.h"
#include "cartes/ICoordonneesAdversaire.h"
#include "communication/Communicateur.h"
#include <IConstantes.h>

class RobotLogic : public ITrameListener {
public:
	class ErreurCarteDejaAjoutee : public std::runtime_error {
	public:
		ErreurCarteDejaAjoutee(unsigned char id)
		        : std::runtime_error("impossible d'ajouter la carte d'ID " + to_string((short)id) +
		                             " : cet ID est deja present dans le robot") {}
	};
	class ErreurIDCarteTropGrand : public std::runtime_error {
	public:
		ErreurIDCarteTropGrand(unsigned char id)
		        : std::runtime_error("impossible d'ajouter la carte d'ID " + to_string((short)id) + " : cet ID est trop grand") {}
	};

	// lors de la réception d'une trame la redistribuer vers la bonne carte
	// NB : fonction appelée par Communicateur::threadReception() (par le Communicateur membre de Robot)
	// retourne false si la trame n'est traitée par aucune carte
	virtual bool onRecu(Trame const&, bool isAck = false);

	// lors de l'émission d'une trame, la redistribuer vers le communicateur
	void onEnvoye(Trame const& t, bool isAck = false);

	Commun::ConstantesCommunes const& constantesCommunes() const {
		return *_constantesCommunes;
	}

	Commun::ConstantesRobot const& constantes() const {
		return *_constantes;
	}

protected:
	RobotLogic(std::unique_ptr<Commun::ConstantesCommunes> constantesCommunes, std::unique_ptr<Commun::ConstantesRobot> constantes)
	        : _constantesCommunes(std::move(constantesCommunes)), _constantes(std::move(constantes)) {}

	// Communicateur pour l'envoi et reception des trames
	std::unique_ptr<Communicateur> _communicateur;

	// tableau de cartes
	std::unique_ptr<Carte> _cartes[Trame::NB_CARTES_MAX];
	std::unique_ptr<Commun::ConstantesCommunes> _constantesCommunes;
	std::unique_ptr<Commun::ConstantesRobot> _constantes;
};

/**
 * Robot logic template est le template à de base à utiliser pour la logique du robot.
 * La logique du robot comprend :
 * 	- la réception des ordres de l'IA sous forme de trames
 * 	- la transmission de ces ordres aux cartes correspondantes
 */
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
class RobotLogicTemplate : public RobotLogic
// public ICoordonneesAdversaire,
// public IDeplacement
{
public:
	// L'argument 'connexion' est soit "", soit de la forme "RS232 /dev/ttyS0" ou "TCPIP localhost 1234"
	RobotLogicTemplate(std::unique_ptr<Commun::ConstantesCommunes> constantesCommunes,
	                   std::unique_ptr<Commun::ConstantesRobot> constantes,
	                   _CouleurEquipe couleur,
	                   const std::string& connexion = "");

	// destructeur
	virtual ~RobotLogicTemplate();

	// met à jour le robot en mettant à jour les cartes
	virtual void update(Duration elapsed);

	/*// ICoordonneesAdversaire
	virtual const Vector2m &getPositionAdversaire() const override;
	// Angle de l'adversaire
	virtual Angle getAngle2dAdversaire() const = 0;*/

	_CouleurEquipe getCouleur() const {
		return _couleur;
	}

	template <_IDCartes ID>
	auto& getCarte() {
		static_assert(std::is_same<std::underlying_type_t<decltype(_CarteInfo<ID>::idCarte)>, std::uint8_t>::value,
		              "Type de l'ID invalide.");
		static_assert(static_cast<std::uint8_t>(_CarteInfo<ID>::idCarte) < Trame::NB_CARTES_MAX, "ID Carte trop grand !!");
		return static_cast<typename _CarteInfo<ID>::typeCarte&>(*_cartes[(_CarteInfo<ID>::idCarte)]);
	}

protected:
	template <_IDCartes ID, typename... Params>
	void addCarteHelper(RobotLogicTemplate& r, Params&&... params) {
		auto carte =
		    std::make_unique<typename _CarteInfo<ID>::typeCarte>(_CarteInfo<ID>::idCarte, std::forward<Params>(params)...);
		r.add(std::move(carte));
	}

private:
	// ajouter une carte au robot
	void add(std::unique_ptr<Carte> carte);
	_CouleurEquipe const _couleur;
};

#include "RobotLogic.hpp"

#endif // ROBOT_H
