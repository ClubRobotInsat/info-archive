// Vecteur.h :
// - Vecteur<N, T> : vecteur � N dimensions d'�l�ments de type T
// - Vecteur<2, T> : vecteur � 2 dimensions d'�l�ments de type T, avec certaines fonctions
// en plus
// - Vecteur<3, T> : vecteur � 3 dimensions d'�l�ments de type T, avec certaines fonctions
// en plus
// + des typedefs pour les vecteurs 2D et 3D (b : byte, s : short, f : float, d : double,
// u : unsigned) :
// - Vecteur2ub, Vecteur2i, Vecteur2s, Vecteur2us, Vecteur2ui, Vecteur2f, Vecteur2d
// - Vecteur3ub, Vecteur3i, Vecteur3s, Vecteur3us, Vecteur3ui, Vecteur3f, Vecteur3d

// NB : pour pouvoir utiliser le constructeur de copie, il faut que l'op�rateur = soit
// d�fini pour le type des scalaires contenus dans le vecteur.

// NB : l'unit� par d�faut des angles est le radian, des fonctions de conversion existent
// dans MathsUtilities.h. Par d�faut, les valeurs renvoy�es sont situ�es entre -PI et PI.

#ifndef VECTEUR_H
#define VECTEUR_H

#include "MathUtilities.h"
#include <iostream>

/************** Vecteur<N, T> **************/
template <int N, typename T>
class Vecteur
{
protected:
	T elements[N];	// Composantes

public:
	// Constructeurs / destructeur
	Vecteur();
	Vecteur(const Vecteur<N, T>& ref);

	template <typename T_scalaire>
	Vecteur(const T_scalaire& scalaire);

	~Vecteur();

	// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
	// toutes les composantes de v1 prendront la valeur du scalaire.
	Vecteur<N, T>& operator=(const Vecteur<N, T>& ref);

	template <typename T_scalaire>
	Vecteur<N, T>& operator=(const T_scalaire& scalaire);

	// Op�rations :
	// + : addition composante par composante
	// - : soustraction composante par composante
	// % : multiplication composante par composante
	// / : division composante par composante
	// * : produit scalaire (quand entre 2 vecteurs)
	// * : multiplication par un scalaire : v * s
	// / : division par un scalaire : v/s
	// NB : le produit vectoriel (^) est r�serv� � la sp�cialisation en 3 dimensions
	Vecteur<N, T> operator+(const Vecteur<N, T>& ref) const;
	Vecteur<N, T> operator-(const Vecteur<N, T>& ref) const;
	Vecteur<N, T> operator%(const Vecteur<N, T>& ref) const;
	Vecteur<N, T> operator/(const Vecteur<N, T>& ref) const;

/*	template <typename T_scalaire>
	T_scalaire operator*(const Vecteur<N, T>& ref) const;	// produit scalaire
*/
	template <typename T_scalaire>
	Vecteur<N, T> operator*(const T_scalaire& scalaire) const;	// v * s

	template <typename T_scalaire>
	Vecteur<N, T> operator/(const T_scalaire& scalaire) const;	// v / s

	// Op�rations-affectations : +=, -=, %=, /=, *=
	Vecteur<N, T>& operator+=(const Vecteur<N, T>& ref);
	Vecteur<N, T>& operator-=(const Vecteur<N, T>& ref);
	Vecteur<N, T>& operator%=(const Vecteur<N, T>& ref);
	Vecteur<N, T>& operator/=(const Vecteur<N, T>& ref);

	template <typename T_scalaire>
	Vecteur<N, T>& operator*=(const T_scalaire& scalaire);

	template <typename T_scalaire>
	Vecteur<N, T>& operator/=(const T_scalaire& scalaire);

	// Comparaison
	// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
	// avec des flottants, du fait de leur impr�cision).
	// Pour effectuer une comparaison approch�e, utiliser environEgal()
	// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
	// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)
	bool operator==(const Vecteur<N, T>& ref) const;
	bool operator!=(const Vecteur<N, T>& ref) const;
	bool operator>(const Vecteur<N, T>& ref) const;
	bool operator>=(const Vecteur<N, T>& ref) const;
	bool operator<(const Vecteur<N, T>& ref) const;
	bool operator<=(const Vecteur<N, T>& ref) const;

	// Acc�s aux �l�ments
	T& operator[](int index);

	// Acc�s aux �l�ments
	const T& operator[](int index) const;

	// Calcul de la norme
	double CalculerNorme() const;

	// Normalisation du vecteur
	void Normaliser();

	// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
	// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
	bool EnvironEgal(const Vecteur<N, T>& ref, double imprecisionMax=EPSILON) const;

	// Met le vecteur � 0.
	void Reset();
};

// Affichage sur la console ou dans un fichier (pour le d�bogage), avec cout & co
template <int N, typename T>
std::ostream& operator<<(std::ostream& stream, const Vecteur<N, T>& vec);

