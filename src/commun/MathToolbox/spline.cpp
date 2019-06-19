//
// Created by benji on 21/01/17.
//

#include "spline.h"

/// Implémentation de band_matrix
/// ------------------------------
band_matrix::band_matrix(int dim, int n_u, int n_l) {
	resize(dim, n_u, n_l);
}

void band_matrix::resize(int dim, int n_u, int n_l) {
	assert(dim > 0);
	assert(n_u >= 0);
	assert(n_l >= 0);
	_upper.resize(static_cast<unsigned long>(n_u + 1));
	_lower.resize(static_cast<unsigned long>(n_l + 1));
	for(size_t i = 0; i < _upper.size(); i++) {
		_upper[i].resize(static_cast<unsigned long>(dim));
	}
	for(size_t i = 0; i < _lower.size(); i++) {
		_lower[i].resize(static_cast<unsigned long>(dim));
	}
}

int band_matrix::dim() const {
	if(!_upper.empty()) {
		return static_cast<int>(_upper[0].size());
	} else {
		return 0;
	}
}

double& band_matrix::operator()(int i, int j) {
	int k = j - i; // Représente quelle bande?
	assert((i >= 0) && (i < dim()) && (j >= 0) && (j < dim()));
	assert((-num_lower() <= k) && (k <= num_upper()));
	// k = 0 : diagonal
	// k < 0 : partie en bas à gauche
	// k > 0 : partie en haut à droite
	if(k >= 0)
		return _upper[k][i];
	else
		return _lower[-k][i];
}

double band_matrix::operator()(int i, int j) const {
	int k = j - i; // Représente quelle bande?
	assert((i >= 0) && (i < dim()) && (j >= 0) && (j < dim()));
	assert((-num_lower() <= k) && (k <= num_upper()));
	// k = 0 : diagonal
	// k < 0 : partie en bas à gauche
	// k > 0 : partie en haut à droite
	if(k >= 0)
		return _upper[k][i];
	else
		return _lower[-k][i];
}

// Dexuième diagonale sauvegardée dans _lower (utilisé pour la décomposition LU)
double band_matrix::saved_diag(int i) const {
	assert((i >= 0) && (i < dim()));
	return _lower[0][i];
}

double& band_matrix::saved_diag(int i) {
	assert((i >= 0) && (i < dim()));
	return _lower[0][i];
}

// LR-Décomposition d'une matrice bande
void band_matrix::lu_decompose() {
	int i_max, j_max;
	int j_min;
	double x;

	// Normalise la colonne i avec a_ii = 1 (préconditionnement)
	for(int i = 0; i < this->dim(); i++) {
		assert(this->operator()(i, i) != 0.0);
		this->saved_diag(i) = 1.0 / this->operator()(i, i);
		j_min = std::max(0, i - this->num_lower());
		j_max = std::min(this->dim() - 1, i + this->num_upper());
		for(int j = j_min; j <= j_max; j++) {
			this->operator()(i, j) *= this->saved_diag(i);
		}
		this->operator()(i, i) = 1.0; // Evite les erreurs d'arrondissement
	}

	// LR-Décomposition de Gauss
	for(int k = 0; k < this->dim(); k++) {
		i_max = std::min(this->dim() - 1, k + this->num_lower());
		for(int i = k + 1; i <= i_max; i++) {
			assert(this->operator()(k, k) != 0.0);
			x = -this->operator()(i, k) / this->operator()(k, k);
			this->operator()(i, k) = -x; // partie gauche
			j_max = std::min(this->dim() - 1, k + this->num_upper());
			for(int j = k + 1; j <= j_max; j++) {
				// partie droite
				this->operator()(i, j) = this->operator()(i, j) + x * this->operator()(k, j);
			}
		}
	}
}

