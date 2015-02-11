//
//  Unit.h
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Unit_h
#define Unit_h

#include <cmath>
#include <limits>
#include "../log/Log.h"

using std::abs;
using std::atan2;
using std::sqrt;

// g++ même dans la version 4.9 ne se conforme pas au standard C++14 pour l'utilisation de fonctions constepxr
#if __clang__
#define CONDITIONAL_CONSTEXPR constexpr
#else
#define CONDITIONAL_CONSTEXPR
#endif

class UnitBase {};

template<int Kg, int M, int S, bool spec>
class Unit : public UnitBase {};

/**
 * Classe destinée à contenir une valeur numérique représentant une grandeur physique.
 * Elle est destinée à être la classe de base d'une hiérarchier d'autres classes.
 * L'instanciation de cette classe sans classe fille n'a pas vraiment d'intérêt.
 * Pour créer une classe fille : class MyValue : public NumericValue<MyValue, double> {…};
 *
 * @param T le type de classe fille. C'est lui qui différencie deux grandeurs physiques différentes et empêche la conversion implicite entre elles.
 * @param ValType le type destiné à stocker les valeurs numériques de la grandeur (float, double, long double voire même entier en fonction de la précision voulue).
 */
template<int Kg, int M, int S>
class Unit<Kg, M, S, false> : public UnitBase  {
	template<int Kg1, int M1, int S1>
	using DerivedType = Unit<Kg1, M1, S1, true>;
public:
	using ValueType = double;

	/**
	 * Constructeur par défaut.
	 * @return L'instance avec la valeur par défaut de ValueType (diverses représentations de 0 en principe)
	 */
	constexpr Unit() : _val(ValueType()) {}
	/**
	 * Copie la valeur numérique.
	 * @param v la valeur à copier, de la même grandeur physique que *this.
	 * @return la nouvelle valeur.
	 */
	Unit(DerivedType<Kg, M, S> const &v) : _val(v._val) {}

	/**
	 * Créé une grandeur physique avec la valeur numérique nue.
	 * Par exemple, une grandeur de longueur stockant des mètres et créée avec cette fonction et une valeur de 1 représentera une valeur de 1 mètre.
	 * @param v la valeur numérique à assigner à la grandeur.
	 * @return une instance de la grandeur physique représentant la valeur demandée.
	 */
	static constexpr DerivedType<Kg, M, S> makeFromValue(ValueType v) {
		return DerivedType<Kg, M, S>(v);
	}

	/**
	 * Retourne la valeur maximale de la grandeur physique représentable.
	 * Dépent uniquement de l'amplitude autorisée par ValueType.
	 */
	static constexpr DerivedType<Kg, M, S> max() {
		return DerivedType<Kg, M, S>(std::numeric_limits<ValueType>::max());
	}

	/**
	 * Échange les valeurs de deux instances de même grandeur physique.
	 * @param v1 la 1ère valeur, qui contiendra la valeur de v2 à la fin de l'appel
	 * @param v2 la 2e valeur, qui contiendra la valeur de v1 à la fin de l'appel
	 */
	constexpr friend void swap(DerivedType<Kg, M, S> &v1, DerivedType<Kg, M, S> const &v2) {
		using std::swap;
		swap(v1._val, v2._val);
	}

	/**
	 * Opérateur d'affectation
	 * @param v l'instance de même grandeur physique que *this qui contient la valeur à récupérer
	 * @return l'instance courante, après que sa grandeur ait été remplacée par celle contenue dans le paramètre.
	 */
	CONDITIONAL_CONSTEXPR DerivedType<Kg, M, S> &operator=(DerivedType<Kg, M, S> v) {
		std::swap(static_cast<DerivedType<Kg, M, S> &>(*this), v);
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}

	/**
	 * Opérateur de négation unaire.
	 * @return une copie de l'instance courante de valeur opposée.
	 */
	constexpr DerivedType<Kg, M, S> operator-() const {
		return makeFromValue(-_val);
	}

	/**
	 * Plus unaire.
	 * @return une copie de l'instance courante de valeur identique.
	 */
	constexpr DerivedType<Kg, M, S> operator+() const {
		return makeFromValue(_val);
	}

