// Vecteur2.hpp
// VecteurN/2/3.hpp : impl�mentations des classes repr�sentant des vecteurs
// Celles-ci sont  dans un .hpp car il est impossible avec tous les compilateurs existants
// � l'heure actuelle d'impl�menter s�par�ment des classes templates dans des fichiers .cpp,
// malgr� le fait que le standard C++ exige que cela soit possible ^^

#include <cmath>

// Constructeur
template <typename T>
Vecteur<2, T>::Vecteur() : x(this->elements[0]), y(this->elements[1])
{
	Reset();
}

// Constructeur de copie
template <typename T>
Vecteur<2, T>::Vecteur(const Vecteur<2, T>& ref) : x(this->elements[0]), y(this->elements[1])
{
	*this = ref;
}

// Constructeur � partir de coordonn�es
template <typename T>
Vecteur<2, T>::Vecteur(const T& x, const T& y) : x(this->elements[0]), y(this->elements[1])
{
	this->elements[0] = x;
	this->elements[1] = y;
}

// Constructeur � partir d'un scalaire
template <typename T>
template <typename T_scalaire>
Vecteur<2, T>::Vecteur(const T_scalaire& scalaire) : x(this->elements[0]), y(this->elements[1])
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] = T(scalaire);
}

// Destructeur
template <typename T>
Vecteur<2, T>::~Vecteur()
{
}

// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
// toutes les composantes de v1 prendront la valeur du scalaire.
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::operator=(const Vecteur<2, T>& ref)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] = ref.elements[i];

	return *this;
}

template <typename T>
template <typename T_scalaire>
Vecteur<2, T>& Vecteur<2, T>::operator=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] = T(scalaire);

	return *this;
}

// Op�rations :
// + : addition composante par composante
// - : soustraction composante par composante
// % : multiplication composante par composante
// / : division composante par composante
// * : produit scalaire (quand entre 2 vecteurs)
// * : multiplication par un scalaire : v * s
// / : division par un scalaire : v/s
// NB : le produit vectoriel (^) est r�serv� � la sp�cialisation en 3 dimensions

// - addition
template <typename T>
Vecteur<2, T> Vecteur<2, T>::operator+(const Vecteur<2, T>& ref) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] + ref.elements[i];
	return v;
}

// - soustraction
template <typename T>
Vecteur<2, T> Vecteur<2, T>::operator-(const Vecteur<2, T>& ref) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] - ref.elements[i];
	return v;
}

// - multiplication (par composante)
template <typename T>
Vecteur<2, T> Vecteur<2, T>::operator%(const Vecteur<2, T>& ref) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] * ref.elements[i];
	return v;
}

// - division (par composante)
template <typename T>
Vecteur<2, T> Vecteur<2, T>::operator/(const Vecteur<2, T>& ref) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] / ref.elements[i];
	return v;
}

// - produit scalaire
template <typename T>
template <typename T_scalaire>
T_scalaire Vecteur<2, T>::operator*(const Vecteur<2, T>& ref) const
{
	T_scalaire resultat = T_scalaire(0);
	for(int i=0 ; i<2 ; i++)
		resultat = resultat + this->elements[i] * ref.elements[i];
	return resultat;
}

// - multiplication par un scalaire : v * s
template <typename T>
template <typename T_scalaire>
Vecteur<2, T> Vecteur<2, T>::operator*(const T_scalaire& scalaire) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] * scalaire;
	return v;
}

// - division par un scalaire : v / s
template <typename T>
template <typename T_scalaire>
Vecteur<2, T> Vecteur<2, T>::operator/(const T_scalaire& scalaire) const
{
	Vecteur<2, T> v;
	for(int i=0 ; i<2 ; i++)
		v[i] = this->elements[i] / scalaire;
	return v;
}

// Op�rations-affectations : +=, -=, %=, /=, *=
// - "+="
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::operator+=(const Vecteur<2, T>& ref)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] += ref.elements[i];
	return *this;
}

// - "-="
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::operator-=(const Vecteur<2, T>& ref)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] -= ref.elements[i];
	return *this;
}

// - "%="
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::operator%=(const Vecteur<2, T>& ref)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] *= ref.elements[i];
	return *this;
}

// - "/="
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::operator/=(const Vecteur<2, T>& ref)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] /= ref.elements[i];
	return *this;
}

// - "*=" (avec un scalaire)
template <typename T>
template <typename T_scalaire>
Vecteur<2, T>& Vecteur<2, T>::operator*=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] *= scalaire;
	return *this;
}

