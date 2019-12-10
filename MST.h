// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

/*
 * For part A of the project. The MST class computes the MST given a subgraph
 *
 */

#ifndef MST_H
#define MST_H

#include "SimpleGraph.h"

class MST : virtual public SimpleGraph {
public:
	MST(std::istream &is, bool border_in);
	void gen_mst(const std::vector<std::vector<double> > &metric =
	std::vector<std::vector<double> > () );
	void print_mst(std::ostream &os, const std::vector<std::vector<double> > &metric =
		std::vector<std::vector<double> > () ) const;

protected:
	std::vector<std::pair<std::vector<Vertex>::iterator, std::vector<Vertex>::iterator> > mst;

	double mst_weight(const std::vector<std::vector<double> > &metric =
		std::vector<std::vector<double> > () ) const;
};

#endif
