//
// Created by terae on 8/16/18.
//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#include "GlobalFrame.h"
#pragma GCC diagnostic pop
#include "log/Log.h"
#include <iomanip>
#include <iostream>
#include <sstream>

std::ostream& operator<<(std::ostream& s, Byte const& b) {
	/*std::ios oldState(nullptr);
	oldState.copyfmt(s);
	*/
	s << "0x" << std::hex << static_cast<int>(b.value());

	// s.copyfmt(oldState);

	return s;
}

GlobalFrame::GlobalFrame(std::initializer_list<uint8_t> donnees) {
	this->addBytes(donnees);
}

GlobalFrame::GlobalFrame(uint16_t nbDonnees, uint8_t const donnees[]) {
	this->addBytes(nbDonnees, donnees);
}

uint16_t GlobalFrame::getNbDonnees() const {
	return static_cast<uint16_t>(_donnees.size());
}

uint8_t GlobalFrame::getNumPaquet() const {
	return _num_paquet;
}

bool GlobalFrame::empty() const {
	return _donnees.empty();
}

uint8_t const* GlobalFrame::getDonnees() const {
	return &_donnees[0];
}

void GlobalFrame::setNumPaquet(uint8_t num_paquet) {
	_num_paquet = num_paquet;
}

void GlobalFrame::addBytes(std::initializer_list<uint8_t> donnees) {
	if(donnees.size() + this->getNbDonnees() > GlobalFrame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(donnees.size());

	_donnees.insert(_donnees.end(), donnees.begin(), donnees.end());
}

void GlobalFrame::addBytes(uint16_t nbDonnees, uint8_t const donnees[]) {
	if(nbDonnees + this->getNbDonnees() > GlobalFrame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(nbDonnees);

	_donnees.insert(_donnees.end(), donnees, donnees + nbDonnees);
}

void GlobalFrame::setDonnees(uint16_t nbDonnees, uint8_t const donnees[]) {
	_donnees.clear();
	this->addBytes(nbDonnees, donnees);
}

void GlobalFrame::set(uint16_t numero, uint8_t bit, bool valeur) {
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

// afficher la trame sur le flux de sortie
std::ostream& operator<<(std::ostream& o, GlobalFrame const& t) {
	o << "données=[";
	for(auto donnee : t._donnees) {
		o << Byte(donnee);
		o << ", ";
	}
	return o << "]]";
}

bool operator==(const GlobalFrame& f1, const GlobalFrame& f2) {
	return f1._donnees == f2._donnees;
}

GlobalFrame GlobalFrame::operator+(const GlobalFrame& f) const {
	GlobalFrame result{*this};
	result += f;
	return result;
}

GlobalFrame& GlobalFrame::operator+=(const GlobalFrame& f) {
	_donnees.insert(_donnees.end(), f._donnees.begin(), f._donnees.end());
	return *this;
}
