// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include <iostream>
#include <vector>

class SimpleGraph {
public:
	SimpleGraph(std::istream &is, bool border_in);

protected:
	enum class VertType {LAND, WATER, COAST};

	struct Vertex {
		double running_dist;
		unsigned parent_index;
		int x, y;
		bool deleted;
		VertType get_vert_type() const;
		bool operator< (const struct Vertex &other) const;
	}; // Vertex

	std::vector<Vertex> vertices;
	const size_t STRING_PRINT_LIMIT;
	const bool border;

	double dist(const Vertex &v_1, const Vertex &v_2) const;
	size_t working_size() const;
	void reset_vertices();
	static bool is_valid_vertex(const Vertex &v);
}; // SimpleGraph

#endif