// Résolution de Ly = b
std::vector<double> band_matrix::l_solve(const std::vector<double>& b) const {
	assert(this->dim() == static_cast<int>(b.size()));
	std::vector<double> x(static_cast<unsigned long>(this->dim()));
	int j_start;
	double sum;
	for(int i = 0; i < this->dim(); i++) {
		sum = 0;
		j_start = std::max(0, i - this->num_lower());
		for(int j = j_start; j < i; j++)
			sum += this->operator()(i, j) * x[j];
		x[i] = (b[i] * this->saved_diag(i)) - sum;
	}
	return x;
}

// Résolution de Rx = y
std::vector<double> band_matrix::r_solve(const std::vector<double>& b) const {
	assert(this->dim() == static_cast<int>(b.size()));
	std::vector<double> x(static_cast<unsigned long>(this->dim()));
	int j_stop;
	double sum;
	for(int i = this->dim() - 1; i >= 0; i--) {
		sum = 0;
		j_stop = std::min(this->dim() - 1, i + this->num_upper());
		for(int j = i + 1; j <= j_stop; j++)
			sum += this->operator()(i, j) * x[j];
		x[i] = (b[i] - sum) / this->operator()(i, i);
	}
	return x;
}

std::vector<double> band_matrix::lu_solve(const std::vector<double>& b, bool is_lu_decomposed) {
	assert(this->dim() == static_cast<int>(b.size()));
	std::vector<double> x, y;
	if(!is_lu_decomposed) {
		this->lu_decompose();
	}
	y = this->l_solve(b);
	x = this->r_solve(y);
	return x;
}


/// Implémentation de spline
/// -------------------------
void spline::set_boundary(spline::deriv_type left, double left_value, spline::deriv_type right, double right_value, bool force_linear_extrapolation) {
	assert(_x.empty()); // Cette méthode doit être appelée avant set_points

	_left = left;
	_right = right;
	_left_value = left_value;
	_right_value = right_value;
	_force_linear_extrapolation = force_linear_extrapolation;
}