	/**
	 * Ajoute la valeur du paramètre à l'instance courante.
	 * Le paramètre doit être homogène à l'instance courante.
	 * @param val la valeur qu'on veut ajouter à la valeur de l'instance courante
	 * @return l'instance courante à laquelle a été ajoutée la valeur du paramètre
	 */
	CONDITIONAL_CONSTEXPR DerivedType<Kg, M, S> &operator+=(DerivedType<Kg, M, S> const &val) {
		_val += val._val;
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}
	/**
	 * Retourne la somme des deux grandeurs physiques.
	 * Les deux grandeurs doivent être homogènes.
	 * @param v1 le premier terme de la somme
	 * @param v2 le deuxième terme de la somme
	 * @return une nouvelle instance représentant la somme de la valeur de l'instance courante avec la valeur du paramètre
	 */
	constexpr friend DerivedType<Kg, M, S> operator+(DerivedType<Kg, M, S> const &v1, DerivedType<Kg, M, S> const &v2) {
		return makeFromValue(v1._val + v2._val);
	}

	/**
	 * Soustrait la valeur du paramètre à l'instance courante.
	 * Le paramètre doit être homogène à l'instance courante.
	 * @param val la valeur qu'on veut soustraire à la valeur de l'instance courante
	 * @return l'instance courante à laquelle a été soustraite la valeur du paramètre
	 */
	CONDITIONAL_CONSTEXPR DerivedType<Kg, M, S> &operator-=(DerivedType<Kg, M, S> const &val) {
		_val -= val._val;
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}
	/**
	 * Retourne la soustraction des deux grandeurs physiques.
	 * Les deux grandeurs doivent être homogènes.
	 * @param v1 le premier terme de la différence
	 * @param v2 le deuxième terme de la différence
	 * @return une nouvelle instance représentant la différence de la valeur de l'instance courante avec la valeur du paramètre
	 */
	constexpr friend DerivedType<Kg, M, S> operator-(DerivedType<Kg, M, S> const &v1, DerivedType<Kg, M, S> const &v2) {
		return makeFromValue(v1._val - v2._val);
	}

	/**
	 * Multiple l'instance courante par la valeur du paramètre.
	 * Le paramètre doit ne doit pas représenter une grandeur physique (il doit être un simple scalaire).
	 * En revanche, il doit correspondre au type trait std::is_arithmetic.
	 * @param val la valeur par laquelle la valeur de l'instance courante sera multipliée
	 * @return l'instance courante multipliée par le paramètre
	 */
	template<typename U>
	CONDITIONAL_CONSTEXPR std::enable_if_t<!std::is_base_of<UnitBase, U>::value, DerivedType<Kg, M, S> &>
	operator*=(U val) {
		_val *= val;
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}
	/**
	 * Retourne le produit d'une grandeur physique avec un nombre.
	 * Le nombre ne doit pas représenter une grandeur physique (il doit être un simple scalaire).
	 * En revanche, il doit correspondre au type trait std::is_arithmetic.
	 *
	 *
	 * @param v1 le premier facteur du produit
	 * @param v2 le deuxième facteur du produit
	 * @return une nouvelle instance représentant la multiplication des deux paramètres
	 */
	template<typename U>
	constexpr friend std::enable_if_t<!std::is_base_of<UnitBase, U>::value && std::is_arithmetic<U>::value, DerivedType<Kg, M, S>>
	operator*(DerivedType<Kg, M, S> const &v1, U v2) {
		return makeFromValue(v1._val * v2);
	}

	/**
	 * Pareil qu'au dessus
	 */
	template<typename U>
	constexpr friend std::enable_if_t<!std::is_base_of<UnitBase, U>::value, DerivedType<Kg, M, S>>
	operator*(U v1, DerivedType<Kg, M, S> const &v2) {
		return v2 * v1;
	}

