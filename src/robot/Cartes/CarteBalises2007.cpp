/*********************************************************
 * CarteBalises.cpp *
 * Implémente la classe gérant les balises 2006/2007 *
 *********************************************************/

#include "CarteBalises2007.h"
#include <cstring>

CarteBalises2007::CarteBalises2007(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_recepteurs, 0, sizeof(_recepteurs));
	std::memset(_recepteursOk, false, sizeof(_recepteursOk));
}

void CarteBalises2007::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b: // Etat récepteurs
			if(message.getNbDonnees() == 5) {
				uint8_t temp;

				bloquerVariables();
				uint8_t const* donnees = message.getDonnees();
				for(int i = 0; i < 4; i++) {
					temp = 0;

					// Renverser le tableau puisque les capteurs sont branchés à l'envers sur la carte -_-
					for(int j = 0; j < 8; j++)
						if(donnees[1 + i] & (0x80 >> j))
							temp |= (0x01 << j);

					if(donnees[0] == 0)
						_recepteurs[i] |= temp << 8;
					else if(donnees[0] == 1)
						_recepteurs[i] |= temp;
				}
				_recepteursOk[donnees[0]] = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}


// Demande une mise à jour des récepteurs
void CarteBalises2007::actualiserRecepteurs() {
	bloquerVariables();
	std::memset(_recepteurs, 0, sizeof(_recepteurs));
	std::memset(_recepteursOk, false, sizeof(_recepteursOk));
	debloquerVariables();

	envoyerMessage(0x01_b, 0xFF_b);
}

// Vérifie que les récepteurs sont à jour
bool CarteBalises2007::recepteursActualises() {
	bool resultat;

	bloquerVariables();
	resultat = _recepteursOk[0] && _recepteursOk[1];
	debloquerVariables();

	return resultat;
}

// Liste les récepteurs captant une balise donnée
unsigned short CarteBalises2007::lireRecepteurs(int balise) {
	unsigned short resultat;

	if(balise >= 4)
		return 0;

	bloquerVariables();
	resultat = _recepteurs[balise];
	debloquerVariables();

	return resultat;
}