void spline::set_points(const std::vector<double>& x, const std::vector<double>& y, bool cubic_spline) {
	assert(x.size() == y.size());
	assert(x.size() > 2);
	_x = x;
	_y = y;
	int n = static_cast<int>(x.size());

	for(int i = 0; i < n - 1; i++) {
		// La suite (x) doit être croissante
		// Utiliser la classe parameter_spline si (x) et (y) non croissants
		assert(_x[i] < _x[i + 1]);
	}

	if(cubic_spline) { // On réalise une interpolation par splines cubiques
		// On initialise la matrice du côté droit du système d'équation pour le paramètre b[] = f" / 2
		band_matrix A(n, 1, 1);
		std::vector<double> rhs(static_cast<unsigned long>(n));
		for(int i = 1; i < n - 1; i++) {
			A(i, i - 1) = 1.0 / 3.0 * (x[i] - x[i - 1]);
			A(i, i) = 2.0 / 3.0 * (x[i + 1] - x[i - 1]);
			A(i, i + 1) = 1.0 / 3.0 * (x[i + 1] - x[i]);

			rhs[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
		}
		// Conditions de limite  à gauche du domaine d'étude
		if(_left == spline::second_deriv) {
			// b[0] = f" / 2
			A(0, 0) = 2.0;
			A(0, 1) = 0.0;
			rhs[0] = _left_value;
		} else if(_left == spline::first_deriv) {
			// c[0] = f' ; on cherche une expression en fonction de b :
			// (2b[0] + b[1]).(x[1] - x[0]) = 3 (y[1]-y[0])/(x[1]-x[0]) - f')
			A(0, 0) = 2.0 * (x[1] - x[0]);
			A(0, 1) = 1.0 * (x[1] - x[0]);
			rhs[0] = 3.0 * ((y[1] - y[0]) / (x[1] - x[0]) - _left_value);
		} else {
			assert(false);
		}

		// Conditions de limite à droite du domaine d'étude
		if(_right == spline::second_deriv) {
			// b[n-1] = f" / 2
			A(n - 1, n - 1) = 2.0;
			A(n - 1, n - 2) = 0.0;
			rhs[n - 1] = _right_value;
		} else if(_right == spline::first_deriv) {
			// c[n-1] = f' ; on cherche une expression en fonction de b :
			// (b[n-2] + 2b[n-1]).(x[n-1] - x[n-2])   =   3 (f' - (y[n-1] - y[n-2])/(x[n-1] - x[n-2]))
			A(n - 1, n - 1) = 2.0 * (x[n - 1] - x[n - 2]);
			A(n - 1, n - 2) = 1.0 * (x[n - 1] - x[n - 2]);
			rhs[n - 1] = 3.0 * (_right_value - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]));
		} else {
			assert(false);
		}

		// On résoud le système d'équation pour obtenir les paramètres b[]
		_b = A.lu_solve(rhs);

		// On calcule les paramètres a[] et c[] en fonction de b[]
		_a.resize(static_cast<unsigned long>(n));
		_c.resize(static_cast<unsigned long>(n));
		for(int i = 0; i < n - 1; i++) {
			_a[i] = 1.0 / 3.0 * (_b[i + 1] - _b[i]) / (x[i + 1] - x[i]);
			_c[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - 1.0 / 3.0 * (2.0 * _b[i] + _b[i + 1]) * (x[i + 1] - x[i]);
		}
	} else { // On réalise une interpolation linéaire
		_a.resize(static_cast<unsigned long>(n));
		_b.resize(static_cast<unsigned long>(n));
		_c.resize(static_cast<unsigned long>(n));
		for(int i = 0; i < n - 1; i++) {
			_a[i] = 0.0;
			_b[i] = 0.0;
			_c[i] = (_y[i + 1] - _y[i]) / (_x[i + 1] - _x[i]);
		}
	}

	// Coefficients d'extrapolation à gauche
	_b0 = (!_force_linear_extrapolation) ? _b[0] : 0.0;
	_c0 = _c[0];

	// Coefficients d'extrapolation à droite
	// f_{n-1}(x) = b*(x - x_{n-1})^2 + c*(x - x_{n-1}) + y_{n-1}
	double h = x[n - 1] - x[n - 2];
	// _b[n-1] est déterminé par les conditions des limites aux bords
	_a[n - 1] = 0.0;
	_c[n - 1] = 3.0 * _a[n - 2] * h * h + 2.0 * _b[n - 2] * h + _c[n - 2]; // = f'_{n-2}(x_{n-1})
	if(_force_linear_extrapolation)
		_b[n - 1] = 0.0;
}

double spline::operator()(double x) const {
	size_t n = _x.size();
	// On cherche le point _x[idx] < x le plus proche ; idx = 0 même si x < _x[0]
	std::vector<double>::const_iterator it;
	it = std::lower_bound(_x.begin(), _x.end(), x);
	int idx = std::max(int(it - _x.begin()) - 1, 0);

	double h = x - _x[idx];
	double interpol;
	if(x < _x[0]) {
		// Extrapolation à gauche
		interpol = (_b0 * h + _c0) * h + _y[0];
	} else if(x > _x[n - 1]) {
		// Extrapolation à droite
		interpol = (_b[n - 1] * h + _c[n - 1]) * h + _y[n - 1];
	} else {
		// Interpolation
		interpol = ((_a[idx] * h + _b[idx]) * h + _c[idx]) * h + _y[idx];
	}
	return interpol;
}

std::pair<double, double> spline::get_deriv(double x) const {
	size_t n = _x.size();
	// On cherche le point _x[idx] < x le plus proche ; idx = 0 même si x < _x[0]
	std::vector<double>::const_iterator it;
	it = std::lower_bound(_x.begin(), _x.end(), x);
	int idx = std::max(int(it - _x.begin()) - 1, 0);

	std::pair<double, double> derivs;
	if(x < _x[0]) {
		// Extrapolation à gauche
		derivs = std::make_pair(_c0, _b0);
	} else if(x > _x[n - 1]) {
		// Extrapolation à droite
		derivs = std::make_pair(_c[n - 1], _b[n - 1]);
	} else {
		// Interpolation
		derivs = std::make_pair(_c[idx], _b[idx]);
	}
	return derivs;
}

