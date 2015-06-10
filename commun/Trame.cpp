// Trame.cpp

#include "Trame.h"
#include <sstream>
#include <iomanip>
#include "log/Log.h"
#include <iostream>

std::ostream &operator<<(std::ostream &s, Byte const &b) {
	std::ios oldState(nullptr);
	oldState.copyfmt(s);

	s << "0x" << std::hex << b.value();

	s.copyfmt(oldState);

	return s;
}

Trame::Trame(uint8_t id, uint8_t cmd, uint8_t nbDonnees, uint8_t const donnees[]) : _num_paquet(0) {
	this->addBytes(nbDonnees, donnees);
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

uint8_t const *Trame::getDonnees() const {
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

uint8_t Trame::demultiplexId(MuxedIdAndCmd const &idAndCmd) {
	uint16_t muxedVal = (uint16_t(idAndCmd.second) << 8) | uint16_t(idAndCmd.first);
	return uint8_t((muxedVal >> BITS_CMD_TRAME) & ~(1 << BITS_ID_TRAME));
}

uint8_t Trame::demultiplexCmd(MuxedIdAndCmd const &idAndCmd) {
	uint16_t muxedVal = (uint16_t(idAndCmd.second) << 8) | uint16_t(idAndCmd.first);
	return uint8_t(muxedVal & 0xF);
}

Trame::MuxedIdAndCmd Trame::multiplexIdAndCmd(uint8_t id, uint8_t cmd) {
	uint16_t muxedVal = (uint16_t(id) << BITS_CMD_TRAME) | uint16_t(cmd);
	return std::make_pair(uint8_t(muxedVal & 0xFF), uint8_t((muxedVal >> 8) & 0xFF));
}

// afficher la trame sur le flux de sortie
std::ostream &operator<<(std::ostream & o, Trame const &t) {
	o << t.toString();
	return o;
}

// convertir la trame en chaine de caractere courte et en hexa
std::string Trame::toString() const {
	std::ostringstream oss;

	oss << "[id=";
	oss << int(_id);
	oss << ":";
	oss << "cmd=";
	oss << Byte(_cmd);
	oss << ":";
	oss << "données=";
	for(uint8_t numDonnee = 0; numDonnee < _donnees.size(); ++numDonnee) {
		oss << Byte(_donnees[numDonnee]);
		if(numDonnee == _donnees.size()-1)
			oss << "]";
		else
			oss << ",";
	}
	return oss.str();
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

