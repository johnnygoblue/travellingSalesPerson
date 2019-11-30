// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#ifndef FASTTSP_H
#define FASTTSP_H

#include "SimpleGraph.h"

class FASTTSP : virtual public SimpleGraph {
public:

	FASTTSP(std::istream &is, bool border);

	void arbitrary_insertion(const std::vector<std::vector<double> > &metric =
		std::vector<std::vector<double> > () );

	void print_tour(std::ostream &os) const;

protected:

	std::vector<std::vector<Vertex>::iterator> tour;

	double tour_distance(size_t num_elements, const std::vector<std::vector<double> > &metric =
		std::vector<std::vector<double> > () ) const;

	std::vector<Vertex>::iterator closest_valid_vertex(std::vector<Vertex>::iterator &current);

	bool next_combination(unsigned &i, unsigned &j) const;
};

#endif
