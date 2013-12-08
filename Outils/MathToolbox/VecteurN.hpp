// VecteurN.hpp
// VecteurN/2/3.hpp : impl�mentations des classes repr�sentant des vecteurs
// Celles-ci sont  dans un .hpp car il est impossible avec tous les compilateurs existants
// � l'heure actuelle d'impl�menter s�par�ment des classes templates dans des fichiers .cpp,
// malgr� le fait que le standard C++ exige que cela soit possible ^^

// Constructeur
template <int N, typename T>
Vecteur<N, T>::Vecteur()
{
	Reset();
}

// Constructeur de copie
template <int N, typename T>
Vecteur<N, T>::Vecteur(const Vecteur<N, T>& ref)
{
	*this = ref;
}

// Constructeur � partir d'un scalaire
template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T>::Vecteur(const T_scalaire& scalaire)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] = T(scalaire);
}

// Destructeur
template <int N, typename T>
Vecteur<N, T>::~Vecteur()
{
}

// Affectation : on peut faire v1 = v2 ou v1 = scalaire, auquel cas
// toutes les composantes de v1 prendront la valeur du scalaire.
template <int N, typename T>
Vecteur<N, T>& Vecteur<N, T>::operator=(const Vecteur<N, T>& ref)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] = ref.elements[i];

	return *this;
}

template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T>& Vecteur<N, T>::operator=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<N ; i++)
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
template <int N, typename T>
Vecteur<N, T> Vecteur<N, T>::operator+(const Vecteur<N, T>& ref) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] + ref.elements[i];
	return v;
}

// - soustraction
template <int N, typename T>
Vecteur<N, T> Vecteur<N, T>::operator-(const Vecteur<N, T>& ref) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] - ref.elements[i];
	return v;
}

// - multiplication (par composante)
template <int N, typename T>
Vecteur<N, T> Vecteur<N, T>::operator%(const Vecteur<N, T>& ref) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] * ref.elements[i];
	return v;
}

// - division (par composante)
template <int N, typename T>
Vecteur<N, T> Vecteur<N, T>::operator/(const Vecteur<N, T>& ref) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] / ref.elements[i];
	return v;
}

// - produit scalaire
/*template <int N, typename T>
template <typename T_scalaire>
T_scalaire Vecteur<N, T>::operator*(const Vecteur<N, T>& ref) const
{
	T_scalaire resultat = T_scalaire(0);
	for(int i=0 ; i<N ; i++)
		resultat = resultat + this->elements[i] * ref.elements[i];
	return resultat;
}
*/

// - multiplication par un scalaire : v * s
template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T> Vecteur<N, T>::operator*(const T_scalaire& scalaire) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] * scalaire;
	return v;
}

// - division par un scalaire : v / s
template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T> Vecteur<N, T>::operator/(const T_scalaire& scalaire) const
{
	Vecteur<N, T> v;
	for(int i=0 ; i<N ; i++)
		v[i] = this->elements[i] / scalaire;
	return v;
}

// Op�rations-affectations : +=, -=, %=, /=, *=
// - "+="
template <int N, typename T>
Vecteur<N, T>& Vecteur<N, T>::operator+=(const Vecteur<N, T>& ref)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] += ref.elements[i];
	return *this;
}

// - "-="
template <int N, typename T>
Vecteur<N, T>& Vecteur<N, T>::operator-=(const Vecteur<N, T>& ref)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] -= ref.elements[i];
	return *this;
}

// - "%="
template <int N, typename T>
Vecteur<N, T>& Vecteur<N, T>::operator%=(const Vecteur<N, T>& ref)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] *= ref.elements[i];
	return *this;
}

// - "/="
template <int N, typename T>
Vecteur<N, T>& Vecteur<N, T>::operator/=(const Vecteur<N, T>& ref)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] /= ref.elements[i];
	return *this;
}

// - "*=" (avec un scalaire)
template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T>& Vecteur<N, T>::operator*=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] *= scalaire;
	return *this;
}

// - "/=" (avec un scalaire)
template <int N, typename T>
template <typename T_scalaire>
Vecteur<N, T>& Vecteur<N, T>::operator/=(const T_scalaire& scalaire)
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] /= scalaire;
	return *this;
}

