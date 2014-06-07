// Trame.cpp

#include "Trame.h"

#include <cstdio>
#include <sstream>

// constructeur : sans données
Trame::Trame(std::uint8_t id, std::uint8_t cmd) : _id(id), _cmd(cmd) {
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est trop grande
	if(_id >= NB_CARTES_MAX)
		throw ErreurIdCarteTropGrand(id);
	if(_cmd >= NUM_CMD_MAX)
		throw ErreurNumCommandeTropGrand();
}

// constructeur : une copie du tableau des données est créée
Trame::Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t nbDonnees, std::uint8_t const donnees[], std::uint8_t num_paquet) : _id(id), _cmd(cmd), _num_paquet(num_paquet), _donnees(donnees, donnees + nbDonnees) {
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est trop grande
	if(_id >= NB_CARTES_MAX)
		throw ErreurIdCarteTropGrand(id);
	if(_cmd >= NUM_CMD_MAX)
		throw ErreurNumCommandeTropGrand();
}

// accesseur
unsigned char Trame::getId() const {return _id;}

// accesseur
unsigned char Trame::getCmd() const {return _cmd;}

// accesseur
unsigned char Trame::getNbDonnees() const {return _donnees.size();}

// accesseur
std::uint8_t Trame::getNumPaquet() const {
	return _num_paquet;
}

// modifieur
void Trame::setNumPaquet(unsigned int num_paquet) {
	_num_paquet = num_paquet;
}

// modifieur
void Trame::set(unsigned int numero, unsigned char bit, bool valeur) {
	// lève ErreurNumeroDonneeTropGrand si numero trop grand
	// lève ErreurNumeroBitTropGrand si bit trop grand
	if(numero >= _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
	if(bit >= 8)
		throw ErreurNumeroBitTropGrand(bit);
	
	if(valeur)
		_donnees[numero] |= 1<<bit;
	else
		_donnees[numero] &= ~(1<<bit);
}

// afficher la trame sur le flux de sortie
std::ostream & operator << (std::ostream & o, const Trame & t) {
	o << t.toString();
	return o;
}

// convertir la tramme en chaine de caractere courte et en hexa
std::string Trame::toString() const {
	std::ostringstream oss;
	
	oss << "[id=";
	oss << std::hex << (int)_id;
	oss << ":";
	oss << "cmd=";
	oss << std::hex << (int)_cmd;
	oss << ":";
	oss << "donnees=";
	for(unsigned int numDonnee=0; numDonnee<_donnees.size(); numDonnee++) {
		oss << std::hex << (int) _donnees[numDonnee];
		if(numDonnee == _donnees.size()-1)
			oss << "]";
		else
			oss << ",";
	}
	return oss.str();
}

// convertir la tramme en chaine de caractere lisible et decimal
std::string Trame::toStringLong() const {
	std::ostringstream oss;
	oss << "id=" << (int) _id << " cmd=" << std::hex << (int) _cmd << " donnees={";
	for(unsigned int numDonnee=0; numDonnee<_donnees.size(); numDonnee++)
		oss << (int) _donnees[numDonnee] << ',';
	oss << '}';
	return oss.str();
}

