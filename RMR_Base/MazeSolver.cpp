#include "MazeSolver.h"

#include <set>
#include <queue>

void maze::MazeSolver::loadMaze(const lidar::Map& data) {

	const Point first = (*(data.getMap().begin())).first,
		last = (*(data.getMap().rbegin())).first;

	const int spacing = data.spacing / 2;
	nodes.clear();
	//create nodes
	for (int y = data.minVal; y < data.maxVal; y += spacing) {
		nodes.emplace_back();
		auto& row = *(nodes.rbegin());
		for (int x = data.minVal; x < data.maxVal; x += spacing) {
			auto n = std::make_shared<Node>();
			n->p = Point(x, y);
			if (data.checkPoint(n->p, 10)) {
				n->blocked = true;
			}
			row.push_back(std::move(n));
		}
	}

}

bool maze::MazeSolver::checkNeighbors(node_cont::size_type row, node_cont::size_type col) const {
	/*Check pattern:
		1 2 3
		4 5 6
		7 8 9
	*/
	const auto& row1 = nodes[row - 1], & row2 = nodes[row], & row3 = nodes[row + 1];

	return row1[col - 1]->blocked || row1[col]->blocked || row1[col + 1]->blocked
		|| row2[col - 1]->blocked || row2[col]->blocked || row2[col + 1]->blocked
		|| row3[col - 1]->blocked || row3[col]->blocked || row3[col + 1]->blocked;

}

void maze::MazeSolver::dilate() {
	const auto sz = nodes.size();
	using pair = std::pair<int, int>;

	std::vector<pair> blockeds;

	for (int i = 1; i < sz - 1; ++i) {
		for (int j = 1; j < sz - 1; ++j) {
			if (nodes[i][j]->blocked) {
				blockeds.emplace_back(i, j);
			}
		}
	}

	for (const auto& p : blockeds) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				nodes[p.first + i][p.second + j]->blocked = true;
			}
		}
	}
}

void maze::MazeSolver::dilate(int runs) {
	while (runs > 0) {
		dilate();
		--runs;
	}
}

void maze::MazeSolver::addNeighbors(int r, int c) {
	if (nodes[r][c]->blocked) return;
	for (int i = r - 1; i <= r + 1; ++i) {
		for (int j = c - 1; j <= c + 1; ++j) {
			if (i == r && j == c) continue;	//itself
			if (!nodes[i][j]->blocked) {
				nodes[r][c]->neighbors.push_back(nodes[i][j]);
			}
		}
	}
}

void maze::MazeSolver::connectNodes() {

	const auto sz = nodes.size();
	for (int i = 1; i < sz - 1; ++i) {
		for (int j = 1; j < sz - 1; ++j) {
			addNeighbors(i, j);
		}
	}
}

maze::MazeSolver::sol_t& maze::MazeSolver::getSolution()
{
	return sol;
}

bool maze::MazeSolver::astar() {

	dilate(10);

	auto cmp = [](std::weak_ptr<Node> l, std::weak_ptr<Node> r) {
		return l.lock()->hDist < r.lock()->hDist;
	};
	
	return false;

}


maze::MazeSolver::sol_t& maze::MazeSolver::dijkstra() {
	dilate(10);
	connectNodes();

	auto cmp = [](std::weak_ptr<Node> l, std::weak_ptr<Node> r) {
		return *(l.lock()) < *(r.lock());
	};

	auto dist_cmp = [](std::weak_ptr<Node> l, std::weak_ptr<Node> r) {
		auto ll = l.lock();
		auto rr = r.lock();
		if (ll->distFromStart != rr->distFromStart) 
			return ll->distFromStart < rr->distFromStart;
		return *ll < *rr;
	};

	std::set<std::weak_ptr<Node>, decltype(dist_cmp)> dist(dist_cmp);

	for (auto& row : nodes) {
		for (std::weak_ptr<Node> n : row) {
			auto val = n.lock();
			val->distFromStart = std::numeric_limits<double>::max();
			val->parent.reset();
			if(!val->blocked)
				dist.insert(val);
		}
	}

	dist.erase(source);
	source.lock()->distFromStart = 0;
	source.lock()->parent = source;
	dist.insert(source);

	while (!dist.empty()) {
		auto u = *(dist.begin());
		dist.erase(u);
		if (*(u.lock()) == *(target.lock()))
			break;

		auto u_ptr = u.lock();

		for (auto neighbor : u_ptr->neighbors) {
			auto n_ptr = neighbor.lock();
			double altDist = u_ptr->distFromStart + n_ptr->distFromNeighbor(*u_ptr);

			if (altDist < n_ptr->distFromStart && !n_ptr->blocked) {
				dist.erase(neighbor);
				n_ptr->distFromStart = altDist;
				n_ptr->parent = u;
				dist.insert(neighbor);
			}
		}
	}


	auto pred = [](std::weak_ptr<Node> a, std::weak_ptr<Node> b) {
		return (*(a.lock()) == *(b.lock()));
	};

	std::weak_ptr<Node> curr = target;
	sol_t ret;
	while (!pred(source, curr))
	{
		auto c = curr.lock();
		ret.push_back(c->p);
		curr = c->parent;
	}
	ret.push_back(source.lock()->p);

	std::reverse(ret.begin(), ret.end());
	sol = ret;
	simplifySol();
	return sol;
}

const maze::MazeSolver::node_cont& maze::MazeSolver::getNodes() const {
	return nodes;
}

maze::MazeSolver::sol_t& maze::MazeSolver::simplifySol() {
	if (sol.size() < 2) {
		return sol;
	}

	//remove every 2nd element
	for (int i = 1; i < sol.size(); ++i) {
		sol.erase(sol.begin() + i);
	}

	decltype(sol) sol2;

	sol2.push_back(sol[0]);
	sol2.push_back(sol[1]);

	double dirNow = sol[0].dirTo(sol[1]);
	
	for (int i = 2; i < sol.size(); ++i) {
		double dir = sol[i - 1].dirTo(sol[i]);
		if (abs(dir - dirNow) > deg2rad(5)) {
			dirNow = dir;
			sol2.push_back(sol[i]);
		}
	}

	sol = sol2;
	sol2.clear();
	for (int i = 1; i < sol.size(); i += 2) {
		sol2.push_back(sol[i]);
	}
	sol2.push_back(target.lock()->p);
	sol = sol2;
	return sol;
}


std::weak_ptr<maze::Node> maze::MazeSolver::getClosest(const Point& p) const {
	double x = p.x, y = p.y;

	int row = -1;
	for (int i = 1; i < nodes.size(); ++i) {
		if (nodes[i - 1][0]->p.y <= p.y && nodes[i][0]->p.y > p.y) {
			row = i - 1;
			break;
		}
	}
	if (row == -1) {
		throw std::runtime_error("row not found");
	}
	int col = -1;
	for (int i = 1; i < nodes.size(); ++i) {
		if (nodes[row][i-1]->p.x <= p.x && nodes[row][i]->p.x > p.x) {
			col = i - 1;
			break;
		}
	}
	if (col == -1) {
		throw std::runtime_error("col not found");
	}

	return nodes[row][col];
}

void maze::MazeSolver::setSource(const Point& p) {

	source = getClosest(p);
}

void maze::MazeSolver::setTarger(const Point& p) {
	target = getClosest(p);
}