	/**
	 * Retourne le produit d'une grandeur physique avec une autre grandeur physique.
	 * Le type de retour est la grandeur physique correspondant (ex : vitesse * temps => distance)
	 *
	 * @param v1 le premier facteur du produit
	 * @param v2 le deuxième facteur du produit
	 * @return une nouvelle instance représentant la multiplication des deux paramètres
	 */
	template<int Kg1, int M1, int S1, int Kg2, int M2, int S2>
	constexpr friend auto operator*(DerivedType<Kg1, M1, S1> const &t1, DerivedType<Kg2, M2, S2> const &t2) -> DerivedType<Kg1 + Kg2, M1 + M2, S1 + S2> {
		return DerivedType<Kg1 + Kg2, M1 + M2, S1 + S2>::makeFromValue(t1._val * t2._val);
	}

	/**
	 * Divise l'instance courante par la valeur du paramètre.
	 * Le paramètre doit ne doit pas représenter une grandeur physique (il doit être un simple scalaire).
	 * En revanche, il doit correspondre au type trait std::is_arithmetic.
	 * @param val la valeur par laquelle la valeur de l'instance courante sera divisée
	 * @return l'instance divisée par le paramètre
	 */
	template<typename U>
	CONDITIONAL_CONSTEXPR std::enable_if_t<std::is_arithmetic<U>::value, DerivedType<Kg, M, S> &>
	operator/=(DerivedType<Kg, M, S> val) {
		_val /= val;
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}
	/**
	 * Retourne le quotient d'une grandeur physique avec un nombre.
	 * Le nombre ne doit pas représenter une grandeur physique (il doit être un simple scalaire).
	 * En revanche, il doit correspondre au type trait std::is_arithmetic.
	 *
	 * @param v1 le dividende de la division
	 * @param v2 le diviseur de la division
	 * @return une nouvelle instance représentant la division des deux paramètres
	 */
	template<typename U>
	constexpr friend std::enable_if_t<std::is_arithmetic<U>::value, DerivedType<Kg, M, S>>
	operator/(DerivedType<Kg, M, S> const &v1, U v2) {
		return makeFromValue(v1._val / v2);
	}

	/**
	 * Retourne le quotient de deux grandeurs physiques. Le type de retour est la grandeur physique correspondant au quotient.
	 *
	 * @param v1 le dividende de la division
	 * @param v2 le diviseur de la division
	 * @return une nouvelle instance représentant la division des deux paramètres
	 */
	template<int Kg1, int M1, int S1, int Kg2, int M2, int S2>
	constexpr friend auto operator/(DerivedType<Kg1, M1, S1> const &t1, DerivedType<Kg2, M2, S2> const &t2) -> DerivedType<Kg1 - Kg2, M1 - M2, S1 - S2> {
		return DerivedType<Kg1 - Kg2, M1 - M2, S1 - S2>::makeFromValue(t1._val / t2._val);
	}

	template<int Kg1, int M1, int S1>
	constexpr friend auto operator/(DerivedType<Kg1, M1, S1> const &t1, DerivedType<Kg1, M1, S1> const &t2) -> ValueType {
		return t1._val / t2._val;
	}

	/**
	 * Affecte le reste de la division de l'instance courante par le paramètre à l'instance courante.
	 * @param val le diviseur de la division
	 * @return l'instance courante, après que le reste de la division lui ait été affecté
	 */
	CONDITIONAL_CONSTEXPR DerivedType<Kg, M, S> &operator%=(DerivedType<Kg, M, S> val) {
		_val = std::fmod(_val, val._val);
		return static_cast<DerivedType<Kg, M, S> &>(*this);
	}

	/**
	 * Retourne le reste de la division des deux grandeurs physiques.
	 * @param v1 le dividende de la division
	 * @param v2 le diviseur de la division
	 * @return le reste de la division
	 */
	constexpr friend DerivedType<Kg, M, S> operator%(DerivedType<Kg, M, S> const & v1, DerivedType<Kg, M, S> v2) {
		return makeFromValue(std::fmod(v1._val, v2._val));
	}

	/**
	 * Teste l'égalité des deux grandeurs physiques.
	 * Si ValueType est un nombre en virgule flottante de précision limitée, attention, un test avec une tolérance sera peut-être préférable.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si les valeurs sont égales, false sinon
	 */
	constexpr friend bool operator==(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return val1._val == val2._val;
	}