// Multiplication par un scalaire, dans l'autre sens : s * v
// NB : il est pass� un Vecteur<N, T> en param�tres et non une r�f�rence car sinon, une
// expression comme v1 = 2*(v2 + v3) ne compile pas.
template <int N, typename T, typename T_scalaire>
Vecteur<N, T> operator*(const T_scalaire& s, Vecteur<N, T> v);

// Division par un scalaire, dans l'autre sens : s / v
template <int N, typename T, typename T_scalaire>
Vecteur<N, T> operator/(const T_scalaire& s, Vecteur<N, T> v);

// Produit scalaire, entre 2 vecteurs
template <int N, typename T>
double operator*(Vecteur<N, T>& op1, Vecteur<N, T>& op2);

/************** Vecteur<2, T> **************/
template <typename T>
class Vecteur<2, T>
{
protected:
	T elements[2];	// Composantes

public:
	// R�f�rences permettant d'acc�der directement aux composantes x et y
	// en lecture et �criture ainsi : v.x, v.y.
	T& x;
	T& y;

public:
	// Constructeurs / destructeur
	Vecteur<2, T>();
	Vecteur<2, T>(const Vecteur<2, T>& ref);
	Vecteur<2, T>(const T& x, const T& y);

	template <typename T_scalaire>
	Vecteur(const T_scalaire& scalaire);

	~Vecteur();

	// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
	// toutes les composantes de v1 prendront la valeur du scalaire.
	Vecteur<2, T>& operator=(const Vecteur<2, T>& ref);

	template <typename T_scalaire>
	Vecteur<2, T>& operator=(const T_scalaire& scalaire);

	// Op�rations :
	// + : addition composante par composante
	// - : soustraction composante par composante
	// % : multiplication composante par composante
	// / : division composante par composante
	// * : produit scalaire (quand entre 2 vecteurs)
	// * : multiplication par un scalaire : v * s
	// / : division par un scalaire : v/s
	// NB : le produit vectoriel (^) est r�serv� � la sp�cialisation en 3 dimensions
	Vecteur<2, T> operator+(const Vecteur<2, T>& ref) const;
	Vecteur<2, T> operator-(const Vecteur<2, T>& ref) const;
	Vecteur<2, T> operator%(const Vecteur<2, T>& ref) const;
	Vecteur<2, T> operator/(const Vecteur<2, T>& ref) const;

	template <typename T_scalaire>
	T_scalaire operator*(const Vecteur<2, T>& ref) const;	// produit scalaire

	template <typename T_scalaire>
	Vecteur<2, T> operator*(const T_scalaire& scalaire) const;	// v * s

	template <typename T_scalaire>
	Vecteur<2, T> operator/(const T_scalaire& scalaire) const;	// v / s

	// Op�rations-affectations : +=, -=, %=, /=, *=
	Vecteur<2, T>& operator+=(const Vecteur<2, T>& ref);
	Vecteur<2, T>& operator-=(const Vecteur<2, T>& ref);
	Vecteur<2, T>& operator%=(const Vecteur<2, T>& ref);
	Vecteur<2, T>& operator/=(const Vecteur<2, T>& ref);

	template <typename T_scalaire>
	Vecteur<2, T>& operator*=(const T_scalaire& scalaire);

	template <typename T_scalaire>
	Vecteur<2, T>& operator/=(const T_scalaire& scalaire);

	// Comparaison
	// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
	// avec des flottants, du fait de leur impr�cision).
	// Pour effectuer une comparaison approch�e, utiliser environEgal()
	// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
	// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)
	bool operator==(const Vecteur<2, T>& ref) const;
	bool operator!=(const Vecteur<2, T>& ref) const;
	bool operator>(const Vecteur<2, T>& ref) const;
	bool operator>=(const Vecteur<2, T>& ref) const;
	bool operator<(const Vecteur<2, T>& ref) const;
	bool operator<=(const Vecteur<2, T>& ref) const;

	// Acc�s aux �l�ments
	T& operator[](int index);

	// Acc�s aux �l�ments
	const T& operator[](int index) const;

	// Calcul de la norme
	double CalculerNorme() const;

	// Calcul de la norme au carree (economise le calcul de la racine carree)
	double CalculerNormeCarre() const;

	// Normalisation du vecteur
	void Normaliser();

	// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
	// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
	bool EnvironEgal(const Vecteur<2, T>& ref, double imprecisionMax=EPSILON) const;

	// Met le vecteur � 0.
	void Reset();

	// PARTICULIER POUR LE VECTEUR 2D :
	// Calcul de l'angle form� entre le vecteur et l'axe des x.
	double CalculerAngle() const;

	// Ajout d'un nouvel angle � celui form� par le vecteur
	Vecteur<2, T>& Tourner(double angle);

	// Tourner le vecteur pour qu'il forme l'angle pr�cis� en param�tres
	Vecteur<2, T>& SetAngle(double angle);
};

// D�terminant entre 2 vecteurs
template <typename T>
double Det(const Vecteur<2, T>& v1, const Vecteur<2, T>& v2);

