// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "FASTTSP.h"
#include <cassert>
#include <algorithm>
#include <numeric>
using namespace std;

FASTTSP::FASTTSP(istream &is, bool border) : SimpleGraph(is, border) { }

void FASTTSP::arbitrary_insertion(const vector<vector<double> > &metric) {
	tour.clear();
	assert(!vertices.empty());
	const size_t n = vertices.size();
	tour.reserve(n + 1);

	if (vertices.size() <= 2) {
		for (auto it = vertices.begin(); it != vertices.end(); ++it) tour.push_back(it);
		return;
	}

	reset_vertices();

	tour.push_back(vertices.begin());
	tour.push_back(vertices.begin() + 1);
	tour.push_back(vertices.begin() + 2);
	tour.push_back(vertices.begin());
	vertices[0].deleted = true;
	vertices[1].deleted = true;
	vertices[2].deleted = true;

	while (tour.size() < n + 1) {
		auto next_vertex = find_if(vertices.begin(), vertices.end(), is_valid_vertex);

		auto min_pos = tour.end();
		double min_tri_dist = numeric_limits<double>::infinity();
		for (size_t i = 0; i < tour.size() - 1; ++i) {
			double candidate_dist;
			if (metric.empty()) {
				candidate_dist = dist(*next_vertex, *tour[i]) + dist(*next_vertex, *tour[i + 1]) - dist(*tour[i], *tour[i + 1]);
			} else {
				const size_t next_vert_ind = unsigned(next_vertex - vertices.begin());
				const size_t tour_i_ind = unsigned(tour[i] - vertices.begin());
				const size_t tour_i_plus_ind = unsigned(tour[i + 1] - vertices.begin());
				candidate_dist = metric[next_vert_ind][tour_i_ind] +
					metric[next_vert_ind][tour_i_plus_ind] -
					metric[tour_i_ind][tour_i_plus_ind];
			}
			if (candidate_dist < min_tri_dist) {
				min_pos = tour.begin() + unsigned(i);
				min_tri_dist = candidate_dist;
			}
		}
		tour.insert(min_pos + 1, next_vertex);
		next_vertex->deleted = true;
	}
	tour.pop_back();
}

vector<SimpleGraph::Vertex>::iterator FASTTSP::closest_valid_vertex
	(vector<SimpleGraph::Vertex>::iterator &current) {

	vector<SimpleGraph::Vertex>::iterator min_vertex = find_if(vertices.begin(), vertices.end(), is_valid_vertex);
	double min_dist = dist(*min_vertex, *current);

	for (auto it = min_vertex + 1; it != vertices.end(); ++it) {
		if (!it->deleted && it != current) {
			// Set to min_vertex if it's closer.
			const double candidate_dist = dist(*it, *current);
			if (candidate_dist < min_dist) {
				min_vertex = it;
				min_dist = candidate_dist;
			}
		}
	}
	return min_vertex;
}

void FASTTSP::print_tour(ostream &os) const {
	const double distance = tour_distance(tour.size());

	os << distance << endl;
	string s;
	for (const auto &it : tour) {
		s += to_string(it - vertices.begin()) + ' ';
		if (s.size() > STRING_PRINT_LIMIT) {
			os << s;
			s.clear();
		}
	}
	os << s;
}

double FASTTSP::tour_distance(size_t num_elements, const vector<vector<double> > &metric) const {
	double distance = 0;
	if (metric.empty()) {
		for (size_t i = 0; i < num_elements - 1; ++i){
			distance += dist(*tour[i], *tour[i + 1]);
		}
		distance += dist(*tour[0], *tour[num_elements - 1]);
	} else {
		for (size_t i = 0; i < num_elements - 1; ++i) {
			distance += metric[unsigned(tour[i] - vertices.begin())][unsigned(tour[i + 1] - vertices.begin())];
		}
		distance += metric[(unsigned)(tour[0] - vertices.begin())][unsigned(tour[num_elements - 1] - vertices.begin())];
	}
	return distance;
}

bool FASTTSP::next_combination(unsigned &i, unsigned &j) const {
	const size_t n = tour.size();
	if (j < n - 1) {
		++j;
		return true;
	}
	if (i < n - 2) {
		++i;
		j = i + 1;
		return true;
	}
	// Else, we've reached the end.
	return false;
}
