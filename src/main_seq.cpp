/**
* @file main_seq.cpp
* @brief unit tests for sequential coloring heuristics
* @author: pss
* @date 27/03/2025
* @last modified 05/02/2026
**/

using namespace std;

#include <iostream>
#include "seq_coloring.h"

/////////////////
// executable - see also unit tests

int main() {
	using VertexList = std::vector<int>;

	const int SIZE = 5;

	bitgraph::ugraph ug{ SIZE };
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 3);
	ug.add_edge(1, 3);

	//A) GreedySequentialColoring
	VertexList subgraph = { 1, 2, 4 };

	GreedySequentialColoring seq_col{ug, ug.num_vertices()};
	int nCol = seq_col.run(&subgraph);
	assert(seq_col.color(1) == 1);
	assert(seq_col.color(2) == 2);
	assert(seq_col.color(4) == 1); 

	cout << "number of colors: " << nCol << endl;


	//B) greedy iset GreedySequentialColoring
	typename bitgraph::ugraph::bitset_type bbsg{ SIZE, {1, 2, 4} };

	GreedyIsetSequentialColoring iset_seq_col(ug);
	nCol = iset_seq_col.run(&bbsg);

	cout << "number of colors: " << nCol << endl;
	assert(iset_seq_col.color(1) == 1);
	assert(iset_seq_col.color(2) == 2);
	assert(iset_seq_col.color(4) == 1);


}