/**
 * @file main_seq.cpp
 * @brief Main executable for sequential vertex coloring heuristics.
 *
 * @details Provides the entry point for running and evaluating
 * sequential coloring algorithms.
 *
 * @author pss
 * @date 2025-03-27
 * @since 2025-03-27
 * @version 2026-02-05
 */

#include <iostream>
#include "seq_coloring.h"


/**
 * @brief Demo: colors a small graph with both sequential heuristics.
 * @details Colors the subgraph induced by vertices {1, 2, 4} and prints the number of colors.
 * @returns 0 on success
 */
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
	int nCol = seq_col.compute_coloring(&subgraph);
	assert(seq_col.color(1) == 1);
	assert(seq_col.color(2) == 2);
	assert(seq_col.color(4) == 1); 

	std::cout << "number of colors: " << nCol << std::endl;


	//B) greedy iset GreedySequentialColoring
	typename bitgraph::ugraph::bitset_type bbsg{ SIZE, {1, 2, 4} };

	GreedyIsetSequentialColoring iset_seq_col{ ug };
	nCol = iset_seq_col.compute_coloring(&bbsg);

	std::cout << "number of colors: " << nCol << std::endl;
	assert(iset_seq_col.color(1) == 1);
	assert(iset_seq_col.color(2) == 2);
	assert(iset_seq_col.color(4) == 1);


}