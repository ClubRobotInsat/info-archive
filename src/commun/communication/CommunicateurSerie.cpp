// Serie.cpp

#include "CommunicateurSerie.h"

/// Ouvre le port série indiqué et le configure
Serie::Serie(const std::string& port, ITrameListener& robot) : Communicateur(port, robot) {
	std::cout << "Serie::Serie " << port << std::endl;
#ifdef WIN32
	_fd = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(_fd == INVALID_HANDLE_VALUE) {
#else
	_fd = open(port.c_str(), O_RDWR | O_NOCTTY);
	if(_fd < 0) {
#endif
		throw ErreurOuvertureSerie("Impossible d'ouvrir le port RS232 '" + port + "'");
	}

#ifdef WIN32
	PurgeComm(_fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	DCB params;
	params.DCBlength = sizeof(DCB);
	if(!GetCommState(_fd, &params)) {
		throw ErreurOuvertureSerie("Impossible d'obtenir la configuration du port Serie '" + port + "'");
		exit(EXIT_FAILURE);
	}
	params.BaudRate = CBR_57600;  // 57600 bauds
	params.ByteSize = 8;          // char de 8 bits
	params.StopBits = ONESTOPBIT; // 1 bit de stop
	params.Parity = NOPARITY;     // pas de bit de parité
	if(!SetCommState(_fd, &params)) {
#else
	tcflush(_fd, TCIOFLUSH);
	termios params;
	memset(&params, 0, sizeof(termios));
	params.c_cflag = B57600 | CS8 | CLOCAL |
	                 CREAD; // 57600 bauds, char de 8 bits, ignorer signaux de contrôle, lecture/écriture, 1 bit de stop
	params.c_iflag = IGNPAR; // pas de bit de parité
	params.c_cc[VMIN] = 1;   // lecture bloquante jusqu'à recevoir 1 octet
	if(tcsetattr(_fd, TCSANOW, &params) == -1) {
#endif
		throw ErreurOuvertureSerie("Impossible de modifier la configuration du port Serie '" + port + "'");
	}
	connexion();

	commencerReception();
}

Serie::~Serie() {}

/// Ecrit un octet sur le port
void Serie::envoyer(unsigned char octet) {

#ifdef WIN32
	DWORD dummy;
	WriteFile(_fd, &octet, 1, &dummy, nullptr);
#else
	if(write(_fd, &octet, 1) == -1) {
	};
#endif
}

/// Attend un octet sur le port et retourne le premier arrivé - BLOQUANT
unsigned char Serie::lire() {
	unsigned char octet;

#ifdef WIN32
	DWORD dummy;
	ReadFile(_fd, &octet, 1, &dummy, nullptr);
#else
	if(read(_fd, &octet, 1) == -1) {
	};
#endif

	return octet;
}

// force une déconnexion (ferme le port), et attend la fin du thread de réception.
void Serie::cleanUp() {
	_cleaningUp = true;

#ifdef WIN32
	CloseHandle(_fd);
#else
	close(_fd);
#endif

	attendreFinReception();

	deconnexion();
}
