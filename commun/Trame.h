// Trame.h

#ifndef TRAME_H_
#define TRAME_H_

#include "Erreur.h"
#include "Utils.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>

class Byte {
public:
	explicit constexpr Byte(std::uint8_t val) : _value(val) {}
	constexpr Byte(Byte const &) = default;
	constexpr bool operator==(Byte const &b) const { return _value == b._value; }
	constexpr bool operator==(std::uint8_t b) const { return _value == b; }

	constexpr std::uint8_t value() const { return _value; }
	constexpr operator std::uint8_t() const { return _value; }

private:
	std::uint8_t _value;
};

inline std::ostream &operator<<(std::ostream &s, Byte const &b) {
	// Moche, mais comme ça on touche pas aux flags du flux (std::hex etc.)
	static char const lookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	std::uint8_t low = b.value() & 0x0F, high = (b.value() & 0xF0) >> 4;

	s << "0x" << lookup[high] << lookup[low];

	return s;
}

inline constexpr Byte operator"" _b(unsigned long long value) {
	return Byte(value);
}

class Trame {
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
	enum {
		NUM_CMD_MAX = 15,
		NB_CARTES_MAX = 16,
		DONNEES_TRAME_MAX = 8,
		BITS_ID_TRAME = 7,
		BITS_CMD_TRAME = 4
	};
	
	// erreur l'indice de la carte est trop grand
	class ErreurIdCarteTropGrand : public Erreur { public: ErreurIdCarteTropGrand(std::uint8_t id):Erreur("L'id "+Utils::toString(id)+" n'est pas un id valide"){} };

	// erreur si une trame n'est pas traitée
	class ErreurTrameNonTraitee : public Erreur { public: ErreurTrameNonTraitee(const Trame & t):Erreur("La trame "+ t.toString() +" n'a pas été traitée"){} };

	// erreur si le numero de la donnée est trop grand
	class ErreurNumeroDonneeTropGrand : public Erreur { public: ErreurNumeroDonneeTropGrand(std::uint8_t num) : Erreur("Le nombre de données dans la trame est trop petit pour accéder au numero " + Utils::toString(num)){} };

	// erreur si la quantité de données est trop grande
	class ErreurTropDeDonnees : public Erreur { public: ErreurTropDeDonnees(std::uint8_t num) : Erreur("Trop de données dans la trame : " + Utils::toString(num)){} };

	// erreur si le numero du bit dans l'octet est trop grand
	class ErreurNumeroBitTropGrand : public Erreur { public: ErreurNumeroBitTropGrand(std::uint8_t num) : Erreur("Le numero du bit " + Utils::toString((short)num) + " est invalide : il doit être comprit entre 0 et 7 inclu "){} };

	// erreur si la distance entre la camera au centre d'observation est négative
	class ErreurNumCommandeTropGrand : public Erreur { public: ErreurNumCommandeTropGrand(std::uint8_t num) : Erreur("Le numéro de commande demandé n'existe pas : " + Utils::toString(num)){} };

public:
	// constructeur : état par défaut, id = cmd = numPaquet = 0, pas de données
	Trame() : Trame(0, 0) { }

	// constructeur : sans données
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est trop grande
	explicit Trame(std::uint8_t id, std::uint8_t cmd) : Trame(id, cmd, 0, nullptr) { }