double spline::get_curvature(double x) const {
	std::pair<double, double> derivs = get_deriv(x);

	return derivs.second * pow(1 + derivs.first, -1.5);
}


/// Implémentation de parametric_spline
/// ------------------------------------

void parameter_spline::set_points(const std::vector<double>& x, const std::vector<double>& y, bool cubic_spline) {
	assert(x.size() == y.size());
	assert(x.size() > 2);

	// On initialise les vecteurs de discrétisation (ils varient entre 0 et f_max ; la suite est croissante)
	_create_parametric_vectors(x, y);

	// On initialise les splines d'interpolation par rapport aux points de discrétisation
	_splineX.set_points(_paramT, x, cubic_spline);
	_splineY.set_points(_paramT, y, cubic_spline);
}

void parameter_spline::set_points(const std::vector<Vector2m>& pos, bool cubic_spline) {
	std::vector<double> x(pos.size()), y(pos.size());

	for(int i = 0; i < pos.size(); i++) {
		x[i] = toSplineCalculation(pos[i].x);
		y[i] = toSplineCalculation(pos[i].y);
	}

	set_points(x, y, cubic_spline);
}

std::vector<std::pair<double, double>> parameter_spline::get_curvature_infos_d() const {
	std::vector<std::pair<double, double>> result;

	std::pair<double, double> Xderivs;
	std::pair<double, double> Yderivs;
	for(int i = 0; i < _paramT.size() - 1; i++) {
		// On récupère les dérivées 1ère et 2ème des splines X et Y au point T (calcul de la courbure avec)
		Xderivs = _splineX.get_deriv(_paramT[i]);
		Yderivs = _splineY.get_deriv(_paramT[i]);
		double curvature = (Xderivs.first * Yderivs.second - Yderivs.first * Xderivs.second) *
		                   pow(Xderivs.first * Xderivs.first + Yderivs.first * Yderivs.first, -1.5);

		// Distance linéaire entre les points qui correspondent à deux T consécutifs
		// TODO : vérifier que l'approximation est cohérente sur le robot (vraie formule avec une intégrale)
		double length = sqrt(pow(_splineY(_paramT[i + 1]) - _splineY(_paramT[i]), 2)) +
		                pow(_splineX(_paramT[i + 1]) - _splineX(_paramT[i]), 2);
		result.emplace_back(curvature, length);
	}
	return result;
}

std::vector<std::pair<Length, Distance>> parameter_spline::get_curvature_infos() const {
	std::vector<std::pair<double, double>> double_infos = get_curvature_infos_d();
	std::vector<std::pair<Length, Distance>> result;

	for(int i = 0; i < double_infos.size(); i++) {
		result.emplace_back(fromSplineCalculationCA(double_infos[i].first), fromSplineCalculationD(double_infos[i].second));
	}
	return result;
}

