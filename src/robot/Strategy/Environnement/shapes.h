#pragma once
#include <iostream>

/**
 * Représente les différents types de formes.
 */
enum ShapeType {
	Shape_Circle,
	Shape_Rectangle,
};

/*
 * Classe abstraite dont dérivent toutes les autres formes.
 */
class Shape {
public:
	/**
	 * Coût appliqué aux noeuds de la grille contenus dans cette forme.
	 */
	float value;

	Shape() = default;

	Shape(float value_) : value(value_) {}

	virtual ~Shape() = default;

	/*
	 * Méthode à substituer qui indique de quel type de forme il s'agit.
	 * Node : cette approche a été préférée au polymorphisme car l'algorithme
	 * de dessin des formes a besoin de beaucoup de données présentes dans Astar,
	 * et afin d'éviter des includes circulaires.
	 */
	virtual ShapeType getType() = 0;
};

/**
 * Représente une forme de cercle.
 */
class Circle : public Shape {
public:
	/**
	 * Rayon intérieur du cercle.
	 */
	Distance radius;
	/*
	 * Position du centre du cercle (mètres).
	 */
	Vector2m position;

	Circle() = default;
	Circle(float value_, Distance radius_, Vector2m position_) : Shape(value_), radius(radius_), position(position_) {}


	ShapeType getType() {
		return ShapeType::Shape_Circle;
	}
};
/**
 * Représente une forme de rectangle.
 */
class Rect : public Shape {
public:
	/*
	 * Position bord supérieur gauche du rectangle (mètres).
	 */
	Vector2m position;
	/*
	 * Taille du rectangle (mètres).
	 */
	Vector2m size;
	/*
	 * Rotation du rectangle dans le sens trigo autour du bord supérieur gauche
	 */
	Angle angle;

	Rect() = default;

	Rect(float value_, Vector2m position_, Vector2m size_, Angle angle_ = 0_deg)
	        : Shape(value_), position(position_), size(size_), angle(angle_) {}

	ShapeType getType() {
		return ShapeType::Shape_Rectangle;
	}
};

struct Point {
	float x;
	float y;
	Point(float x_, float y_) : x(x_), y(y_) {}

	friend std::ostream& operator<<(std::ostream& o, Point const& node);
};
