//
// Created by benji on 21/01/17.
//

#ifndef ROOT_SPLINE_H
#define ROOT_SPLINE_H

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>

#include "Commun.h"

#define POINTS_MULTIPLIER \
	10 // Résolution plus fine du vecteur d'évaluation de la spline
	   // Entre chaque point donné, il y aura N points d'interpolation calculés

/**
 *                                                          ( A  B  ⁰  ⁰  ⁰  ⁰ )
 *                                                          ( ⁰  C  D  ⁰  ⁰  ⁰ )
 * Outil pour résoudre des équations de matrices de bande : ( ⁰  ⁰  E  F  ⁰  ⁰ )
 *                                                          ( ⁰  ⁰  ⁰  G  H  ⁰ )
 *                                                          ( ⁰  ⁰  ⁰  ⁰  I  J )
 */
class band_matrix {
private:
	std::vector<std::vector<double>> _upper; // bande supérieure
	std::vector<std::vector<double>> _lower; // bande inférieure
public:
	band_matrix() = default;
	band_matrix(int dim, int n_u, int n_l);

	void resize(int dim, int n_u, int n_l);
	int dim() const;
	int num_upper() const {
		return static_cast<int>(_upper.size() - 1);
	}

	int num_lower() const {
		return static_cast<int>(_lower.size() - 1);
	}

	/**
	 * Ces opérateurs permettent d'accéder aux éléments A(i, j)
	 * Les index i, j varient de 0 à dim() - 1
	 * @param i
	 * @param j
	 */
	double& operator()(int i, int j);      // écriture
	double operator()(int i, int j) const; // lecture

	// Permet d'ajouter une diagonale supplémentaire (sur la bande inférieure)
	double& saved_diag(int i);
	double saved_diag(int i) const;

	void lu_decompose();

	std::vector<double> r_solve(const std::vector<double>& b) const;

	std::vector<double> l_solve(const std::vector<double>& b) const;

	std::vector<double> lu_solve(const std::vector<double>& b, bool is_lu_decomposed = false);
};


/**
 * Cette classe permet de calculer l'interpolation par splines cubiques d'un ensemble de points de R²
 * Une cubique est calculée pour chacun des 3 points consécutifs
 * Un lissage entre les cubiques est garanti par une continuité C¹ ou C²
 *
 * Par défaut, les splines fi sont naturelles donc C² : f"_i = f"_(i+1) = 0.0
 */
class spline {
public:
	enum deriv_type { first_deriv = 1, second_deriv = 2 };

private:
	std::vector<double> _x, _y; // Coordonnées des points à interpoler (x,y)
	/// Paramètres d'interpolation
	// y(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i
	std::vector<double> _a, _b, _c; // a = y'''_i / 6   ;   b = y''_i / 2   ;   c = y'_i
	// pour l'extrapolation de gauche
	double _b0, _c0;
	deriv_type _left, _right;
	double _left_value, _right_value;
	// Extrapolation linéaire ou quelconque en dehors du domaine considéré?
	bool _force_linear_extrapolation;

public:
	/// Constructeur
	spline()
	        : _b0(0), _c0(0), _left(second_deriv), _right(second_deriv), _left_value(0.0), _right_value(0.0), _force_linear_extrapolation(false) {
		;
	}

	/**
	 * [Optionnel]
	 * Appeler la fonction avant set_points
	 *
	 * Permet de caractériser les conditions sur les extrapolations à droite et à gauche
	 * Initialement, f"(droite) = f"(gauche) = 0.0 (pas de courbure)
	 *
	 * @param left degré de dérivation (1 ou 2) pour l'extrapolation à gauche
	 * @param left_value valeur de la dérivée pour la cubique de gauche
	 * @param right degré de dérivation à droite
	 * @param right_value valeur de la dérivée à droite
	 * @param force_linear_extrapolation extrapolation linéaire ou quelconque en-dehors du domaine considéré?
	 */
	void set_boundary(deriv_type left, double left_value, deriv_type right, double right_value, bool force_linear_extrapolation = false);

	/**
	 * Initialise les points de la spline
	 * @param x Les valeurs doivent être croissantes
	 * @param y
	 * @param cubic_spline extrapolation cubique ou linéaire
	 */
	void set_points(const std::vector<double>& x, const std::vector<double>& y, bool cubic_spline = true);

	/**
	 * Calcul de y(x) selon l'extrapolation choisie
	 * @param x
	 * @return
	 */
	double operator()(double x) const;

	/**
	 * Réupère les valeurs des dérivées au point X
	 * @param x
	 * @return pair<first_deriv, second_deriv>
	 */
	std::pair<double, double> get_deriv(double x) const;

