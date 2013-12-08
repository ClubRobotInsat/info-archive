// Vecteur3.hpp
// VecteurN/2/3.hpp : impl�mentations des classes repr�sentant des vecteurs
// Celles-ci sont  dans un .hpp car il est impossible avec tous les compilateurs existants
// � l'heure actuelle d'impl�menter s�par�ment des classes templates dans des fichiers .cpp,
// malgr� le fait que le standard C++ exige que cela soit possible ^^

// Constructeur
template <typename T>
Vecteur<3, T>::Vecteur()
: x(this->elements[0]), y(this->elements[1]), z(this->elements[2]),
  r(this->elements[0]), g(this->elements[1]), b(this->elements[2])
{
	Reset();
}

// Constructeur de copie
template <typename T>
Vecteur<3, T>::Vecteur(const Vecteur<3, T>& ref)
: x(this->elements[0]), y(this->elements[1]), z(this->elements[2]),
  r(this->elements[0]), g(this->elements[1]), b(this->elements[2])
{
	*this = ref;
}

// Constructeur � partir des composantes
template <typename T>
Vecteur<3, T>::Vecteur(const T& x, const T& y, const T& z)
: x(this->elements[0]), y(this->elements[1]), z(this->elements[2]),
  r(this->elements[0]), g(this->elements[1]), b(this->elements[2])
{
	this->elements[0] = x;
	this->elements[1] = y;
	this->elements[2] = z;
}

// Constructeur � partir d'un scalaire
template <typename T>
template <typename T_scalaire>
Vecteur<3, T>::Vecteur(const T_scalaire& scalaire)
: x(this->elements[0]), y(this->elements[1]), z(this->elements[2]),
  r(this->elements[0]), g(this->elements[1]), b(this->elements[2])
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] = T(scalaire);
}

// Destructeur
template <typename T>
Vecteur<3, T>::~Vecteur()
{
}

// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
// toutes les composantes de v1 prendront la valeur du scalaire.
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator=(const Vecteur<3, T>& ref)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] = ref.elements[i];

	return *this;
}

template <typename T>
template <typename T_scalaire>
Vecteur<3, T>& Vecteur<3, T>::operator=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<3 ; i++)
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
// (� savoir ICI)

// - addition
template <typename T>
Vecteur<3, T> Vecteur<3, T>::operator+(const Vecteur<3, T>& ref) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] + ref.elements[i];
	return v;
}

// - soustraction
template <typename T>
Vecteur<3, T> Vecteur<3, T>::operator-(const Vecteur<3, T>& ref) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] - ref.elements[i];
	return v;
}

// - multiplication (par composante)
template <typename T>
Vecteur<3, T> Vecteur<3, T>::operator%(const Vecteur<3, T>& ref) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] * ref.elements[i];
	return v;
}

// - division (par composante)
template <typename T>
Vecteur<3, T> Vecteur<3, T>::operator/(const Vecteur<3, T>& ref) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] / ref.elements[i];
	return v;
}

// - produit vectoriel
template <typename T>
Vecteur<3, T> Vecteur<3, T>::operator^(const Vecteur<3, T>& ref) const
{
	Vecteur<3, T> v;
	v.x = this->y * ref.z - this->z * ref.y;
	v.y = - (this->x * ref.z - this->z * ref.x);
	v.z = this->x * ref.y - this->y * ref.x;
	return v;
}

// - produit scalaire
template <typename T>
template <typename T_scalaire>
T_scalaire Vecteur<3, T>::operator*(const Vecteur<3, T>& ref) const
{
	T_scalaire resultat = T_scalaire(0);
	for(int i=0 ; i<3 ; i++)
		resultat = resultat + this->elements[i] * ref.elements[i];
	return resultat;
}

// - multiplication par un scalaire : v * s
template <typename T>
template <typename T_scalaire>
Vecteur<3, T> Vecteur<3, T>::operator*(const T_scalaire& scalaire) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] * scalaire;
	return v;
}

// - division par un scalaire : v / s
template <typename T>
template <typename T_scalaire>
Vecteur<3, T> Vecteur<3, T>::operator/(const T_scalaire& scalaire) const
{
	Vecteur<3, T> v;
	for(int i=0 ; i<3 ; i++)
		v[i] = this->elements[i] / scalaire;
	return v;
}