	/**
	 * Teste l'égalité des deux grandeurs physiques.
	 * Si ValueType est un nombre en virgule flottante de précision limitée, attention, un test avec une tolérance sera peut-être préférable.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si les valeurs sont différentes, false sinon
	 */
	constexpr friend bool operator!=(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return !(val1 == val2);
	}

	/**
	 * Compare les deux grandeurs physiques.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si la première valeur est inférieure à la deuxième, false sinon
	 */
	constexpr friend bool operator<(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return val1._val < val2._val;
	}

	/**
	 * Compare les deux grandeurs physiques.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si la première valeur est supérieure à la deuxième, false sinon
	 */
	constexpr friend bool operator>(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return val2._val < val1._val;
	}

	/**
	 * Compare les deux grandeurs physiques.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si la première valeur est inférieure ou égale à la deuxième, false sinon
	 */
	constexpr friend bool operator<=(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return !(val1._val > val2._val);
	}

	/**
	 * Compare les deux grandeurs physiques.
	 * @param val1 le premier terme de la comparaison
	 * @param val2 le deuxième terme de la comparaison
	 * @return true si la première valeur est supérieure ou égale à la deuxième, false sinon
	 */
	constexpr friend bool operator>=(DerivedType<Kg, M, S> const &val1, DerivedType<Kg, M, S> const &val2) {
		return !(val1._val < val2._val);
	}

	/**
	 * Retourne la valeur absolue de la valeur.
	 * @param val la valeur dont on veut retourner la valeur absolue
	 * @return la paramètre s'il est supérieur ou égal à 0, son opposé sinon.
	 */
	constexpr friend DerivedType<Kg, M, S> abs(DerivedType<Kg, M, S> const &val) {
		return val >= DerivedType<Kg, M, S>() ? val : -val;
	}

	/**
	 * Retourne le cosinus de la valeur.
	 * @param val la valeur dont on veut retourner le cosinus
	 * @return un nombre de type ValueType représentant le cosinus de la valeur de l'instance
	 */
	constexpr friend ValueType cos(DerivedType<Kg, M, S> const &val) {
		return std::cos(val._val);
	}
	/**
	 * Retourne le sinus de la valeur.
	 * @param val la valeur dont on veut retourner le sinus
	 * @return un nombre de type ValueType représentant le sinus de la valeur de l'instance
	 */
	constexpr friend ValueType sin(DerivedType<Kg, M, S> const &val) {
		return std::sin(val._val);
	}

protected:
	/**
	 * Accède à la valeur numérique stockée par l'instance.
	 * Accessible uniquement aux classes filles.
	 * Par défault, le type de la valeur retourné est ValueType, et donc capable de retourner directement la valeur de l'instance.
	 * En revanche, si le type de retour est spécifié, une vérification est effectuée pour être sûr que la conversion est sans overflow.
	 * Si c'est le cas, on affiche une erreur, et la valeur retournée représente une valeur incohérente.
	 * Ce test n'est pas effectué si la macro UNIT_NO_OVERFLOW_CHECK est définie, mais la valeur retournée sera bien sûr toujours incohérente.
	 *
	 * @return la valeur numérique de l'instance.
	 */
	template<typename U = ValueType>
	CONDITIONAL_CONSTEXPR U value() const {
#ifndef UNIT_NO_OVERFLOW_CHECK
		if(_val > std::numeric_limits<U>::max() || _val < std::numeric_limits<U>::lowest()) {
			logError("<ACHTUNG !>");
			logError("Le type vers lequel est convertie la grandeur physique ne peut pas représenter la valeur actuelle de la grandeur !");
			logError("C'est grave ! FIXME !");
			logError("</ACHTUNG !>");
		}
#endif // UNIT_NO_OVERFLOW_CHECK
		return static_cast<U>(_val);
	}

	constexpr Unit(float val) : _val(val) {}
	
	ValueType _val;
};

template<int Kg, int M, int S>
class Unit<Kg, M, S, true> : public Unit<Kg, M, S, false> {
	friend class Unit<Kg, M, S, false>;
	using Unit<Kg, M, S, false>::Unit;
};

#endif
