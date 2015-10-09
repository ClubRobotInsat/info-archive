// Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>
#include <iterator>

#ifndef M_PI
#define M_E         2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440
#endif

#define NO_COPY_CTOR(name) private: name(const name&) = delete; \
                                    void operator=(name) = delete;

using std::to_string;

// Permet la copie d'un std::vector sur un std::ostream (std::cout par exemple).
template<class T>
std::ostream &operator<<(std::ostream &out, std::vector<T> const &v) {
	out << '[';
	std::copy(v.begin(), v.end(), std::ostream_iterator<char>(out, ", "));
	out << ']';

	return out;
}

template<typename T>
std::string to_string(T const &t) {
	// utiliser un flux de sortie pour créer la chaîne
	std::ostringstream oss;
	// écrire la valeur dans le flux
	oss << t;
	// renvoyer une string
	return oss.str();
}

template<>
inline std::string to_string<uint8_t>(uint8_t const &value) {
	return to_string<int>(static_cast<uint8_t>(value));
}

/**** Conversions de/vers chaînes de caractères ******************************/

namespace Utils {
	// Fonctions utilisées par getNumbersArray()
	// Lisent un nombre à partir d'une chaîne de caractères le représentant
	template<typename T>
	inline std::enable_if_t<std::is_scalar<T>::value, T> strToNumber(std::string const &str) {
		T ret;

		std::istringstream s(str);
		s >> ret;

		return ret;
	}

	template<typename T>
	inline void strToNumber(std::string const &str, T &pNumber) {
		pNumber = Utils::strToNumber<T>(str);
	}

	// Fonction qui récupère dans numbersArray les nombres indiqués dans une chaîne
	// de la forme "1.234 5.678" ou "1 5 4 3"
	template <class T>
	inline unsigned int getNumbersArray(const char* str, T* numbersArray) {
		// Chaîne correspondant au nombre actuellement parcouru
		char strCurrentNumber[32] = "";

		int nbNum = 0;
		
		for(unsigned i=0, j=0, k=0 ; str[i] != '\0' ; i++) {
			// Si le caractère parcouru est le dernier de str :
			if(str[i+1] == '\0') {
				// On termine la chaîne strCurrentNumber
				strCurrentNumber[j] = str[i];
				j++;
				strCurrentNumber[j] = '\0';
				
				// Ajout du nombre au tableau de nombres
				strToNumber(strCurrentNumber, numbersArray[k]);
				k++;
				
				// Mémorisation du nombre de flottants de numbersArray
				nbNum = k;
				
				// Fin de la boucle
				break;
			}
			// Si le caractère parcouru est un espace, une tabulation ou un retour à la ligne :
			else if(str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n') {
				// On termine la chaîne strCurrentNumber
				strCurrentNumber[j] = '\0';
				
				// Ajout du nombre au tableau de nombres
				strToNumber(strCurrentNumber, numbersArray[k]);
				k++;
				
				// On avance jusqu'au prochain nombre
				while(str[i+1] == ' ' || str[i+1] == '\t'|| str[i+1] == '\r' || str[i+1] == '\n')
					i++;
				
				// Réinitialisation de strCurrentNumber, pour le nombre suivant
				j = 0;
				strCurrentNumber[0] = '\0';
			}
			// Sinon :
			else {
				// On continue de remplir strCurrentNumber
				strCurrentNumber[j] = str[i];
				j++;
			}
		}

		return nbNum;
	}
};

#endif // UTILS_H
