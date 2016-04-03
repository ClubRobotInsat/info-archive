// Trame.h

#ifndef TRAME_H_
#define TRAME_H_

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

class Trame final {
public:
	/// Valeurs constantes de certains octets des trames
	enum {
		OCTET_DEBUT_TRAME_1 = 0xac,
		OCTET_DEBUT_TRAME_2 = 0xdc,
		OCTET_DEBUT_TRAME_3 = 0xab,
		OCTET_DEBUT_TRAME_4_NORMAL = 0xba,
		OCTET_DEBUT_TRAME_4_ACK = 0xbb,
	};

	// indice max d'une carte
	enum { NUM_CMD_MAX = 15, NB_CARTES_MAX = 16, DONNEES_TRAME_MAX = 8, BITS_ID_TRAME = 7, BITS_CMD_TRAME = 4 };

	// erreur l'indice de la carte est trop grand
	class ErreurIdCarteTropGrand : public std::runtime_error {
	public:
		ErreurIdCarteTropGrand(uint8_t id) : std::runtime_error("L'id " + to_string(id) + " n'est pas un id valide") {}
	};

	// erreur si une trame n'est pas traitée
	class ErreurTrameNonTraitee : public std::runtime_error {
	public:
		ErreurTrameNonTraitee(const Trame& t)
		        : std::runtime_error("La trame " + to_string(t) + " n'a pas été traitée") {}
	};

	// erreur si le numero de la donnée est trop grand
	class ErreurNumeroDonneeTropGrand : public std::runtime_error {
	public:
		ErreurNumeroDonneeTropGrand(uint8_t num)
		        : std::runtime_error("Le nombre de données dans la trame est trop petit pour accéder au numero " + to_string(num)) {}
	};

	// erreur si la quantité de données est trop grande
	class ErreurTropDeDonnees : public std::runtime_error {
	public:
		ErreurTropDeDonnees(uint8_t num) : std::runtime_error("Trop de données dans la trame : " + to_string(num)) {}
	};

	// erreur si le numero du bit dans l'octet est trop grand
	class ErreurNumeroBitTropGrand : public std::runtime_error {
	public:
		ErreurNumeroBitTropGrand(uint8_t num)
		        : std::runtime_error("Le numero du bit " + to_string((short)num) +
		                             " est invalide : il doit être comprit entre 0 et 7 inclu ") {}
	};

	// erreur si la distance entre la camera au centre d'observation est négative
	class ErreurNumCommandeTropGrand : public std::runtime_error {
	public:
		ErreurNumCommandeTropGrand(uint8_t num)
		        : std::runtime_error("Le numéro de commande demandé n'existe pas : " + to_string(num)) {}
	};

public:
	// constructeur : état par défaut, id = cmd = numPaquet = 0, pas de données
	Trame() : Trame(0, 0) {}

	// constructeur : sans données
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est trop grande
	explicit Trame(uint8_t id, uint8_t cmd) : Trame(id, cmd, {}) {}

	// constructeur, avec plusieurs données pour le 1er et 2e et une seule pour le 3e
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est 0xFtrop grande
	explicit Trame(uint8_t id, uint8_t cmd, std::initializer_list<uint8_t> donnees);
	explicit Trame(uint8_t id, uint8_t cmd, uint8_t nbDonnees, uint8_t const donnees[]);
	explicit Trame(uint8_t id, uint8_t cmd, uint8_t donnee) : Trame(id, cmd, {donnee}) {}

	Trame(Trame const& t) = default;
	Trame(Trame&& t) = default;

	// destructeur
	~Trame() = default;

	// accesseur
	uint8_t getId() const;
	uint8_t getCmd() const;
	uint8_t getNbDonnees() const;
	uint8_t getNumPaquet() const;

	uint8_t const* getDonnees() const;

	// Récupération de sizeof(T) octets à partir de l'octet numero
	template <typename T = uint8_t>
	T getDonnee(uint8_t numero) const;

	// Récupération d'un bit de l'octet numero
	bool getDonneeBool(uint8_t numero, uint8_t bit);

	void addByte(uint8_t value) {
		this->addDonnees(value);
	}
	void addBytes(std::initializer_list<uint8_t> bytes);
	void addBytes(uint8_t count, uint8_t const bytes[]);

