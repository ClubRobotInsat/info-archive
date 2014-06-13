// Trame.cpp

#include "Trame.h"
#include <sstream>

Trame::Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t nbDonnees, std::uint8_t const donnees[]) : _num_paquet(0) {
	this->addBytes(nbDonnees, donnees);
	this->setId(id);
	this->setCmd(cmd);
}

std::uint8_t Trame::getId() const {
	return _id;
}

std::uint8_t Trame::getCmd() const {
	return _cmd;
}

std::uint8_t Trame::getNbDonnees() const {
	return _donnees.size();
}

std::uint8_t Trame::getNumPaquet() const {
	return _num_paquet;
}

std::uint8_t const *Trame::getDonnees() const {
	return &_donnees[0];
}

void Trame::setNumPaquet(std::uint8_t num_paquet) {
	_num_paquet = num_paquet;
}

void Trame::setCmd(std::uint8_t cmd) {
	if(_cmd >= NUM_CMD_MAX)
		throw ErreurNumCommandeTropGrand(cmd);
	_cmd = cmd;
}

void Trame::setId(std::uint8_t id) {
	if(_id >= NB_CARTES_MAX)
		throw ErreurIdCarteTropGrand(id);
	_id = id;
}

void Trame::addBytes(std::uint8_t nbDonnees, std::uint8_t const donnees[]) {
	if(nbDonnees + this->getNbDonnees() > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(nbDonnees);

	_donnees.insert(_donnees.end(), donnees, donnees + nbDonnees);
}

void Trame::setDonnees(std::uint8_t nbDonnees, std::uint8_t const donnees[]) {
	_donnees.clear();
	this->addBytes(nbDonnees, donnees);
}

void Trame::set(std::uint8_t numero, std::uint8_t bit, bool valeur) {
	// lève ErreurNumeroDonneeTropGrand si numero trop grand
	// lève ErreurNumeroBitTropGrand si bit trop grand
	if(numero >= _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
	if(bit >= 8)
		throw ErreurNumeroBitTropGrand(bit);
	
	if(valeur)
		_donnees[numero] |= 1 << bit;
	else
		_donnees[numero] &= ~(1 << bit);
}

std::uint8_t Trame::demultiplexId(MuxedIdAndCmd const &idAndCmd) {
	std::uint16_t muxedVal = (std::uint16_t(idAndCmd.second) << 8) | std::uint16_t(idAndCmd.first);
	return std::uint8_t((muxedVal >> BITS_CMD_TRAME) & ~(1 << BITS_ID_TRAME));
}

std::uint8_t Trame::demultiplexCmd(MuxedIdAndCmd const &idAndCmd) {
	std::uint16_t muxedVal = (std::uint16_t(idAndCmd.second) << 8) | std::uint16_t(idAndCmd.first);
	return std::uint8_t(muxedVal & ~(1 << BITS_CMD_TRAME));
}

Trame::MuxedIdAndCmd Trame::multiplexIdAndCmd(std::uint8_t id, std::uint8_t cmd) {
	std::uint16_t muxedVal = (std::uint16_t(id) << BITS_CMD_TRAME) | std::uint16_t(cmd);
	return std::make_pair(std::uint8_t(muxedVal & 0xFF), std::uint8_t((muxedVal >> 8) & 0xFF));
}

// afficher la trame sur le flux de sortie
std::ostream & operator <<(std::ostream & o, const Trame &t) {
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
	for(std::uint8_t numDonnee = 0; numDonnee < _donnees.size(); ++numDonnee) {
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
	for(std::uint8_t numDonnee = 0; numDonnee < _donnees.size(); ++numDonnee)
		oss << (int) _donnees[numDonnee] << ',';
	oss << '}';
	return oss.str();
}

