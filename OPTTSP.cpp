// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "OPTTSP.h"

#include <cassert>
#include <algorithm>
using namespace std;

OPTTSP::OPTTSP(istream &is, bool border) : SimpleGraph(is, border), MST(is, border),
	FASTTSP(is, border), tsp_upperbound(numeric_limits<double>::infinity()) {

	const size_t n = vertices.size();
	metric.resize(n);
	for (size_t i = 0; i < n; ++i) metric[i].resize(n);

	for (size_t i = 0; i < n; ++i) {
		metric[i][i] = 0;
		for (size_t j = i + 1; j < n; ++j) {
			metric[i][j] = dist(vertices[i], vertices[j]);
			metric[j][i] = metric[i][j];
		}
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

	for (size_t i = perm_length; i < tour.size(); ++i) tour[i]->deleted = false;

	gen_mst(metric);
	assert(mst.size() == tour.size() - perm_length - 1);

	const double subgraph_mst_dist = mst_weight(metric);

	const auto closest_to_front = closest_mst_vertex(tour.front(), perm_length);
	const auto closest_to_back = closest_mst_vertex(tour[perm_length - 1], perm_length);

	const double dist_from_front = metric[unsigned(closest_to_front - vertices.begin())][unsigned(tour.front() - vertices.begin())];
	const double dist_from_back = metric[unsigned(closest_to_back - vertices.begin())][unsigned(tour[perm_length - 1] - vertices.begin())];

	const double current_tsp_lowerbound = current_open_tour_dist + subgraph_mst_dist + dist_from_front + dist_from_back;

	if (current_tsp_lowerbound >= tsp_upperbound) return false;
	else return true;
}

vector<SimpleGraph::Vertex>::iterator OPTTSP::closest_mst_vertex
	(vector<Vertex>::iterator &current, size_t perm_length) {

	const size_t n = tour.size();
	assert(1 <= perm_length);
	assert(perm_length < n);

	vector<Vertex>::iterator closest_vertex = tour[perm_length];
	double min_dist = metric[unsigned(tour[perm_length] - vertices.begin())][unsigned(current - vertices.begin())];

	for (size_t i = perm_length + 1; i < n; ++i) {
		double possible_dist = metric[unsigned(tour[i] - vertices.begin())][unsigned(current - vertices.begin())];
		if (possible_dist < min_dist) {
			min_dist = possible_dist;
			closest_vertex = tour[i];
		}
	}

	return closest_vertex;
}
