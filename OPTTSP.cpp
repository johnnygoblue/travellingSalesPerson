// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "OPTTSP.h"

#include <algorithm>
using namespace std;

OPTTSP::OPTTSP(istream &is, bool border) : SimpleGraph(is, border), MST(is, border),
	FASTTSP(is, border), tsp_upperbound(numeric_limits<double>::infinity()) {

	const size_t num_vertices = vertices.size();
	size_t i = 0;
	metric.resize(num_vertices);
	while (i < num_vertices) {
		metric[i].resize(num_vertices);
		++i;
	}
	i = 0;
	while (i < num_vertices) {
		size_t j = 0;
		metric[i][i] = 0;
		while (j < num_vertices) {
			metric[i][j] = dist(vertices[i], vertices[j]);
			metric[j][i] = metric[i][j];
			++j;
		}
		++i;
	}
}

void OPTTSP::compute_tour() {
	arbitrary_insertion(metric);
	tsp_upperbound = tour_distance(tour.size(), metric);

	opt_tour.resize(tour.size());
	copy(tour.begin(), tour.end(), opt_tour.begin());

	gen_perms(1);

	copy(opt_tour.begin(), opt_tour.end(), tour.begin());
}

void OPTTSP::gen_perms(size_t perm_length) {
	if (perm_length == tour.size()) {
		const double tour_length = tour_distance(tour.size(), metric);
		if (tour_length < tsp_upperbound) {
			copy(tour.begin(), tour.end(), opt_tour.begin());
			tsp_upperbound = tour_length;
		}
		return;
	}

  	if (!promising(perm_length)) return;

 	for (size_t i = perm_length; i < tour.size(); ++i) {
    	swap(tour[perm_length], tour[i]);
    	gen_perms(perm_length + 1);
    	swap(tour[perm_length], tour[i]);
  	}
}

bool OPTTSP::promising(size_t perm_length) {
	const double current_open_tour_dist = tour_distance(perm_length, metric)
		- metric[unsigned(tour[0] - vertices.begin())][unsigned(tour[perm_length - 1] - vertices.begin())];

	size_t i = perm_length;
	while (i < tour.size()) {
		tour[i]->deleted = false;
		++i;
	}

	gen_mst(metric);

	const double subgraph_mst_dist = mst_weight(metric);

	const auto closest_to_front = closest_mst_vertex(tour.front(), perm_length);
	const auto closest_to_back = closest_mst_vertex(tour[perm_length - 1], perm_length);

	const double dist_from_front = metric[unsigned(closest_to_front - vertices.begin())][unsigned(tour.front() - vertices.begin())];
	const double dist_from_back = metric[unsigned(closest_to_back - vertices.begin())][unsigned(tour[perm_length - 1] - vertices.begin())];

	const double current_tsp_lowerbound = current_open_tour_dist + subgraph_mst_dist + dist_from_front + dist_from_back;

	if (current_tsp_lowerbound < tsp_upperbound) {
		return true;
	}
	return false;
}

vector<SimpleGraph::Vertex>::iterator OPTTSP::closest_mst_vertex
	(vector<Vertex>::iterator &current, size_t perm_length) {
	vector<Vertex>::iterator closest_vertex = tour[perm_length];
	double min_dist = metric[unsigned(tour[perm_length] - vertices.begin())][unsigned(current - vertices.begin())];
	size_t i = perm_length + 1;

	while (i < tour.size()) {
		double possible_dist = metric[unsigned(tour[i] - vertices.begin())][unsigned(current - vertices.begin())];
		if (possible_dist < min_dist) {
			min_dist = possible_dist;
			closest_vertex = tour[i];
		}
		++i;
	}
	return closest_vertex;
}