std::vector<CurvatureInfos> parameter_spline::get_curvature_advance(Distance enter_axis) const {
	// On calcule une nouvelle spline avec bcp plus de données réparties le long du trajet
	parameter_spline accurate_spline;
	auto checkpoints = this->get_points();
	accurate_spline.set_points(checkpoints);

	auto Xderivs = accurate_spline.get_derivs(true);
	auto Yderivs = accurate_spline.get_derivs(false);

	std::vector<CurvatureInfos> result;

	for(int i = 0; i < Xderivs.size(); i++) {
		double Xderiv1 = Xderivs[i].first;
		double Yderiv1 = Yderivs[i].first;
		Distance length = (checkpoints[i + 1] - checkpoints[i]).norm();

		double denom = Xderiv1 * Yderivs[i].second - Yderiv1 * Xderivs[i].second;
		if(std::abs(denom) > 1.0) {
			std::cout << "WARNING: faulty case with R=0_m." << std::endl;
		} // TODO : gérer le cas défectueux (R = 0_m)
		Distance R = fromSplineCalculationD(std::abs(pow(Xderiv1 * Xderiv1 + Yderiv1 * Yderiv1, 1.5) / denom));
		bool inversion = R < 0_m;
		R = abs(R);
		double speedCoeffL = (R - enter_axis / 2) / (R + enter_axis / 2);
		double speedCoeffR = (R + enter_axis / 2) / (R - enter_axis / 2);

		/*std::cout << "R vaut " << R << " (dénominateur = " << Xderiv1 * Yderivs[i].second - Yderiv1 *
		Xderivs[i].second << ')'
		          << std::endl;
		std::cout << "Le coeff droit  vaut " << speedCoeffL << std::endl;
		std::cout << "Le coeff gauche vaut " << speedCoeffR << std::endl;

		std::cout << "La distance à parcourir vaut " << length << '\n' << std::endl;*/

		if(inversion)
			result.push_back({speedCoeffR, speedCoeffL, length});
		else
			result.push_back({speedCoeffL, speedCoeffR, length});
	}
	return result;
}

std::vector<std::pair<double, double>> parameter_spline::get_points_d() const {
	std::vector<std::pair<double, double>> result;
	//  On récupère les points M = (x, y) associés à chacune des valeurs du paramètre de discrétisation T
	for(int i = 0; i < _paramTT.size(); i++) {
		result.emplace_back(_splineX(_paramTT[i]), _splineY(_paramTT[i]));
	}
	return result;
}

std::vector<Vector2m> parameter_spline::get_points() const {
	std::vector<Vector2m> pos;
	for(int i = 0; i < _paramTT.size() - 1; i++) {
		pos.push_back(fromSplineCalculation(_splineX(_paramTT[i]), _splineY(_paramTT[i])));
	}
	return pos;
}

std::vector<std::pair<double, double>> parameter_spline::get_derivs(bool X) const {
	std::vector<std::pair<double, double>> result;
	for(int i = 0; i < _paramT.size() - 1; i++)
		result.push_back((X ? _splineX : _splineY).get_deriv(_paramT[i]));
	return result;
}

void parameter_spline::_create_parametric_vectors(const std::vector<double>& x, const std::vector<double>& y) {
	_paramT.push_back(0.0);
	// Initialisation de la suite (Ti)
	// La distance entre les valeurs de T dépend de la distance entre les points d'interpolation
	for(int i = 1; i < x.size(); i++) {
		_paramT.push_back(_paramT.back() + sqrt((x[i] - x[i - 1]) * (x[i] - x[i - 1]) + (y[i] - y[i - 1]) * (y[i] - y[i - 1])));
	}

	// Initialisation de la suite (TTi) plus précise
	// Il y a POINTS_MULTIPLIER équidistants entre chacune des valeurs de (Ti)
	double dist, dt;
	for(int i = 0; i < x.size() - 1; i++) {
		dist = _paramT[i + 1] - _paramT[i];
		dt = dist / POINTS_MULTIPLIER;
		for(int j = 0; j < POINTS_MULTIPLIER; j++) {
			_paramTT.push_back(_paramT[i] + j * dt);
		}
	}
	_paramTT.push_back(_paramT[x.size()]);

	// debug();
}

void parameter_spline::debug() {
	std::cout << "Composition du vecteur T :\n";
	for(int i = 0; i < _paramT.size(); i++) {
		std::cout << _paramT[i] << '\n';
	}
	std::cout << "\n\nComposition du vecteur TT :\n";
	for(int i = 0; i < _paramTT.size(); i++) {
		std::cout << _paramTT[i] << '\n';
	}
}
