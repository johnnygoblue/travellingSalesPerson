// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "MST.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
using namespace std;

MST::MST(istream &is, bool border_in) : SimpleGraph(is, border_in) { }

void MST::check_border() {
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
		if (!COAST_present && LAND_present && WATER_present) {
			cerr << "Unable to construct MST, need coast line!" << endl;
			exit(1);
		}
	}
	return;
}

void MST::gen_mst(const vector<vector<double> > &metric) {
	mst.clear();
	const size_t graph_size = working_size();
	mst.reserve(graph_size - 1);

	check_border();
	vector<vector<Vertex>::iterator> innies;
	innies.reserve(graph_size);

	innies.push_back(find_if(vertices.begin(), vertices.end(), is_valid_vertex));
	innies.back()->deleted = true;

	while (mst.size() < graph_size - 1) {
		auto it = innies.front();
		double candidate_dist = 0.0;
		while (it != vertices.end()) {
			if (is_valid_vertex(*it)) {
				if (!metric.empty()) {
					candidate_dist = metric[(unsigned long)(innies.back() - vertices.begin())][(unsigned long)(it - vertices.begin())];
				} else {
					candidate_dist = dist(*innies.back(), *it);
				}
				if (candidate_dist < it->running_dist) {
					it->running_dist = candidate_dist;
					it->parent_index = (unsigned)(innies.back() - vertices.begin());
				}
			}
			++it;
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
	auto p = mst.begin();
	while (p != mst.end()) {
		s += to_string(p->first - vertices.begin()) + space + to_string(p->second - vertices.begin()) + "\n";
		if (s.length() > STRING_BUF_LIMIT) {
			os << s;
			s.clear();
		}
		++p;
	}
	os << s;
}

double MST::mst_weight(const vector<std::vector<double> > &metric) const {
	double weight = 0;
	auto p = mst.begin();
	if (!metric.empty()) {
		while (p != mst.end()) {
			weight += metric[(unsigned long)(p->first - vertices.begin())][(unsigned long)(p->second - vertices.begin())];
			++p;
		}
	} else {
		while (p != mst.end()) {
			weight += dist(*p->first, *p->second);
			++p;
		}
	}
	return weight;
}
