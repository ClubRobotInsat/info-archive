/************************************************************
 * CarteDeplacement2009.h *
 * Spécifie la classe gérant le déplacement 2008/2009 *
 ************************************************************/

#ifndef _CARTE_DEPLACEMENT_2009_H_
#define _CARTE_DEPLACEMENT_2009_H_

#include "Carte.h"
#include "CarteDeplacementCommun.h"
#include "ConstantesCommunes.h"

ENUM_CLASS(CarteDeplacement2009Action, Action, Lecture)

class CarteDeplacement2009 : public CarteDeplacement {
public:
	CarteDeplacement2009(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Fait redemarrer la carte
	virtual void reset() override;

	// Actualise les coordonnées du robot
	// void actualiserCoordonnees();

	// Vérifie que les coordonnées du robot soient actualisées
	// bool coordonneesActualisees();

	// Retourne les coordonnées du robot
	virtual Coordonnees lireCoordonnees() override;

	// Retourne la période d'actualisation des coordonnées
	// Cette donnée est valide seulement si l'envoi automatique est activé.
	virtual Duration getPeriodeEnvoiAuto() override;

	// Définit les coordonnées actuelles du robot
	virtual void definirCoordonnees(const Coordonnees& pos) override;

	virtual const Repere& getReference() override {
		return REFERENCE;
	}

	// S'arrêter aux coordonnées actuelles
	virtual void arreter() override;

	// Avancer de la distance donnée.
	// La distance peut être positive ou négative.
	virtual void avancer(Distance distance) override;

	// Tourner de l'angle donné
	virtual void tourner(Angle angle) override;

	// Tourner jusqu'à avoir l'angle absolu donné
	virtual void tournerAbsolu(Angle, SensRotation sens) override;

	// Se tourner vers le point donné
	virtual void pointerVers(const Coordonnees& coords, SensRotation sens) override;

	// Se rend au point donné par une composition de rotation et d'avance, imprécis => déprécié
	[[deprecated]] virtual void vaVers(Distance x, Distance y, SensAvance sens) override;

	// Se rendre au point donné par une composition de rotation et d'avance
	virtual void allerADecompose(Distance x, Distance y, SensAvance sens) override;

	// Se rendre au point donné par une trajectoire lissée
	/**
	 * //////////////////// ! \\\\\\\\\\\\\\\\\\\\\\
	 * NE PAS UTILISER
	 */
	[[deprecated]] virtual void allerA(Distance x, Distance y, SensAvance sens) override;

	// Avancer à vitesse constante jusqu'à nouvel ordre
	virtual void avancerInfini(Speed vitesse, SensAvance sens) override;

	// Tourner à vitesse constante jusqu'à nouvel ordre
	virtual void tournerInfini(AngularSpeed vitesse, SensRotation sens) override;

	// Se rendre au point donné par une trajectoire lissée sans s'y arrêter
	virtual void passerPar(Coordonnees coords, Speed vitesse, SensAvance sens) override;

	// Pivote de l'angle donné autour de la roue droite
	virtual void pivoterDroite(Angle angle, SensRotation sens) override;

	// Pivote de l'angle donné autour de la roue gauche
	void pivoterGauche(Angle angle, SensRotation sens) override;

	// Fait avancer chacune des 2 roues d'une distance donnée
	virtual void positionDroiteGauche(Distance droite, Distance gauche, SensAvance sens) override;

	// changer la vitesse individuelle des 2 roues
	virtual void vitesseDroiteGauche(Speed vitesse_d, Speed vitesse_g, SensAvance sens) override;

	// Fait avancer chacune des 2 roues d'une distance donnée
	virtual void arretUrgence() override;

	// Permet de contrôler en direct la vitesse longitudinale et angulaire du robot
	virtual void controleVitesseLongAng(Speed vitesse_longitudinale, AngularSpeed vitesse_angulaire) override;

	// Actualise les données de debug demandées
	virtual void demanderMesure(TypeGrandeur grandeur) override;

	// Vérifie que les données de debug soient actualisées
	virtual bool mesureActualisee() override;

	// Lit une donnée de debug
	virtual std::int16_t lireMesure(TypeMesure type) override;

	// Allume ou éteint la LED témoin
	virtual void allumerLed(bool led1, bool led2) override;

	// Permet d'activer ou de désactiver certains types d'asservissement (distance, angle, roue droite, roue gauche)
	virtual void activerAsservissement(TypeGrandeur type, bool active) override;

	// Vérifie qu'un ack ait été reçu
	virtual TypeAck ackRecu() override;

	// Active l'envoi automatique des coordonnées du robot
	virtual void activerEnvoiAuto(bool on_off) override;

	// Définit la variable 'param' (voir les define dans le .h) à la valeur 'val'
	virtual void reglerParametre(uint8_t param, float val) override;

	// Demande à la carte d'envoyer la valeur d'un paramètre
	virtual void demanderParametre(uint8_t param) override;

	// Signifie si le dernier parametre demande a été reçu ou pas
	virtual bool parametreActualise() override;

	// Retourne la valeur du dernier parametre recu
	virtual float lireParametre() override;

	// Indique si le robot est bloqué depuis le dernier appel de cette fonction.
	// ATTENTION, ne représente pas l'état de blocage instantané !!
	virtual bool verifierNouveauBlocage() override;

	// ********************
	// ********************
	// *****ATTENTION******
	// Fonctions dépréciées
	// Ne remplissent pas leur rôle correctement
	// Indique si le déplacement est terminé ou pas (robot arrêté et précision atteinte autour de la cible)
	// XXX NB : Les fonctions ne remplissent-elles pas leur rôle, ou est-ce qu'on ne les utilise pas correctement ?
	// -> utilisation pour les déplacements unitaires (avancer, tourner) peut être plus précise que de se reposer sur
	// l'actualisation des coordonnées du robot. Test en cours.
	virtual bool verifierDeplacementTermine() override;
	// Indique si la précision autour de la cible a été atteinte ou non (n'attend pas que le robot se soit immobilisé)
	virtual bool verifierPrecisionAtteinte() override;

private:
	static constexpr const Repere& REFERENCE = ABSOLUTE_REFERENCE;

	Coordonnees _coords;
	std::atomic_bool _coordsOk;

	// Chaque thread a sa propre valeur de blocage afin d'éviter l'écrasement de la valeur par un thread concurrent
	std::atomic_bool _bloque;
	TypeDeplacement _deplacement;
	std::atomic_bool _deplacementTermine;
	std::atomic_bool _precisionAtteinte;
	std::atomic<std::int16_t> _mesure[4];
	std::atomic_bool _mesureOk;

	std::atomic<float> _parametre;
	std::atomic_bool _parametreOk;

	std::atomic<TypeAck> _ack;

	// Reinitialise la valeur de blocage pour tous les threads
	void setBloque(bool bloque);

	void setDeplacementTermine(bool bloque);
	void setPrecisionAtteinte(bool bloque);
};

#endif
