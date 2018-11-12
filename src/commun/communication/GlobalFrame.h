//
// Created by terae on 8/16/18.
//

#ifndef ROOT_IFRAME_H
#define ROOT_IFRAME_H

#include "Utils.h"

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Byte final {
public:
	constexpr Byte(uint8_t val) : _value(val) {}
	constexpr Byte(Byte const&) = default;
	constexpr bool operator==(Byte const& b) const {
		return _value == b._value;
	}
	constexpr bool operator==(uint8_t b) const {
		return _value == b;
	}

	constexpr uint8_t value() const {
		return _value;
	}
	constexpr operator uint8_t() const {
		return _value;
	}

private:
	uint8_t _value;
};

std::ostream& operator<<(std::ostream& s, Byte const& b);

inline constexpr Byte operator"" _b(unsigned long long value) {
	return Byte(value);
}

/*
inline constexpr Byte makeFromString(std::string value) {

}
*/

/// Cette classe de base permets de créer des trames génériques,
/// à la fois pour une architecture avec des cartes (IDs et commandes) comme avant 2019
/// mais aussi avec une architecture où tout l'état du robot est envoyé en permanence
class GlobalFrame {
public:
	enum { DONNEES_TRAME_MAX = 0xFFFF };

	// erreur si une trame n'est pas traitée
	class ErreurTrameNonTraitee : public std::runtime_error {
	public:
		explicit ErreurTrameNonTraitee(const GlobalFrame& t)
		        : std::runtime_error("La trame " + to_string(t) + " n'a pas été traitée") {}
	};

	// erreur si le numero de la donnée est trop grand
	class ErreurNumeroDonneeTropGrand : public std::runtime_error {
	public:
		explicit ErreurNumeroDonneeTropGrand(uint16_t num)
		        : std::runtime_error("Le nombre de données dans la trame est trop petit pour accéder au numero " + to_string(num)) {}
	};

	// erreur si la quantité de données est trop grande
	class ErreurTropDeDonnees : public std::runtime_error {
	public:
		explicit ErreurTropDeDonnees(uint16_t num)
		        : std::runtime_error("Trop de données dans la trame : " + to_string(num)) {}
	};

	// erreur si le numero du bit dans l'octet est trop grand
	class ErreurNumeroBitTropGrand : public std::runtime_error {
	public:
		explicit ErreurNumeroBitTropGrand(uint8_t num)
		        : std::runtime_error("Le numero du bit " + to_string(static_cast<short>(num)) +
		                             " est invalide : il doit être comprit entre 0 et 7 inclu ") {}
	};

	// Constructeurs
	GlobalFrame() = default;
	GlobalFrame(std::initializer_list<uint8_t> donnees);
	explicit GlobalFrame(uint16_t nbDonnees, uint8_t const donnees[]);
	explicit GlobalFrame(uint8_t donnee) : GlobalFrame({donnee}) {}

	GlobalFrame(GlobalFrame const& t) = default;
	GlobalFrame(GlobalFrame&& t) = default;

	// destructeur
	~GlobalFrame() = default;

	// accesseurs
	uint16_t getNbDonnees() const;
	uint8_t getNumPaquet() const;

	bool empty() const;

	uint8_t const* getDonnees() const;

	// Récupération de sizeof(T) octets à partir de l'octet numero
	template <typename T = uint8_t>
	T getDonnee(uint16_t numero) const;

	// Récupération d'un bit de l'octet numero
	bool getDonneeBool(uint8_t numero, uint8_t bit);

	void addByte(uint8_t value) {
		this->addDonnees(value);
	}
	void addBytes(std::initializer_list<uint8_t> bytes);
	void addBytes(uint16_t count, uint8_t const bytes[]);

	template <typename... Args>
	void addDonnees(Args&&... values) {
		this->addDonneesInternal(std::forward<Args>(values)...);
	}

	// Affecte une valeur à un bit de l'octet numeroOctet
	void set(uint16_t numeroOctet,
	         uint8_t numeroBit,
	         bool valeurBit); // lève ErreurNumeroDonneeTropGrand et ErreurNumeroBitTropGrand

	void setNumPaquet(uint8_t num_paquet);

	void setDonnees(uint16_t nbDonnees, uint8_t const donnees[]);
	void setDonnee(uint8_t donnee) {
		this->setDonnees(1, &donnee);
	}

	// afficher la trame sur le flux de sortie
	friend std::ostream& operator<<(std::ostream&, const GlobalFrame&);

	friend bool operator==(const GlobalFrame&, const GlobalFrame&);

	// Opérateurs de concaténation des trames
	// Les informations annexes des '_donnees' sont copiées depuis la variable de gauche
	GlobalFrame operator+(const GlobalFrame& f) const;
	GlobalFrame& operator+=(const GlobalFrame& f);

protected:
	// numéro de paquet (pour les ack)
	uint8_t _num_paquet = 0;

	// tableau des données
	std::vector<uint8_t> _donnees;

private:
	void addDonneesInternal() {}

	template <typename T, typename... Args>
	void addDonneesInternal(T&& value, Args&&... values);
	template <typename... Args>
	void addDonneesInternal(Byte const& value, Args&&... values);
};

template <typename T>
T GlobalFrame::getDonnee(uint16_t numero) const {
	static_assert(sizeof(T) <= GlobalFrame::DONNEES_TRAME_MAX, "Le type demandé ne peut pas contenir dans une trame !");
	if(numero + sizeof(T) > _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);

	T value;
	uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);
	std::copy(&_donnees[numero], &_donnees[numero + sizeof(T)], pointer);

	return value;
}

inline bool GlobalFrame::getDonneeBool(uint8_t numero, uint8_t bit) {
	if(numero >= _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
	if(bit >= 8)
		throw ErreurNumeroBitTropGrand(bit);

	return (_donnees[numero] & (0x1 << bit)) != 0;
}

template <typename T, typename... Args>
void GlobalFrame::addDonneesInternal(T&& value, Args&&... values) {
	if(this->getNbDonnees() + sizeof(T) > GlobalFrame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(this->getNbDonnees() + sizeof(T));

	auto pointer = reinterpret_cast<uint8_t const*>(&value);
	_donnees.insert(_donnees.end(), pointer, pointer + sizeof(T));

	this->addDonneesInternal(values...);
}

template <typename... Args>
void GlobalFrame::addDonneesInternal(Byte const& value, Args&&... values) {
	if(this->getNbDonnees() + 1 > GlobalFrame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(static_cast<uint8_t>(this->getNbDonnees() + 1));

	auto byte = value.value();
	_donnees.insert(_donnees.end(), &byte, &byte + 1);

	this->addDonneesInternal(values...);
}

#endif // ROOT_IFRAME_H
