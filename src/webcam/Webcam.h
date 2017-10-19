// Webcam.h

#ifndef WEBCAM_H
#define WEBCAM_H

#include "IWebcam.h"

// Fonction qui crée un nouvelle webcam en fonction de ce qui est passé en paramètre
// Si force_error == true : on fait une erreur s'il n'y a rien de précisé sur la ligne de commande.
IWebcam* NewWebcam(int argc, char* argv[], bool force_error = true);

#endif // WEBCAM_H
