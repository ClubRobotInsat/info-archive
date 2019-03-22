/************************************************************
 * CarteAsservissement2009.h *
 * Spécifie la classe gérant l'asservissement 2008/2009 *
 ************************************************************/

#ifndef _CARTE_ASSERVISSEMENT_2009_H_
#define _CARTE_ASSERVISSEMENT_2009_H_

#include "CarteAsservissement.h"
#include <atomic>

class CarteAsservissement2009 : public CarteAsservissement {
public:
	CarteAsservissement2009(Commun::CAN& can, uint8_t id);

	void traiterMessage(Trame const& message) override;

	// Fait redemarrer la carte
	void reset() override;

	// Actualise l'angle actuel du moteur
	void actualiserAngle() override;

	// Verifie que l'angle actualise est arrive
	bool angleActualise() override;

	// Retourne l'angle du moteur
	Angle lireAngle() override;

	// Definit l'angle actuel du moteur
	void definirAngle(Angle angle) override;

	// Indique si le déplacement est terminé ou pas
	bool verifierDeplacementTermine() override;

	// Indique si la précision autour de la cible a été atteinte ou non
	bool verifierPrecisionAtteinte() override;

	// Indique si le moteur est bloqué ou pas.
	bool verifierBlocage() override;

	// S'arrêter aux coordonnées actuelles
	void arreter() override;

	// Tourner de l'angle donné
	void tourner(Angle angle) override;

	// Tourner jusqu'à avoir l'angle absolu donné
	void tournerAbsolu(Angle angle) override;

	// Tourner à vitesse constante jusqu'à nouvel ordre
	void tournerInfini(AngularSpeed vitesse, SensRotation sens) override;

	// Arret d'urgence
	void arretUrgence(void) override;

	// Actualise les données de debug demandées
	void demanderMesure() override;

	// Vérifie que les données de debug soient actualisées
	bool mesureActualisee() override;

	// Lit une donnée de debug
	short lireMesure(TypeMesure type) override;

	// Allume ou éteint la LED témoin
	void allumerLed(bool led1, bool led2) override;

	// Permet d'activer ou de désactiver l'asservissement en angle
	void activerAsservissementAngle(bool active) override;

	// Vérifie qu'un ack ait été reçu
	TypeAck ackRecu() override;

	// Active l'envoi automatique des coordonnées du robot
	void activerEnvoiAuto(bool on_off) override;

	// Définit la variable 'param' (voir les define dans le .h) à la valeur 'val'
	void reglerParametre(uint8_t param, float val) override;

	// Demande à la carte d'envoyer la valeur d'un parametre
	void demanderParametre(uint8_t param) override;

	// Signifie si le dernier parametre demande a ete recu ou pas
	bool parametreActualise() override;

	// Retourne la valeur du dernier parametre recu
	float lireParametre() override;

private:
	// bool _pongRecu;

	std::mutex _mutexAngle;
	Angle _angle;

	std::atomic_bool _angleOk;

	std::atomic_bool _bloque;
	TypeDeplacement _deplacement;
	std::atomic_bool _deplacementTermine;
	std::atomic_bool _precisionAtteinte;
	std::atomic_short _mesure[4];
	std::atomic_bool _mesureOk;

	std::atomic<float> _parametre;
	std::atomic_bool _parametreOk;

	std::atomic<TypeAck> _ack;
};

#endif
