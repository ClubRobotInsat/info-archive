// Trame.h

#ifndef TRAME_H_
#define TRAME_H_

#include "GlobalFrame.h"

/// Ancien format des trames (< 2019), approprié pour une architecture avec des cartes décentralisées
/// qui communiquent de manière asynchrone par un bus CAN
class Trame final : public GlobalFrame {
public:
	// indice max d'une carte
	enum { NUM_CMD_MAX = 15, NB_CARTES_MAX = GlobalFrame::NB_MODULES_MAX, BITS_ID_TRAME = 7, BITS_CMD_TRAME = 4 };

	// erreur l'indice de la carte est trop grand
	class ErreurIdCarteTropGrand : public std::runtime_error {
	public:
		ErreurIdCarteTropGrand(uint8_t id) : std::runtime_error("L'id " + to_string(id) + " n'est pas un id valide") {}
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

	Trame operator=(Trame const& t2);

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

	void setCmd(uint8_t cmd);
	void setId(uint8_t id);

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
	// numéro de la carte à qui est adressée cette trame
	uint8_t _id = 0;

	// numéro de la commande
	uint8_t _cmd = 0;
};

#endif /*TRAME_H_*/
