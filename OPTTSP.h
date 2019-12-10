// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

/*
 * For part C of the project. The OPTTSP class uses both part A and part B to compute the TSP
 * solution using the B&B techniques.
 *
 */

#ifndef OPTTSP_H
#define OPTTSP_H

#include "MST.h"
#include "FASTTSP.h"

class OPTTSP : public MST, public FASTTSP {
public:
	OPTTSP(std::istream &is, bool border);

	void compute_tour();

private:
	std::vector<std::vector<double> > metric; // use metric graph for optimized TSP
	std::vector<std::vector<Vertex>::iterator> opt_tour;
	double tsp_upperbound;

	void gen_perms(size_t perm_length);
	bool promising(size_t perm_length);
	std::vector<Vertex>::iterator closest_mst_vertex(std::vector<Vertex>::iterator &current, size_t perm_length);
};

#endif
