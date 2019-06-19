/************************************************************
 * CarteColorimetres2008.cpp *
 * Implémente la classe gérant les colorimètres 2007/2008 *
 ************************************************************/

#include "CarteColorimetres2008.h"

CarteColorimetres2008::CarteColorimetres2008(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_capteurs, 0, sizeof(_capteurs));
	for(int i = 0; i < NOMBRE_DE_CAPTEURS; i++)
		_ack[i] = false;
}

void CarteColorimetres2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x02_b:
		case 0x01_b: // Reponse etat _capteurs
			if(message.getNbDonnees() == 2 * NOMBRE_DE_COULEURS + 1) {
				int numero_capteur = message.getCmd() - 1;
				bloquerVariables();
				for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++) {
					_capteurs[numero_capteur][couleur] = message.getDonnee<uint16_t>(2 * couleur + 1);
				}
				_ack[numero_capteur] = true;
				debloquerVariables();
			} else
				printf("[ATTENTION] Carte 0x%x : message 0x%x : longueur de trame incorrecte\n", getId(), message.getCmd());
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Demande une actualisation des etats des colorimetres
void CarteColorimetres2008::actualiserCapteur(uint8_t capteur) {
	if(capteur >= NOMBRE_DE_CAPTEURS)
		// TODO lever erreur
		return;

	_ack[capteur] = false;

	envoyerMessage(capteur + 1, capteur);
}

// Demande si le capteur a bien ete actualise
bool CarteColorimetres2008::capteurActualise(uint8_t capteur) {
	if(capteur >= NOMBRE_DE_CAPTEURS)
		// TODO lever erreur
		return false;

	return _ack[capteur];
}

// Retourne l'etat d'un des colorimetres
void CarteColorimetres2008::getCapteur(uint8_t capteur, uint16_t* rgb) {
	if(capteur >= NOMBRE_DE_CAPTEURS)
		// TODO lever erreur
		return;

	bloquerVariables();
	for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++)
		rgb[couleur] = _capteurs[capteur][couleur];
	debloquerVariables();
}

// accesseur sur une des couleurs d'un des colorimetres
uint16_t CarteColorimetres2008::getCapteur(uint8_t capteur, uint8_t couleur) {
	if(capteur >= NOMBRE_DE_CAPTEURS)
		// TODO lever erreur
		return 0;
	if(couleur >= NOMBRE_DE_COULEURS)
		// TODO lever erreur
		return 0;

	return _capteurs[capteur][couleur];
}

// accesseur sur une des couleurs d'un des colorimetres
Vector3us CarteColorimetres2008::getCapteur(uint8_t capteur) {
	Vector3us retour;

	bloquerVariables();
	for(int couleur = 0; couleur < NOMBRE_DE_COULEURS; couleur++)
		retour[couleur] = _capteurs[capteur][couleur];
	debloquerVariables();

	return retour;
}
