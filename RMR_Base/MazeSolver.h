#pragma once

#include "LidarData.h"
#include "Helpers.h"
#include <limits>
#include <memory>

namespace maze {
	
	struct Node {
		Point p{ 0.0, 0.0 };
		bool blocked = false;
		double distFromStart = std::numeric_limits<double>::max();
		double hDist = std::numeric_limits<double>::max();
		std::weak_ptr<Node> parent;
		std::vector<std::weak_ptr<Node>> neighbors;

		bool operator<(const Node& n) const {
			return this->p < n.p;
		}

		double distFromNeighbor(const Node& neighbor) {
			return sqrt(pow(p.x - neighbor.p.x,2) + pow(p.y - neighbor.p.y,2));
		}

		bool operator==(const Node& rhs) const {
			return this->p == rhs.p;
		}
	};

	class MazeSolver {
	public:
		using sol_t = std::vector<Point>;
		using node_cont = std::vector<std::vector<std::shared_ptr<Node>>>;
		node_cont nodes;
		std::weak_ptr<Node> source;
		std::weak_ptr<Node> target;
		sol_t sol;
	private:


	public:
		void loadMaze(const lidar::Map&);
		bool checkNeighbors(node_cont::size_type row, node_cont::size_type col) const;
		void dilate();
		void dilate(int runs);
		void addNeighbors(int r, int c);
		void connectNodes();
		void setStart(const Point&);
		void setFinish(const Point&);
		sol_t& getSolution();
		bool astar();
		sol_t& dijkstra();
		const node_cont& getNodes() const;

		std::weak_ptr<maze::Node> getClosest(const Point& p) const;

		void setSource(const Point& p);
		void setTarger(const Point& p);
	};

}