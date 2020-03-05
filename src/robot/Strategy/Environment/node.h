#pragma once
#include <vector>

/**
 * Représente un noeud de la grille de l'A*.
 * Un noeud contient des coûts utilisés pour le calcul
 * de la trajectoire (StaticValue, DynamicValue).
 */
namespace RobotEnv {
	struct Node {
		Node() = default;
		Node(Node const&) = default;
		/**
		 * Crée une nouvelle instance de node en l'initialisant avec tous ses paramètres.
		 */
		Node(int x, int y, float v, float effectiveCost, float heuristicCost);

		/**
		 * Obtient le coût total du noeud.
		 */
		float totalCost() const;

		friend bool operator==(Node const& node1, Node const& node2);
		friend bool operator<(Node const& node1, Node const& node2);

		float _debugValue;
		/**
		 * Représente le coût statique de ce noeud, c'est à dire, un coût qui ne
		 * présentera pas de variation au cours des différentes exécutions de l'algorithme.
		 * Le coût statique est calculé comme la somme des coûts des obstacles statiques
		 * se trouvant sur ce noeud.
		 */
		float staticValue;
		/**
		 * Représente le coût dynamique de ce noeud.
		 * Le coût dynamique est susceptible de varier à chaque exécution de l'algorithme.
		 * Le coût dynamique est calculé comme la somme des coûts des obstacles dynamiques
		 * se trouvant sur ce noeud.
		 */
		float dynamicValue;
		/**
		 * Coût effectif du noeud lors du calcul de la trajectoire.
		 * Cette valeur est remise à 0 avant chaque calcul de l'A*.
		 */
		float effectiveCost;
		/**
		 * Coût heuristique du chemin entre ce noeud et le noeud de destination,
		 * utilisé lors du calcul de la trajectoire.
		 * Cette valeur est remise à 0 avant chaque calcul de l'A*.
		 */
		float heuristicCost;
		/**
		 * Position X du noeud sur la grille de l'A*.
		 */
		int x;
		/**
		 * Position Y du noeud sur la grille de l'A*.
		 */
		int y;
	};

} // namespace RobotEnv

/**
 * Représente une matrice de noeuds.
 */
using Nodemap = std::vector<std::vector<RobotEnv::Node>>;

namespace std {
	template <>
	struct hash<RobotEnv::Node> {
		size_t operator()(RobotEnv::Node const& node) const {
			return node.y * 100000 + node.y;
		}
	};
} // namespace std