	/**
	 * Calcul de la courbure algébrique
	 * alpha[x, f(x)] = f"(x)/[1 + f'²(x)]^(3/2)
	 * Unité : [alpha] = m/rad
	 * @param x
	 * @return
	 */
	double get_curvature(double x) const;
};

struct CurvatureInfos {
	double speedCoeffL;
	double speedCoeffR;

	Distance length;
};

/**
 * Utile pour les interpolations avec des points 'quelconques'
 * Paramétrage de la spline suivant une variable discrète T dont la suite (Ti) est croissante
 * Deux splines sont calculées : X(T) et Y(T)
 *
 * Cette classe permet d'avoir en entrée des points dont l'interpolation a plusieurs Y avec un seul X
 */
class parameter_spline {
public:
	/// Constructeur
	parameter_spline() = default;

	/**
	 * Initialise les points de la spline
	 * @param x
	 * @param y
	 * @param cubic_spline extrapolation cubique ou linéaire
	 */
	void set_points(const std::vector<double>& x, const std::vector<double>& y, bool cubic_spline = true);
	void set_points(const std::vector<Vector2m>& pos, bool cubic_spline = true);

	/**
	 * Calcul des informations propres à la courbure algébrique
	 * alpha[x(t), y(t)] = [x'(t).y"(t) - y'(t).x"(t)]/[x'²(t) + t'²(t)]^(3/2)
	 * Unité : [alpha] = m/rad
	 *
	 * longueur de l'arc = intégrale de T_i à T_(i+1) de ||f'(t)||.dt = intégrale de sqrt[x²(t) + y²(t)].dt
	 * Unité : [longueur] = m
	 *
	 * @todo actuellement, length = sqrt([x(T)-x(T+1)]² + [y(T)-y(T+1)]²) (distance entre 2 points)
	 * @return pair<curvature, length>
	 */
	std::vector<std::pair<double, double>> get_curvature_infos_d() const;
	/**
	 * Le résultat pair<Length, Distance> est foireux
	 * La courbure algébrique est en m/rad mais l'unité la plus appropriée est 'Length' (en m)
	 * Il faudrait multiplier la valeur par l'angle parcouru mais comment le calculer??
	 * @todo trouver les bonnes formules et la bonne exploitation
	 * @return
	 */
	std::vector<std::pair<Length, Distance>> get_curvature_infos() const;

	/**
	 * Calcul des informations d'avance le long d'une courbe
	 * @param enter_axis = distance entre les deux cercles d'occultation de la spline
	 * @return Vector2   = <coeff de vitesse - roue gauche ; coeff de vitesse - roue droite>
	 * @return Distance  = distance d'avance jusqu'au prochain checkpoint
	 */
	std::vector<CurvatureInfos> get_curvature_advance(Distance enter_axis) const;

	/**
	 * Calcule l'ensemble des points de la courbe y(x)
	 * Résolution de la courbe : tous les POINTS_MULTIPLIER * nbr(T)
	 * @return pair<x, y>
	 */
	std::vector<std::pair<double, double>> get_points_d() const;
	std::vector<Vector2m> get_points() const;

	/**
	 * Calcul des dérivées première et seconde de l'ensemble des points
	 * @param X -> calcule les dérivées en X si 'true', en Y sinon
	 * @return vector<ordre 1, ordre 2>
	 */
	std::vector<std::pair<double, double>> get_derivs(bool X) const;

private:
	// Splines cubiques paramétrées x(T) et y(T)
	spline _splineX;
	spline _splineY;
	// Discrétisation des coordonnées
	// La distance entre le paramètre T dépend de la distance entre les points d'interpolation
	// Création de la suite croissante (Ti), i>=1 : T_0 = 0
	//                                              T_(i+1) = T_i + ||X_i - X_(i-1)||_2
	std::vector<double> _paramT;
	// Partition beaucoup plus fine que le paramètre T : on considère POINTS_MULTIPLIER points de plus
	// La spline paramétrée est évaluée en chacun des points [x(TT), y(TT)] pour trouver la meilleure courbure
	std::vector<double> _paramTT;

	// Initialisation des vecteurs T et TT
	void _create_parametric_vectors(const std::vector<double>& x, const std::vector<double>& y);

	void debug();
};


inline double toSplineCalculation(Distance const& d) {
	return d.toM<double>();
}

inline Distance fromSplineCalculationD(double const& d) {
	return Distance::makeFromM(d);
}

inline Vector2m fromSplineCalculation(double const& x, double const& y) {
	return Vector2m(fromSplineCalculationD(x), fromSplineCalculationD(y));
}

// Utilisé pour des courbures algébriques en m/rad ... unité ~ m ??
inline Length fromSplineCalculationCA(double const& ca) {
	return Length::makeFromM(ca);
}


#endif // ROOT_SPLINE_H
