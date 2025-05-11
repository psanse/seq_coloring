/**
* @file main_seq.cpp
* @brief main file for an efficient algorithm to solve the graph isomorphism problem 
* @author: pss
* @details created 27/03/2025
**/

using namespace std;

#include <iostream>
#include "graph/graph.h"
#include "bitscan/bitscan.h"


template<class Bitset_t>
class SEQ {

	std::vector<Bitset_t> bbc_;						//list of color (bit)sets.
	std::vector<int> lc_;							//[vertex_index] -> color number --- standard color encoding 

	std::size_t nV_;								//number of vertices	
	std::size_t maxCol_;							//maximum number of colors
	std::size_t nCol_;								//number of colors of the current coloring
public:
	//construction / destruction
	SEQ(std::size_t size, std::size_t MAX_COL = size) :
		nV_{ size },
		maxCol_(MAX_COL),
		lc_{ size, 0 },
		bbc_{ maxCol_ },							//internally, color numbers range from [0 , maxCol_)
	{		
		//allocate empty bitsets
		for (const bb& : bbc_) {
			bbc.reset(nV_);
		}	
	}

	//copy and move constructors forbidden
	SEQ(const SEQ&) = delete;
	SEQ(SEQ&&) = delete;
	~SEQ() = default;

	//setters and getters
	std::size_t number_of_colors() const { return nCol_; }
	std::size_t number_of_vertices() const { return nV_; }
	std::size_t maximum_number_of_colors() const { return maxCol_; }

	unsigned int color(int v) { return lc_[v]; }


	//TODO...

	///////////////
	//drivers

	

};

int main() {
	
	std::cout << " hello world" << std::endl;	

	//test vector of bitsets
	//bitarray bbc{ 10, {1, 5, 7} };

	std::vector<bitarray> bbc{ 2 };
	bbc[0].reset(2);
	bbc[1].reset(2);
	
	


}