// Op�rations-affectations : +=, -=, %=, /=, *=
// - "+="
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator+=(const Vecteur<3, T>& ref)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] += ref.elements[i];
	return *this;
}

// - "-="
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator-=(const Vecteur<3, T>& ref)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] -= ref.elements[i];
	return *this;
}

// - "%="
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator%=(const Vecteur<3, T>& ref)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] *= ref.elements[i];
	return *this;
}

// - "/="
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator/=(const Vecteur<3, T>& ref)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] /= ref.elements[i];
	return *this;
}

// - "^="
template <typename T>
Vecteur<3, T>& Vecteur<3, T>::operator^=(const Vecteur<3, T>& ref)
{
	double temp_x = this->x;
	double temp_y = this->y;

	this->x = temp_y * ref.z - this->z * ref.y;
	this->y = - (temp_x * ref.z - this->z * ref.x);
	this->z = temp_x * ref.y - temp_y * ref.x;

	return *this;
}

// - "*=" (avec un scalaire)
template <typename T>
template <typename T_scalaire>
Vecteur<3, T>& Vecteur<3, T>::operator*=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] *= scalaire;
	return *this;
}

// - "/=" (avec un scalaire)
template <typename T>
template <typename T_scalaire>
Vecteur<3, T>& Vecteur<3, T>::operator/=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] /= scalaire;
	return *this;
}

// Comparaison
// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
// avec des flottants, du fait de leur impr�cision).
// Pour effectuer une comparaison approch�e, utiliser Vecteur<3, T>::environEgal()
// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)

// - "=="
template <typename T>
bool Vecteur<3, T>::operator==(const Vecteur<3, T>& ref) const
{
	bool resultat = true;
	for(int i=0 ; i<3 ; i++)
		if(this->elements[i] != ref.elements[i])
		{
			resultat = false;
			break;
		}
	return resultat;
}

// - "!="
template <typename T>
bool Vecteur<3, T>::operator!=(const Vecteur<3, T>& ref) const
{
	return ! (*this == ref);
}

// - ">"
template <typename T>
bool Vecteur<3, T>::operator>(const Vecteur<3, T>& ref) const
{
	return this->CalculerNorme() > ref.CalculerNorme();
}

// - ">="
template <typename T>
bool Vecteur<3, T>::operator>=(const Vecteur<3, T>& ref) const
{
	return this->CalculerNorme() >= ref.CalculerNorme();
}

// - "<"
template <typename T>
bool Vecteur<3, T>::operator<(const Vecteur<3, T>& ref) const
{
	return this->CalculerNorme() < ref.CalculerNorme();
}

// - "<="
template <typename T>
bool Vecteur<3, T>::operator<=(const Vecteur<3, T>& ref) const
{
	return this->CalculerNorme() <= ref.CalculerNorme();
}

// Acc�s aux �l�ments
template <typename T>
T& Vecteur<3, T>::operator[](int index)
{
	return this->elements[index];
}

// Acc�s aux �l�ments
template <typename T>
const T& Vecteur<3, T>::operator[](int index) const
{
	return this->elements[index];
}

// Calcul de la norme
template <typename T>
double Vecteur<3, T>::CalculerNorme() const
{
	double resultat = 0.0;

	for(int i=0 ; i<3 ; i++)
		resultat += double(this->elements[i] * this->elements[i]);

	resultat = sqrt(resultat);
	return resultat;
}

// Normalisation du vecteur
template <typename T>
void Vecteur<3, T>::Normaliser()
{
	*this /= this->CalculerNorme();
}

// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
template <typename T>
bool Vecteur<3, T>::EnvironEgal(const Vecteur<3, T>& ref, double imprecisionMax) const
{
	bool resultat = true;
	for(int i=0 ; i<3 ; i++)
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
void Vecteur<3, T>::Reset()
{
	for(int i=0 ; i<3 ; i++)
		this->elements[i] = T(0.0);
}

// D�terminant entre 3 vecteurs
// Calcul� avec le produit mixte :)
template <typename T>
double Det(const Vecteur<3, T>& v1, const Vecteur<3, T>& v2, const Vecteur<3, T>& v3)
{
	//return v1 * (v2 ^ v3);
	return 0.0;
}
