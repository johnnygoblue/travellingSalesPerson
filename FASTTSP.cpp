// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "FASTTSP.h"
#include <algorithm>
#include <numeric>
using namespace std;

FASTTSP::FASTTSP(istream &is, bool border) : SimpleGraph(is, border) { }

bool FASTTSP::initialize_insertion() {
	const size_t num_vertices = vertices.size();

	tour.clear();
	tour.reserve(num_vertices + 1);

	if (num_vertices <= 2) {
		auto it = vertices.begin();
		while(it != vertices.end()) {
			tour.push_back(it);
			++it;
		}
		return false;
	}

	reset_vertices();

	/* start with 0, 1, and 2 before we go arbitrary */
	tour.push_back(vertices.begin());
	tour.push_back(vertices.begin() + 1);
	tour.push_back(vertices.begin() + 2);
	tour.push_back(vertices.begin());
	vertices[0].deleted = true;
	vertices[1].deleted = true;
	vertices[2].deleted = true;
	return true;
}

void FASTTSP::arbitrary_insertion(const vector<vector<double> > &metric) {
	if (!initialize_insertion()) {
		return;
	}
	while (tour.size() < vertices.size() + 1) {
		auto next_vertex = find_if(vertices.begin(), vertices.end(), is_valid_vertex);

		auto min_pos = tour.end();
		double min_tri_dist = numeric_limits<double>::infinity();
		size_t i = 0;
		while (i < tour.size() - 1) {
			double candidate_dist;
			if (!metric.empty()) {
				const size_t next_vert_ind = unsigned(next_vertex - vertices.begin());
				const size_t tour_i_ind = unsigned(tour[i] - vertices.begin());
				const size_t tour_i_plus_ind = unsigned(tour[i + 1] - vertices.begin());
				candidate_dist = metric[next_vert_ind][tour_i_ind] + metric[next_vert_ind][tour_i_plus_ind]
					- metric[tour_i_ind][tour_i_plus_ind];
			}
			else {
				candidate_dist = dist(*next_vertex, *tour[i]) + dist(*next_vertex, *tour[i + 1]) - dist(*tour[i], *tour[i + 1]);
			}
			if (candidate_dist < min_tri_dist) {
				min_pos = tour.begin() + unsigned(i);
				min_tri_dist = candidate_dist;
			}
			++i;
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
	auto it = min_vertex + 1;
	while (it != vertices.end()) {
		if (it != current && !it->deleted) {
			const double candidate_dist = dist(*it, *current);
			if (min_dist > candidate_dist) {
				min_vertex = it;
				min_dist = candidate_dist;
			}
		}
		++it;
	}
	return min_vertex;
}

void FASTTSP::print_tour(ostream &os) const {
	const string space = " ";
	string s;
	const double distance = tour_distance(tour.size());
	os << distance << endl;
	for (const auto &it : tour) {
		s += to_string(it - vertices.begin()) + space;
		if (s.size() > STRING_BUF_LIMIT) {
			os << s;
			s.clear();
		}
	}
	os << s;
}

double FASTTSP::tour_distance(size_t num_elements, const vector<vector<double> > &metric) const {
	double distance = 0;
	size_t i = 0;
	if (!metric.empty()) {
		while (i < num_elements - 1) {
			distance += metric[unsigned(tour[i] - vertices.begin())][unsigned(tour[i + 1] - vertices.begin())];
			++i;
		}
		distance += metric[(unsigned)(tour[0] - vertices.begin())][unsigned(tour[num_elements - 1] - vertices.begin())];
	} else {
		while (i < num_elements - 1) {
			distance += dist(*tour[i], *tour[i + 1]);
			++i;
		}
		distance += dist(*tour[0], *tour[num_elements - 1]);
	}
	return distance;
}
