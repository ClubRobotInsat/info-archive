// Trame.cpp

#include "Trame.h"
#include "log/Log.h"
#include <iomanip>
#include <iostream>
#include <sstream>

std::ostream& operator<<(std::ostream& s, Byte const& b) {
	/*std::ios oldState(nullptr);
	oldState.copyfmt(s);
	*/
	s << "0x" << std::hex << (int)b.value();

	// s.copyfmt(oldState);

	return s;
}

Trame::Trame(uint8_t id, uint8_t cmd, uint8_t nbDonnees, uint8_t const donnees[]) {
	this->addBytes(nbDonnees, donnees);
	this->setId(id);
	this->setCmd(cmd);
}

Trame::Trame(uint8_t id, uint8_t cmd, std::initializer_list<uint8_t> donnees) {
	this->addBytes(donnees);
	this->setId(id);
	this->setCmd(cmd);
}

uint8_t Trame::getId() const {
	return _id;
}

uint8_t Trame::getCmd() const {
	return _cmd;
}

uint8_t Trame::getNbDonnees() const {
	return _donnees.size();
}

uint8_t Trame::getNumPaquet() const {
	return _num_paquet;
}

uint8_t const* Trame::getDonnees() const {
	return &_donnees[0];
}

void Trame::setNumPaquet(uint8_t num_paquet) {
	_num_paquet = num_paquet;
}

void Trame::setCmd(uint8_t cmd) {
	if(_cmd >= NUM_CMD_MAX)
		throw ErreurNumCommandeTropGrand(cmd);
	_cmd = cmd;
}

void Trame::setId(uint8_t id) {
	if(_id >= NB_CARTES_MAX)
		throw ErreurIdCarteTropGrand(id);
	_id = id;
}

void Trame::addBytes(std::initializer_list<uint8_t> donnees) {
	if(donnees.size() + this->getNbDonnees() > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(donnees.size());

	_donnees.insert(_donnees.end(), donnees.begin(), donnees.end());
}

void Trame::addBytes(uint8_t nbDonnees, uint8_t const donnees[]) {
	if(nbDonnees + this->getNbDonnees() > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(nbDonnees);

	_donnees.insert(_donnees.end(), donnees, donnees + nbDonnees);
}

void Trame::setDonnees(uint8_t nbDonnees, uint8_t const donnees[]) {
	_donnees.clear();
	this->addBytes(nbDonnees, donnees);
}

void Trame::set(uint8_t numero, uint8_t bit, bool valeur) {
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

uint8_t Trame::demultiplexId(MuxedIdAndCmd const& idAndCmd) {
	uint16_t muxedVal = (uint16_t(idAndCmd.second) << 8) | uint16_t(idAndCmd.first);
	return uint8_t((muxedVal >> BITS_CMD_TRAME) & ~(1 << BITS_ID_TRAME));
}

uint8_t Trame::demultiplexCmd(MuxedIdAndCmd const& idAndCmd) {
	uint16_t muxedVal = (uint16_t(idAndCmd.second) << 8) | uint16_t(idAndCmd.first);
	return uint8_t(muxedVal & 0xF);
}

Trame::MuxedIdAndCmd Trame::multiplexIdAndCmd(uint8_t id, uint8_t cmd) {
	uint16_t muxedVal = (uint16_t(id) << BITS_CMD_TRAME) | uint16_t(cmd);
	return std::make_pair(uint8_t(muxedVal & 0xFF), uint8_t((muxedVal >> 8) & 0xFF));
}

// afficher la trame sur le flux de sortie
std::ostream& operator<<(std::ostream& o, Trame const& t) {
	o << "[id=";
	o << int(t._id);
	o << ":";
	o << "cmd=";
	o << Byte(t._cmd);
	o << ":";
	o << "données=[";
	for(auto donnee : t._donnees) {
		o << Byte(donnee);
		o << ", ";
	}
	o << "]]";
	return o;
}

// convertir la trame en chaîne de caractères lisible et avec les nombres en base décimale
std::string Trame::toStringLong() const {
	std::ostringstream oss;
	oss << "id=" << int(_id) << " cmd=" << Byte(_cmd) << " donnees={";
	for(uint8_t numDonnee = 0; numDonnee < _donnees.size(); ++numDonnee)
		oss << (int)_donnees[numDonnee] << ',';
	oss << '}';
	return oss.str();
}
/*
Trame Trame::operator=(Trame const &t2) {
    Trame result;
    result._id = t2._id;
    result._cmd = t2._cmd;
    result._donnees = t2._donnees;
    result._num_paquet = t2._num_paquet;
    return result;
}
*/