// Comparaison
// NB : l'op�rateur == effectue une comparaison EXACTE (faire attention lors de l'utilisation
// avec des flottants, du fait de leur impr�cision).
// Pour effectuer une comparaison approch�e, utiliser Vecteur<N, T>::environEgal()
// NB : pour les comparaisons >, >=, < et <=, celles-ci se font au niveau des normes
// des vecteurs (qui sont donc recalcul�es � chaque comparaison !)

// - "=="
template <int N, typename T>
bool Vecteur<N, T>::operator==(const Vecteur<N, T>& ref) const
{
	bool resultat = true;
	for(int i=0 ; i<N ; i++)
		if(this->elements[i] != ref.elements[i])
		{
			resultat = false;
			break;
		}
	return resultat;
}

// - "!="
template <int N, typename T>
bool Vecteur<N, T>::operator!=(const Vecteur<N, T>& ref) const
{
	return ! (*this == ref);
}

// - ">"
template <int N, typename T>
bool Vecteur<N, T>::operator>(const Vecteur<N, T>& ref) const
{
	return this->CalculerNorme() > ref.CalculerNorme();
}

// - ">="
template <int N, typename T>
bool Vecteur<N, T>::operator>=(const Vecteur<N, T>& ref) const
{
	return this->CalculerNorme() >= ref.CalculerNorme();
}

// - "<"
template <int N, typename T>
bool Vecteur<N, T>::operator<(const Vecteur<N, T>& ref) const
{
	return this->CalculerNorme() < ref.CalculerNorme();
}

// - "<="
template <int N, typename T>
bool Vecteur<N, T>::operator<=(const Vecteur<N, T>& ref) const
{
	return this->CalculerNorme() <= ref.CalculerNorme();
}

// Acc�s aux �l�ments
template <int N, typename T>
T& Vecteur<N, T>::operator[](int index)
{
	return this->elements[index];
}

// Acc�s aux �l�ments
template <int N, typename T>
const T& Vecteur<N, T>::operator[](int index) const
{
	return this->elements[index];
}

// Calcul de la norme
template <int N, typename T>
double Vecteur<N, T>::CalculerNorme() const
{
	double resultat = 0.0;

	for(int i=0 ; i<N ; i++)
		resultat += double(this->elements[i] * this->elements[i]);

	resultat = sqrt(resultat);
	return resultat;
}

// Normalisation du vecteur
template <int N, typename T>
void Vecteur<N, T>::Normaliser()
{
	*this /= this->CalculerNorme();
}

// Comparaison approch�e : renvoie true si et seulement si toutes les composantes
// des 2 vecteurs sont �gales � + ou - imprecisionMax pr�s.
template <int N, typename T>
bool Vecteur<N, T>::EnvironEgal(const Vecteur<N, T>& ref, double imprecisionMax) const
{
	bool resultat = true;
	for(int i=0 ; i<N ; i++)
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
template <int N, typename T>
void Vecteur<N, T>::Reset()
{
	for(int i=0 ; i<N ; i++)
		this->elements[i] = T(0.0);
}

// Affichage sur la console ou dans un fichier (pour le d�bogage)
// NB : on consid�re que l'utilisateur n'est pas assez con pour faire un vecteur
// de dimension 0 ^^
template <int N, typename T>
std::ostream& operator<<(std::ostream& stream, const Vecteur<N, T>& vec)
{
	stream << "[";
	for(int i=0 ; i<N-1 ; i++)
		stream << vec[i] << ", ";
	stream << vec[N-1] << "]";
	return stream;
}

// Multiplication par un scalaire, dans l'autre sens : s * v
template <int N, typename T, typename T_scalaire>
Vecteur<N, T> operator*(const T_scalaire& s, Vecteur<N, T> v)
{
	Vecteur<N, T> resultat;
	for(int i=0 ; i<N ; i++)
		resultat[i] = s * v[i];
	return resultat;
}

// Division par un scalaire, dans l'autre sens : s / v
template <int N, typename T, typename T_scalaire>
Vecteur<N, T> operator/(const T_scalaire& s, Vecteur<N, T> v)
{
	Vecteur<N, T> resultat;
	for(int i=0 ; i<N ; i++)
		resultat[i] = s / v[i];
	return resultat;
}

// Produit scalaire, entre 2 vecteurs
template <int N, typename T>
double operator*(Vecteur<N, T>& op1, Vecteur<N, T>& op2)
{
	double resultat = 0.0;
	for(int i=0 ; i<N ; i++)
		resultat = resultat + double(op1[i] * op2[i]);
	return resultat;
}
