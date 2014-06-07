// Trame.h

#ifndef TRAME_H_
#define TRAME_H_

#include "Erreur.h"
#include "Utils.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>

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
		NB_CARTES_MAX = 128 // Adressage de la carte sur 7 bits => 128 cartes max
	};
	
	// erreur l'indice de la carte est trop grand
	class ErreurIdCarteTropGrand : public Erreur { public: ErreurIdCarteTropGrand(unsigned char id):Erreur("L'id "+Utils::toString(id)+" n'est pas un id valide"){} };

	// erreur si une trame n'est pas traitée
	class ErreurTrameNonTraitee : public Erreur { public: ErreurTrameNonTraitee(const Trame & t):Erreur("La trame "+ t.toString() +" n'a pas ete traitee"){} };

public:
	// constructeur : sans données
	Trame(std::uint8_t, std::uint8_t cmd);
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est trop grande
	
	// constructeur : une copie du tableau des données est crée
	Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t nbDonnees, std::uint8_t const donnees[], std::uint8_t num_paquet = 0);
	Trame(std::uint8_t id, std::uint8_t cmd, std::uint8_t donnees, std::uint8_t num_paquet = 0) : Trame(id, cmd, 1, &donnees, num_paquet) { }
	// lève ErreurIdCarteTropGrand si l'id de la carte est trop grand
	// lève ErreurNumCommandeTropGrand si la commande est 0xFtrop grande
	
	Trame(Trame const &t) = default;
	
	// destructeur
	virtual ~Trame() = default;
	
	// accesseur
	std::uint8_t getId() const;
	std::uint8_t getCmd() const;
	std::uint8_t getNbDonnees() const;
	std::uint8_t getNumPaquet() const;

	template<typename T = std::uint8_t>
	T getDonnee(std::uint8_t numero) const;

	// Récupération d'un bit de l'octet numero
	bool getDonneeBool(std::uint8_t numero, std::uint8_t bit);

	template<typename T>
	void add(T value);
	
	// modifieur
	void set(unsigned int numero, unsigned char bit, bool valeur); // lève ErreurNumeroDonneeTropGrand et ErreurNumeroBitTropGrand
	void setNumPaquet(unsigned int num_paquet);
	
	// erreur si le numero de la donnee est trop grand
	class ErreurNumeroDonneeTropGrand : public Erreur { public: ErreurNumeroDonneeTropGrand(unsigned char num) : Erreur("Le nombre de donnee dans la trame est trop petit pour acceder au numero " + Utils::toString(num)){} };
	
	// erreur si le numero d bit dans l'octet est trop grand
	class ErreurNumeroBitTropGrand : public Erreur { public: ErreurNumeroBitTropGrand(unsigned char num) : Erreur("Le numero du bit " + Utils::toString((short)num) + " est invalide : il doit etre comprit entre 0 et 7 inclut "){} };
	
	// afficher la trame sur le flux de sortie
	friend std::ostream & operator << (std::ostream &, const Trame &);
	
	// convertir la tramme en chaine de caractere courte et en hexa
	std::string toString() const;
	
	// convertir la tramme en chaine de caractere lisible et decimal
	std::string toStringLong() const;
	
	// erreur si la distance entre la camera au centre d'observation est negative
	class ErreurNumCommandeTropGrand {};
	
private:
	// numero de la carte a qui est adressée cette trame
	std::uint8_t _id;
	
	// numero de la commande
	std::uint8_t _cmd;
	
	// numéro de paquet (pour les ack)
	std::uint8_t _num_paquet;
	
	// tableau des données
	std::vector<std::uint8_t> _donnees;
};

template<typename T>
T Trame::getDonnee(std::uint8_t numero) const {
	if(numero + sizeof(T) > _donnees.size())
		throw ErreurNumeroDonneeTropGrand(numero);
		
	T value;
	std::uint8_t * pointer = reinterpret_cast<std::uint8_t *>(&value);
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

template<typename T>
void Trame::add(T value) {
	std::uint8_t * pointer = reinterpret_cast<std::uint8_t *>(&value);
	_donnees.insert(_donnees.end(), pointer, pointer + sizeof(T));
}

#endif /*TRAME_H_*/
