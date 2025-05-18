/**
* @file main_seq.cpp
* @brief unit tests for sequential coloring heuristics
* @author: pss
* @details created 27/03/2025, last update 18/05/2025
**/

using namespace std;

#include <iostream>
#include "seq_coloring.h"


/////////////////
// unit tests

int main() {
	const int SIZE = 5;

	ugraph ug{ SIZE };
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 3);
	ug.add_edge(1, 3);

	//A) greedy seq
	std::vector<int> sg = { 1, 2, 4 };

	seq sq{ug, (int)ug.size()};
	int nCol = sq.seq_coloring(&sg);
	assert(sq.color(1) == 1);
	assert(sq.color(2) == 2);
	assert(sq.color(4) == 1); 

	cout << "number of colors: " << nCol << endl;


	//B) greedy iset seq
	typename ugraph::_bbt bbsg{ SIZE, {1, 2, 4} };

	seqIset sqI(ug);
	nCol = sqI.seq_coloring(&bbsg);

	cout << "number of colors: " << nCol << endl;
	assert(sqI.color(1) == 1);
	assert(sqI.color(2) == 2);
	assert(sqI.color(4) == 1);


}