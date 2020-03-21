#include "node.h"
#include <unordered_map>

namespace RobotEnv {
	Node::Node(int x_, int y_, float v_, float effectiveCost_, float heuristicCost_) {
		x = x_;
		y = y_;
		staticValue = v_;
		effectiveCost = effectiveCost_;
		heuristicCost = heuristicCost_;
		dynamicValue = 0;
	}

	float Node::totalCost() const {
		return heuristicCost;
	}

	bool operator==(Node const& node1, Node const& node2) {
		return node1.x == node2.x && node1.y == node2.y;
	}

	bool operator<(Node const& node1, Node const& node2) {
		return node1.totalCost() < node2.totalCost();
	}

} // namespace RobotEnv