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

template<class BitSet_t>
struct BaseSeq{
	std::vector<BitSet_t> bbc_;			//list of color (bit)sets.
	std::vector<int> lc_;				//[vertex_index] -> color number --- standard color encoding 

	int nV_;							//number of vertices
	int nCol_;							//number of colors of the current coloring
	int maxCol_;						//maximum number of colors of the current coloring

	//construction / destruction
	BaseSeq(int size, int MAX_COL = size) :
		nV_{ size },
		maxCol_{ MAX_COL },
		lc_((int)size, 0),
		bbc_(MAX_COL, BitSet_t{ size })		//internally, color numbers range from [0 , maxCol_)
	{}

	//interface
	virtual int seq_coloring(const ugraph& ug, BitSet_t bbsg) = 0;
	virtual int seq_coloring(const ugraph& ug, std::vector<int> lvsg) = 0;
	virtual int seq_coloring(const ugraph& ug) = 0;

};


template<class BitSet_t>
class SEQ {

	std::vector<BitSet_t> bbc_;						//list of color (bit)sets.
	std::vector<int> lc_;							//[vertex_index] -> color number --- standard color encoding 

	int nV_;										//number of vertices	
	int maxCol_;									//maximum number of colors
	int nCol_ = 0;									//number of colors of the current coloring
public:
	//construction / destruction
	SEQ(int size, int MAX_COL = size) :
		nV_{ size },
		maxCol_{ MAX_COL },
		lc_( (int)size, 0 ),
		bbc_(MAX_COL, BitSet_t{ size })				//internally, color numbers range from [0 , maxCol_)
	{}

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
	int seq_coloring(const ugraph& ug) {

		int pc = ug.size();

		///////////////////
		assert(pc >= 1);
		///////////////////
		
		//set list of all vertices
		std::vector<int> uncol;
		uncol.resize(pc);									//set of uncolored vertices
		std::iota(uncol.begin(), uncol.end(), 0);			//set of vertices
		
		//I/O
		//com::stl::print_collection(uncol);
			

		//TODO...
		int nC = 1;
		bool color_found;
		for (const auto& v : uncol) {

			//find first open bit color set with no vertices adjacent to v
			color_found = false;
			for (auto c = 1; c <= nC; ++c) {
				int w = find_first_common_block(0, WDIV(v), ug.neighbors(v), bbc_[c]);
				if (w == bbo::noBit) {

					//enlarge color set, set color number
					bbc_[c].set_bit(v);						
					lc_[v] = c;	

					bbc_[c].print();
					
					color_found = true;
					break;
				}
			}

			//open a new color
			if (!color_found) {
				++nC;

				//add vertex to color set
				bbc_[nC].set_bit(v);
			}
		}

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

	SEQ<bitarray> sq{ 5, 3 };
	int nCol = sq.seq_coloring(ug);

	cout << "number of colors: " << nCol << endl;	


	/*std::vector<bitarray> kk{ 2 };
	kk[0].reset(10);*/
	



	


}