	template <typename... Args>
	void addDonnees(Args&&... values) {
		this->addDonneesInternal(std::forward<Args>(values)...);
	}

	// Affecte une valeur à un bit de l'octet numeroOctet
	void set(uint8_t numeroOctet,
	         uint8_t numeroBit,
	         bool valeurBit); // lève ErreurNumeroDonneeTropGrand et ErreurNumeroBitTropGrand

	void setNumPaquet(uint8_t num_paquet);
	void setCmd(uint8_t cmd);
	void setId(uint8_t id);
	void setDonnees(uint8_t nbDonnees, uint8_t const donnees[]);
	void setDonnee(uint8_t donnee) {
		this->setDonnees(1, &donnee);
	}

	// Les 11 bits d'en-tête d'une trame CAN, regroupés en 2 fois 8 bits, dans l'ordre .first et .second
	using MuxedIdAndCmd = std::pair<uint8_t, uint8_t>;

	// Ces 2 fonctions vont permettre l'extraction de l'ID ou de la commande depuis les 11 bits d'en-tête d'une trame
	// CAN (regroupés en 2 fois 8 bits).
	static uint8_t demultiplexId(MuxedIdAndCmd const& idAndCmd);
	static uint8_t demultiplexId(uint8_t first, uint8_t second) {
		return demultiplexId({first, second});
	}
	static uint8_t demultiplexCmd(MuxedIdAndCmd const& idAndCmd);
	static uint8_t demultiplexCmd(uint8_t first, uint8_t second) {
		return demultiplexCmd({first, second});
	}

	// Retourne les 2 octets contenant les 11 bits d'en-tête CAN, à partir de l'ID de carte et de la commande
	static MuxedIdAndCmd multiplexIdAndCmd(uint8_t id, uint8_t cmd);

	// afficher la trame sur le flux de sortie
	friend std::ostream& operator<<(std::ostream&, const Trame&);

	// convertir la trame en chaîne de caractères lisible et avec les nombres en base décimale
	std::string toStringLong() const;

	friend bool operator==(Trame const& t1, Trame const& t2) {
		return t1._id == t2._id && t1._cmd == t2._cmd && t1._num_paquet == t2._num_paquet && t1._donnees == t2._donnees;
	}

private:
	void addDonneesInternal() {}

	template <typename T, typename... Args>
	void addDonneesInternal(T&& value, Args&&... values);
	template <typename... Args>
	void addDonneesInternal(Byte const& value, Args&&... values);

	// numéro de la carte à qui est adressée cette trame
	uint8_t _id = 0;

	// numéro de la commande
	uint8_t _cmd = 0;

	// numéro de paquet (pour les ack)
	uint8_t _num_paquet = 0;

	// tableau des données
	std::vector<uint8_t> _donnees;
};

template <typename T>
T Trame::getDonnee(uint8_t numero) const {
	static_assert(sizeof(T) <= DONNEES_TRAME_MAX, "Le type demandé ne peut pas contenir dans une trame !");
	if(numero + sizeof(T) > _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);

	T value;
	uint8_t* pointer = reinterpret_cast<uint8_t*>(&value);
	std::copy(&_donnees[numero], &_donnees[numero + sizeof(T)], pointer);

	return value;
}

inline bool Trame::getDonneeBool(uint8_t numero, uint8_t bit) {
	if(numero >= _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
	if(bit >= 8)
		throw ErreurNumeroBitTropGrand(bit);

	return _donnees[numero] && (1 << bit);
}

template <typename T, typename... Args>
void Trame::addDonneesInternal(T&& value, Args&&... values) {
	if(this->getNbDonnees() + sizeof(T) > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(this->getNbDonnees() + sizeof(T));

	auto pointer = reinterpret_cast<uint8_t const*>(&value);
	_donnees.insert(_donnees.end(), pointer, pointer + sizeof(T));

	this->addDonneesInternal(values...);
}

template <typename... Args>
void Trame::addDonneesInternal(Byte const& value, Args&&... values) {
	if(this->getNbDonnees() + 1 > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(this->getNbDonnees() + 1);

	auto byte = value.value();
	_donnees.insert(_donnees.end(), &byte, &byte + 1);

	this->addDonneesInternal(values...);
}

#endif /*TRAME_H_*/