	// constructeur, avec plusieurs données pour le 1er et une seule pour le 2e
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est 0xFtrop grande
	explicit Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t nbDonnees, std::uint8_t const donnees[]);
	explicit Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t donnee) : Trame(id, cmd, 1, &donnee) { }

	Trame(Trame const &t) = default;
	Trame(Trame &&t) = default;

	// destructeur
	virtual ~Trame() = default;
	
	// accesseur
	std::uint8_t getId() const;
	std::uint8_t getCmd() const;
	std::uint8_t getNbDonnees() const;
	std::uint8_t getNumPaquet() const;

	std::uint8_t const *getDonnees() const;

	// Récupération de sizeof(T) octets à partir de l'octet numero
	template<typename T = std::uint8_t>
	T getDonnee(std::uint8_t numero) const;

	// Récupération d'un bit de l'octet numero
	bool getDonneeBool(std::uint8_t numero, std::uint8_t bit);

	void addByte(std::uint8_t value) { this->addDonnees(value); }
	void addBytes(std::uint8_t count, std::uint8_t const bytes[]);

	template<typename... Args>
	void addDonnees(Args &&... values);

	// Affecte une valeur à un bit de l'octet numero
	void set(std::uint8_t numero, std::uint8_t bit, bool valeur); // lève ErreurNumeroDonneeTropGrand et ErreurNumeroBitTropGrand

	void setNumPaquet(std::uint8_t num_paquet);
	void setCmd(std::uint8_t cmd);
	void setId(std::uint8_t id);
	void setDonnees(std::uint8_t nbDonnees, std::uint8_t const donnees[]);
	void setDonnee(std::uint8_t donnee) { this->setDonnees(1, &donnee); }

	// Les 11 bits d'en-tête d'une trame CAN, regroupés en 2 fois 8 bits, dans l'ordre .first et .second
	using MuxedIdAndCmd = std::pair<std::uint8_t, std::uint8_t>;

	// Ces 2 fonctions vont permettre l'extraction de l'ID ou de la commande depuis les 11 bits d'en-tête d'une trame CAN (regroupés en 2 fois 8 bits).
	static std::uint8_t demultiplexId(MuxedIdAndCmd const &idAndCmd);
	static std::uint8_t demultiplexId(std::uint8_t first, std::uint8_t second) { return demultiplexId(MuxedIdAndCmd(first, second)); }
	static std::uint8_t demultiplexCmd(MuxedIdAndCmd const &idAndCmd);
	static std::uint8_t demultiplexCmd(std::uint8_t first, std::uint8_t second) { return demultiplexCmd(MuxedIdAndCmd(first, second)); }

	// Retourne les 2 octets contenant les 11 bits d'en-tête CAN, à partir de l'ID de carte et de la commande
	static MuxedIdAndCmd multiplexIdAndCmd(std::uint8_t id, std::uint8_t cmd);

	// afficher la trame sur le flux de sortie
	friend std::ostream & operator <<(std::ostream &, const Trame &);
	
	// convertir la tramme en chaine de caractere courte et en hexa
	std::string toString() const;
	
	// convertir la tramme en chaine de caractere lisible et decimal
	std::string toStringLong() const;

	friend bool operator==(Trame const &t1, Trame const &t2) {
		return t1._id == t2._id && t1._cmd == t2._cmd && t1._num_paquet == t2._num_paquet && t1._donnees == t2._donnees;
	}
	
private:
	void addDonneesInternal() { }

	template<typename T, typename... Args>
	void addDonneesInternal(T &&value, Args &&... values);
	template<typename... Args>
	void addDonneesInternal(Byte const &value, Args &&... values);

	// numéro de la carte à qui est adressée cette trame
	std::uint8_t _id = 0;
	
	// numéro de la commande
	std::uint8_t _cmd = 0;
	
	// numéro de paquet (pour les ack)
	std::uint8_t _num_paquet = 0;
	
	// tableau des données
	std::vector<std::uint8_t> _donnees;
};

template<typename T>
T Trame::getDonnee(std::uint8_t numero) const {
	static_assert(sizeof(T) <= DONNEES_TRAME_MAX, "Le type demandé ne peut pas contenir dans une trame !");
	if(numero + sizeof(T) > _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
		
	T value;
	std::uint8_t *pointer = reinterpret_cast<std::uint8_t *>(&value);
	std::copy(&_donnees[numero], &_donnees[numero + sizeof(T)], pointer);
	
	return value;
}

inline bool Trame::getDonneeBool(std::uint8_t numero, std::uint8_t bit) {
	if(numero >= _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
	if(bit >= 8)
		throw ErreurNumeroBitTropGrand(bit);
	
	return _donnees[numero] && (1 << bit);
}

template<typename... Args>
void Trame::addDonnees(Args &&... values) {
	this->addDonneesInternal(std::forward<Args>(values)...);
}

template<typename T, typename... Args>
void Trame::addDonneesInternal(T &&value, Args &&... values) {
	if(this->getNbDonnees() + sizeof(T) > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(this->getNbDonnees() + sizeof(T));

	auto pointer = reinterpret_cast<std::uint8_t const *>(&value);
	_donnees.insert(_donnees.end(), pointer, pointer + sizeof(T));

	this->addDonneesInternal(values...);
}

template<typename... Args>
void Trame::addDonneesInternal(Byte const &value, Args &&... values) {
	if(this->getNbDonnees() + 1 > Trame::DONNEES_TRAME_MAX)
		throw ErreurTropDeDonnees(this->getNbDonnees() + 1);

	auto byte = value.value();
	_donnees.insert(_donnees.end(), &byte, &byte + 1);

	this->addDonneesInternal(values...);
}

#endif /*TRAME_H_*/
