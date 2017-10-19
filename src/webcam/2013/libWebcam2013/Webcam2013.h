/*
 * Webcam2013.h
 *
 *  Created on: 24 avr. 2013
 *      Author: jerk
 */

#ifndef WEBCAM2013_H_
#define WEBCAM2013_H_

#include "Webcam.h"
#include <vector>

/* Defines */
using namespace std;

/* Structures */
struct Position {
	int x, y;
};
enum Couleurs { BLEU = 0, ROUGE = 1, OTHER = 2 };

inline char* couleurToString(enum Couleurs c) {
	if(c == 0)
		return const_cast<char*>("BLEU");
	else if(c == 1)
		return const_cast<char*>("ROUGE");
	else
		return const_cast<char*>("UNDEFINED");
}

/* On dit que la classe Robot2013 existe mais elle est inclue dans le .cpp
 * Optimisation pour le temps compilation
 */
class Robot2013;

class Webcam2013 {
public:
	static const int NB_BOUGIES = 10;

private:
	/* Variables */
	IWebcam* webcam;
	Robot2013* robot;
	vector<struct Position> positions;
	enum Couleurs couleurs1[NB_BOUGIES];
	enum Couleurs couleurs2[NB_BOUGIES];
	enum Couleurs couleurs3[NB_BOUGIES];

	/* Utilitaires */
	void SavePPM(const char* filename, const unsigned char* data, int width, int height);
	void FillColorTab(const unsigned char* data, int width, enum Couleurs tab[]);
	bool CompareColorTab(const enum Couleurs tab1[], const enum Couleurs tab2[], int size);

public:
	/* Constructeurs */
	Webcam2013();
	Webcam2013(int argc, char* argv[], Robot2013* robot);
	~Webcam2013();

	/* Utilitaires */
	vector<struct Position> getPositions();
	enum Couleurs* FindConfiguration();

	/* Fonctions appelables */
	bool initializeWebcam(int argc, char* argv[]);
};

#endif /* WEBCAM2013_H_ */