// - "/=" (avec un scalaire)
template <typename T>
template <typename T_scalaire>
Vecteur<2, T>& Vecteur<2, T>::operator/=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] /= scalaire;
	return *this;
}

// Comparaison
// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
// avec des flottants, du fait de leur impr�cision).
// Pour effectuer une comparaison approch�e, utiliser Vecteur<2, T>::environEgal()
// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)

// - "=="
template <typename T>
bool Vecteur<2, T>::operator==(const Vecteur<2, T>& ref) const
{
	bool resultat = true;
	for(int i=0 ; i<2 ; i++)
		if(this->elements[i] != ref.elements[i])
		{
			resultat = false;
			break;
		}
	return resultat;
}

// - "!="
template <typename T>
bool Vecteur<2, T>::operator!=(const Vecteur<2, T>& ref) const
{
	return ! (*this == ref);
}

// - ">"
template <typename T>
bool Vecteur<2, T>::operator>(const Vecteur<2, T>& ref) const
{
	return this->CalculerNorme() > ref.CalculerNorme();
}

// - ">="
template <typename T>
bool Vecteur<2, T>::operator>=(const Vecteur<2, T>& ref) const
{
	return this->CalculerNorme() >= ref.CalculerNorme();
}

// - "<"
template <typename T>
bool Vecteur<2, T>::operator<(const Vecteur<2, T>& ref) const
{
	return this->CalculerNorme() < ref.CalculerNorme();
}

// - "<="
template <typename T>
bool Vecteur<2, T>::operator<=(const Vecteur<2, T>& ref) const
{
	return this->CalculerNorme() <= ref.CalculerNorme();
}

// Acc�s aux �l�ments
template <typename T>
T& Vecteur<2, T>::operator[](int index)
{
	return this->elements[index];
}

// Acc�s aux �l�ments
template <typename T>
const T& Vecteur<2, T>::operator[](int index) const
{
	return this->elements[index];
}

// Calcul de la norme
template <typename T>
double Vecteur<2, T>::CalculerNorme() const
{
	double resultat = 0.0;

	for(int i=0 ; i<2 ; i++)
		resultat += double(this->elements[i] * this->elements[i]);

	resultat = sqrt(resultat);
	return resultat;
}

// Calcul de la norme au carree (economise le calcul de la racine carree)
template <typename T>
double Vecteur<2, T>::CalculerNormeCarre() const
{
	double resultat = 0.0;

	for(int i=0 ; i<2 ; i++)
		resultat += double(this->elements[i] * this->elements[i]);
	return resultat;
}


// Normalisation du vecteur
template <typename T>
void Vecteur<2, T>::Normaliser()
{
	*this /= this->CalculerNorme();
}

// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
template <typename T>
bool Vecteur<2, T>::EnvironEgal(const Vecteur<2, T>& ref, double imprecisionMax) const
{
	bool resultat = true;
	for(int i=0 ; i<2 ; i++)
	{
		if(	double(this->elements[i]) - imprecisionMax < double(ref.elements[i]) ||
			double(this->elements[i]) + imprecisionMax > double(ref.elements[i]))
		{
			resultat = false;
			break;
		}
	}

	return resultat;
}

// Met le vecteur � 0.
template <typename T>
void Vecteur<2, T>::Reset()
{
	for(int i=0 ; i<2 ; i++)
		this->elements[i] = T(0.0);
}

// PARTICULIER POUR LE VECTEUR 2D :
// Calcul de l'angle form� entre le vecteur et l'axe des x.
template <typename T>
double Vecteur<2, T>::CalculerAngle() const
{
	return (x == 0 && y == 0) ? 0.0 : atan2(double(y), double(x)); // resultat non defini pour x = y = 0
}

// Ajout d'un nouvel angle � celui form� par le vecteur
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::Tourner(double angle)
{
	double norme = CalculerNorme();
	double angle_temp = CalculerAngle();

	angle_temp += angle;

	this->x = T(norme*cos(angle_temp));
	this->y = T(norme*sin(angle_temp));

	return *this;
}

// Tourner le vecteur pour qu'il forme l'angle pr�cis� en param�tres
template <typename T>
Vecteur<2, T>& Vecteur<2, T>::SetAngle(double angle)
{
	double norme = CalculerNorme();

	this->x = T(norme*cos(angle));
	this->y = T(norme*sin(angle));

	return *this;
}

// D�terminant entre 2 vecteurs
template <typename T>
double det(const Vecteur<2, T>& v1, const Vecteur<2, T>& v2)
{
	return double(v1.x*v2.y - v2.x*v1.y);
}
