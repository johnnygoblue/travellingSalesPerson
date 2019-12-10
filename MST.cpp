// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "MST.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
using namespace std;

MST::MST(istream &is, bool border_in) : SimpleGraph(is, border_in) { }

void MST::gen_mst(const vector<vector<double> > &metric) {
	mst.clear();
	const size_t graph_size = working_size();
	mst.reserve(graph_size - 1);

	if (border) {
		/* check if we can generate a MST given the subgraph */
		bool WATER_present = false, LAND_present = false, COAST_present = false;
		for (const auto &it : vertices) {
			if (COAST_present) {
				break;
			}
			SimpleGraph::VertType type = it.get_vert_type();
			if (type == SimpleGraph::VertType::LAND) {
				LAND_present = true;
			} else if (type == SimpleGraph::VertType::WATER) {
				WATER_present = true;
			} else if (type == SimpleGraph::VertType::COAST) {
				COAST_present = true;
			} else {
				cerr << "Invalid type '" << int(type) << "'" << endl;
			}
		}
		if (LAND_present && WATER_present && !COAST_present) {
			cerr << "Unable to construct MST, need coast line!" << endl;
			exit(1);
		}
	}

	vector<vector<Vertex>::iterator> innies;
	innies.reserve(graph_size);

	innies.push_back(find_if(vertices.begin(), vertices.end(), is_valid_vertex));
	innies.back()->deleted = true;

	while (mst.size() < graph_size - 1) {
		for (auto it = innies.front(); it != vertices.end(); ++it) {
			if (is_valid_vertex(*it)) {
				const double candidate_dist = metric.empty() ? dist(*innies.back(), *it) :
					metric[(unsigned long)(innies.back() - vertices.begin())][(unsigned long)(it - vertices.begin())];
				if (candidate_dist < it->running_dist) {
					it->running_dist = candidate_dist;
					it->parent_index = (unsigned)(innies.back() - vertices.begin());
				}
			}
		}

		auto closest = min_element(vertices.begin(), vertices.end());
		closest->deleted = true;
		closest->running_dist = numeric_limits<double>::infinity();
		const size_t parent_of_closest = closest->parent_index;
		innies.push_back(closest);

		const size_t closest_index = (unsigned long)(closest - vertices.begin());
		if (parent_of_closest < closest_index) {
			mst.push_back({vertices.begin() + unsigned(parent_of_closest), closest});
		} else {
			mst.push_back({closest, vertices.begin() + (unsigned)parent_of_closest});
		}
	}
}

void MST::print_mst(std::ostream &os, const std::vector<std::vector<double> > &metric) const {
	string s;
	const double weight = mst_weight(metric);
	const string space = " ";

	os << weight << "\n";
	for (const auto &p : mst) {
		s += to_string(p.first - vertices.begin()) + space + to_string(p.second - vertices.begin()) + "\n";
		if (s.length() > STRING_BUF_LIMIT) {
			os << s;
			s.clear();
		}
	}
	os << s;
}

double MST::mst_weight(const vector<std::vector<double> > &metric) const {
	double weight = 0;
	if (metric.empty()) {
		for (const auto &p : mst)
			weight += dist(*p.first, *p.second);
	} else {
		for (const auto &p : mst)
			weight += metric[(unsigned long)(p.first - vertices.begin())][(unsigned long)(p.second - vertices.begin())];
	}
	return weight;
}
