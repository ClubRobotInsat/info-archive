// VideoServeurUVCMJPG.cpp

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

#include "Socket.h"
#include "WebcamUVCMJPG.h"

#define DEFAULT_UVC_WIDTH 640
#define DEFAULT_UVC_HEIGHT 480
#define FPS 15.0f

using namespace std;

int main(int argc, char* argv[]) {
	unsigned short port = 9999;
	const char* device = "/dev/video0";
	for(int argNum = 1; argNum < argc; argNum++) {
		char* fin;
		int conv = strtod(argv[argNum], &fin);
		if(fin != argv[argNum])
			port = conv;
		else
			device = argv[argNum];
	}

	WebcamUVCMJPG webcam = WebcamUVCMJPG();
	if(!webcam.Init(device))
		return 1;

	// Connexion
	Socket::Init();
	Socket socketServeur;
	socketServeur.Create(SockProtocol::TCP);
	socketServeur.Listen(port);

	cout << "WebcamTCP/IP : ouverture d'un serveur sur le port " << port << " - attente d'un client (webcam)..." << endl;
	Socket socketClient;
	socketServeur.Accept(&(socketClient));

	cout << "Webcam TCP/IP connectee - envoi de la taille de l'image : " << DEFAULT_UVC_WIDTH << "x"
	     << DEFAULT_UVC_HEIGHT << "..." << flush;
	char str[64] = "";
	sprintf(str, "%dx%d\n", DEFAULT_UVC_WIDTH, DEFAULT_UVC_HEIGHT);
	socketServeur.SendMsg(&socketClient, str, strlen(str) + 1);
	cout << "OK" << endl;
	cout << "Webcam TCP/IP : debut de l'envoi a la demande de captures de la webcam" << endl;

	bool running = true;

	while(running) {
		// Récupération d'une image
		webcam.QueryImage();
		if(!webcam.WaitForImageSync()) {
			fprintf(stderr, "Erreur lors de la capture d'une frame par la webcam\n");
			exit(EXIT_FAILURE);
		}

		// Attente de réception d'une demande "GET" de l'IA
		socketServeur.ReceiveMsg(&socketClient, str, sizeof(str));

		if(string(str) == "GET") {
			socketServeur.SendMsg(&socketClient, webcam.GetData(), webcam.GetDataSize());
		}
		if(string(str) == "END") {
			running = false;
		}
	}

	cout << "Webcam TCP/IP : fin de la connexion avec le client" << endl;

	// Libération des ressources
	socketServeur.Destroy();

	return 0;
}
