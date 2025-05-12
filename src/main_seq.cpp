/**
* @file main_seq.cpp
* @brief main file for an efficient algorithm to solve the graph isomorphism problem 
* @author: pss
* @details created 27/03/2025
**/

using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>				//std::iota
#include "graph/graph.h"
#include "bitscan/bitscan.h"
#include "utils/common.h"

#include <cassert>


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
		maxCol_{ MAX_COL },
		lc_{ (int)size, 0 },
		bbc_{ maxCol_ }							//internally, color numbers range from [0 , maxCol_)
	{		
		//allocate empty bitsets
		for (auto& bb : bbc_) {
			bb.reset(nV_);
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
	/**
	* @brief computes greedy sequential coloring for the graph @ug
	* @param ug: input graph
	* @returns : number of colors required
	**/
	template<class BitSet_t>
	int seq_coloring(const ugraph& ug) {

		int nC = 1;


		//TODO...
		int pc = ug.size();
		std::vector<int> uncol{pc};
		std::iota(uncol.begin(), uncol.end(), 1);			//set of vertices
		com::stl::print_collection(uncol);
		
		///////////////////
		assert(pc >= 1);
		///////////////////

		//int col = 1, v = bbo::noBit, nBB = bbo::noBit;

		////main loop - greedy coloring	
		//typename Graph_t::_bbt bb_unsel(bbsg);
		//typename Graph_t::_bbt bb_sel(g.size());
		//while (true) {

		//	//load bb_sel with remaining vertices to be colored
		//	bb_sel = bb_unsel;

		//	//build a new iset with vertices from bb_sel
		//	bb_sel.init_scan(bbo::DESTRUCTIVE);
		//	while ((v = bb_sel.next_bit_del()) != bbo::noBit) {

		//		//update coloring
		//		if (ub) { ub[v] = col; }

		//		//remove colored vertex from bbsg
		//		bb_unsel.erase_bit(v);

		//		/////////////////////////////////
		//		if ((--pc) == 0) { return col; }				//early exit - all vertices colored
		//		/////////////////////////////////

		//		//removes neighbors of v
		//		bb_sel.erase_block(WDIV(v), -1, g.neighbors(v));
		//	}

		//	//open a new color
		//	++col;
		//}

		//return col;		//should not reach here


		


		return nC;
	}
	

};

int main() {
	
	std::cout << " hello world" << std::endl;	

	//test vector of bitsets
	//bitarray bbc{ 10, {1, 5, 7} };

	/*std::vector<bitarray> bbc{ 2 };
	bbc[0].reset(2);
	bbc[1].reset(2);*/
	
	ugraph ug{ 5 };
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 3);

	SEQ<bitarray> sq(ug.size(), 10);
	sq.seq_coloring<bitarray>(ug);
	



	


}