/************** Vecteur<3, T> **************/
template <typename T>
class Vecteur<3, T>
{
protected:
	T elements[3];	// Composantes

public:
	// R�f�rences permettant d'acc�der directement aux composantes x et y
	// en lecture et �criture ainsi : v.x, v.y, v.z, ou v.r, v.g, v.b (pour des couleurs)
	T& x;
	T& y;
	T& z;

	T& r;
	T& g;
	T& b;

public:
	// Constructeurs / destructeur
	Vecteur<3, T>();
	Vecteur<3, T>(const Vecteur<3, T>& ref);
	Vecteur<3, T>(const T& x, const T& y, const T& z);

	template <typename T_scalaire>
	Vecteur(const T_scalaire& scalaire);

	~Vecteur();

	// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
	// toutes les composantes de v1 prendront la valeur du scalaire.
	Vecteur<3, T>& operator=(const Vecteur<3, T>& ref);

	template <typename T_scalaire>
	Vecteur<3, T>& operator=(const T_scalaire& scalaire);

	// Op�rations :
	// + : addition composante par composante
	// - : soustraction composante par composante
	// % : multiplication composante par composante
	// / : division composante par composante
	// * : produit scalaire (quand entre 2 vecteurs)
	// * : multiplication par un scalaire : v * s
	// / : division par un scalaire : v/s
	// NB : le produit vectoriel (^) est r�serv� � la sp�cialisation en 3 dimensions
	// (� savoir ICI)
	Vecteur<3, T> operator+(const Vecteur<3, T>& ref) const;
	Vecteur<3, T> operator-(const Vecteur<3, T>& ref) const;
	Vecteur<3, T> operator%(const Vecteur<3, T>& ref) const;
	Vecteur<3, T> operator/(const Vecteur<3, T>& ref) const;
	Vecteur<3, T> operator^(const Vecteur<3, T>& ref) const;

	template <typename T_scalaire>
	T_scalaire operator*(const Vecteur<3, T>& ref) const;	// produit scalaire

	template <typename T_scalaire>
	Vecteur<3, T> operator*(const T_scalaire& scalaire) const;	// v * s

	template <typename T_scalaire>
	Vecteur<3, T> operator/(const T_scalaire& scalaire) const;	// v / s

	// Op�rations-affectations : +=, -=, %=, /=, *=
	Vecteur<3, T>& operator+=(const Vecteur<3, T>& ref);
	Vecteur<3, T>& operator-=(const Vecteur<3, T>& ref);
	Vecteur<3, T>& operator%=(const Vecteur<3, T>& ref);
	Vecteur<3, T>& operator/=(const Vecteur<3, T>& ref);
	Vecteur<3, T>& operator^=(const Vecteur<3, T>& ref);

	template <typename T_scalaire>
	Vecteur<3, T>& operator*=(const T_scalaire& scalaire);

	template <typename T_scalaire>
	Vecteur<3, T>& operator/=(const T_scalaire& scalaire);

	// Comparaison
	// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
	// avec des flottants, du fait de leur impr�cision).
	// Pour effectuer une comparaison approch�e, utiliser environEgal()
	// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
	// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)
	bool operator==(const Vecteur<3, T>& ref) const;
	bool operator!=(const Vecteur<3, T>& ref) const;
	bool operator>(const Vecteur<3, T>& ref) const;
	bool operator>=(const Vecteur<3, T>& ref) const;
	bool operator<(const Vecteur<3, T>& ref) const;
	bool operator<=(const Vecteur<3, T>& ref) const;

	// Acc�s aux �l�ments
	T& operator[](int index);

	// Acc�s aux �l�ments
	const T& operator[](int index) const;

	// Calcul de la norme
	double CalculerNorme() const;

	// Normalisation du vecteur
	void Normaliser();

	// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
	// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
	bool EnvironEgal(const Vecteur<3, T>& ref, double imprecisionMax=EPSILON) const;

	// Met le vecteur � 0.
	void Reset();
};

// D�terminant entre 3 vecteurs
template <typename T>
double Det(const Vecteur<3, T>& v1, const Vecteur<3, T>& v2, const Vecteur<3, T>& v3);

// Les typedefs des types de vecteurs les + utilis�s
typedef Vecteur<2, unsigned char>	Vecteur2ub;
typedef Vecteur<2, int>				Vecteur2i;
typedef Vecteur<2, short>			Vecteur2s;
typedef Vecteur<2, unsigned short>	Vecteur2us;
typedef Vecteur<2, unsigned int>	Vecteur2ui;
typedef Vecteur<2, float>			Vecteur2f;
typedef Vecteur<2, double>			Vecteur2d;

typedef Vecteur<3, unsigned char>	Vecteur3ub;
typedef Vecteur<3, int>				Vecteur3i;
typedef Vecteur<3, short>			Vecteur3s;
typedef Vecteur<3, unsigned short>	Vecteur3us;
typedef Vecteur<3, unsigned int>	Vecteur3ui;
typedef Vecteur<3, float>			Vecteur3f;
typedef Vecteur<3, double>			Vecteur3d;

// Impl�mentations
#include "VecteurN.hpp"
#include "Vecteur2.hpp"
#include "Vecteur3.hpp"

#endif // VECTEUR_H
