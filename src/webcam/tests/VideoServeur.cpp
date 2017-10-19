// VideoServeur.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "IWebcam.h"
#include "ImageFiltree.h"
#include "Webcam.h"
#include "WebcamSimuTCPIP.h"
#include "WebcamV4L.h"

using namespace std;

int main(int argc, char* argv[]) {
	unsigned short port = 9999;
	char str[64] = "";
	int width, height;
	char* convertionFin;

	// Récupération des arguments et création de la webcam
	IWebcam* webcam = NewWebcam(argc, argv);
	if(!webcam) {
		fprintf(stderr, "Impossible de creer la webcam\n");
		return -1;
	}
	width = webcam->GetWidth();
	height = webcam->GetHeight();

	// convertion en int du 4eme argument
	if(argc == 4) {
		port = (unsigned short)strtod(argv[3], &convertionFin);
		if(convertionFin == argv[3] || port > 0xFFFF) {
			fprintf(stderr, "vous devez preciser un numero de port valide en 4eme argument\n");
			return -1;
		}
	}

	// Connexion
	Socket::Init();
	Socket socketServeur;
	socketServeur.Create(SockProtocol::TCP);
	socketServeur.Listen(port);

	cout << "WebcamTCP/IP : ouverture d'un serveur sur le port " << port << " - attente d'un client (webcam)..." << endl;
	Socket socketClient;
	socketServeur.Accept(&(socketClient));

	cout << "Webcam TCP/IP connectee - envoi de la taille de l'image : " << webcam->GetWidth() << "x"
	     << webcam->GetHeight() << "..." << flush;
	sprintf(str, "%dx%d\n", width, height);
	socketServeur.SendMsg(&socketClient, str, strlen(str) + 1);
	cout << "OK" << endl;
	cout << "Webcam TCP/IP : debut de l'envoi a la demande de captures de la webcam" << endl;

	bool running = true;

	while(running) {
		// Récupération d'une image
		webcam->QueryImage();
		if(!webcam->WaitForImageSync()) {
			fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
			exit(EXIT_FAILURE);
		}

		// Attente de réception d'une demande "GET" de l'IA
		socketServeur.ReceiveMsg(&socketClient, str, sizeof(str));

		if(string(str) == "GET") {
			socketServeur.SendMsg(&socketClient, webcam->GetData(), webcam->GetWidth() * webcam->GetHeight() * 3 * sizeof(unsigned char));
		}
		if(string(str) == "END") {
			running = false;
		}
	}

	cout << "Webcam TCP/IP : fin de la connexion avec le client" << endl;

	// Libération des ressources
	delete webcam;

	return 0;
}
