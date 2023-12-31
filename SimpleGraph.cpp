// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <algorithm>
#include <limits>
#include <cmath>
#include <list>

#include "SimpleGraph.h"

using namespace std;

SimpleGraph::VertType SimpleGraph::Vertex::get_vert_type() const {
	if (x < 0 && y < 0) return SimpleGraph::VertType::WATER;
	if (x > 0 || y > 0) return SimpleGraph::VertType::LAND;
	return SimpleGraph::VertType::COAST;
}

bool SimpleGraph::Vertex::operator< (const SimpleGraph::Vertex &other) const { return running_dist < other.running_dist; }

SimpleGraph::SimpleGraph(istream &is, bool border_in) : STRING_BUF_LIMIT(4096), border(border_in) {
	size_t size_in;
	int x_in, y_in;

	is >> size_in;
	vertices.reserve(size_in);

	while (is >> x_in >> y_in) {
		vertices.push_back( { numeric_limits<double>::infinity(), 0, x_in, y_in, false,} );
	}
}

bool SimpleGraph::is_valid_vertex(const Vertex &v) { return !v.deleted; }

size_t SimpleGraph::working_size() const {
	return (unsigned long)count_if(vertices.begin(), vertices.end(), is_valid_vertex);
}

double SimpleGraph::dist(const Vertex &v_1, const Vertex &v_2) const {

	if (&v_1 == &v_2) return 0;
	/*
		LAND + LAND = 0
		LAND + WATER = 1
		LAND + COAST = 2
		WATER + WATER = 2
		WATER + COAST = 3
		COAST + COAST = 4
	If we get a sume of 1, it's infinity. Otherwise, use Euclidean distance.
	*/
	if (border && ((int)v_1.get_vert_type() + (int)v_2.get_vert_type() == 1))
		return numeric_limits<double>::infinity();
	const double delta_x = v_1.x - v_2.x;
	const double delta_y = v_1.y - v_2.y;
	const double dist = sqrt(delta_x * delta_x + delta_y * delta_y);
	return dist;
}

void SimpleGraph::reset_vertices() {
	for (auto &v : vertices) {
		v.deleted = false;
	}
}
