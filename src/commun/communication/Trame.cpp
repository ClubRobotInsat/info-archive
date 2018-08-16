// Trame.cpp

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#include "Trame.h"
#pragma GCC diagnostic pop
#include "log/Log.h"
#include <iomanip>
#include <iostream>
#include <sstream>

Trame::Trame(uint8_t id, uint8_t cmd, std::initializer_list<uint8_t> donnees) : GlobalFrame(donnees) {
	this->setId(id);
	this->setCmd(cmd);
}

Trame::Trame(uint8_t id, uint8_t cmd, uint8_t nbDonnees, uint8_t const donnees[]) : GlobalFrame(nbDonnees, donnees) {
	this->setId(id);
	this->setCmd(cmd);
}

uint8_t Trame::getId() const {
	return _id;
}

uint8_t Trame::getCmd() const {
	return _cmd;
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
