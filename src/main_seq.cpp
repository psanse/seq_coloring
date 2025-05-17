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

///////////////////////
//
// Base class for greedy sequential coloring heuristic
// (a vertex is colored at each iteration)
//
///////////////////////

template<class BitSet_t>
struct BaseSeq{
	std::vector<BitSet_t> bbc_;			//list of color (bit)sets.
	std::vector<int> lc_;				//[vertex_index] -> color number --- standard color encoding 

	int nV_;							//number of vertices
	int maxCol_; 						//maximum number of colors of the current 
	int nCol_ = 0;						//number of colors of the current coloring

	//construction / destruction
	BaseSeq(int size, int MAX_COL) :
		nV_{ size },
		maxCol_{ MAX_COL },
		lc_(size, 0),
		bbc_(MAX_COL, BitSet_t{ size })		//internally, color numbers range from [0 , maxCol_)
	{}

	BaseSeq(int size) :
		nV_{ size },
		lc_((int)size, 0),
		maxCol_{ -1 }		
	{}

	BaseSeq() : nV_(-1), maxCol_(-1) {}

	//setters and getters	
	int number_of_vertices_colored() const { return nV_; }
	int maximum_number_of_colors() const { return maxCol_; }
	int number_of_colors() const { return nCol_; }

	//interface
	virtual int seq_coloring(const ugraph& ug, BitSet_t bbsg) = 0;
	virtual int seq_coloring(const ugraph& ug, std::vector<int> lvsg) = 0;
	virtual int seq_coloring(const ugraph& ug) = 0;

};

template<class Grapht_t = ugraph>
class baseSeq {
public:
	
	//construction / destruction
	baseSeq(const ugraph& ug):
		ug_{ ug },
		lc_(ug_.size(), 0)
	{
		assert(ug.size() > 0);			//assert graph is not empty
	}
	
	//setters and getters
	int number_of_colors()	const { return nCol_; }
	int color(int v)		const { return lc_[v]; }	
	const ugraph& graph()	const { return ug_; }

protected:
	const Grapht_t& ug_;				//input graph
	std::vector<int> lc_;				//[vertex_index] -> color number --- standard color encoding 
	int nCol_ = 0;						//number of colors of the current coloring
};

///////////////////////
//
// Base class for greedy independent set sequential coloring heuristic
// (color isets are computed at each iteration)
//
///////////////////////

class seqIset : public  baseSeq<ugraph> {

	using bitset = typename ugraph::_bbt;

public:
	//construction / destruction
	seqIset(const ugraph& ug) : baseSeq(ug),
		bbSel_{ (int)ug_.size() },
		bbUnsel_{ (int)ug_.size() , true }
	{}
	
	/**
	* @brief computes greedy sequential coloring for the induced subgraph by the (bit) set @pBBsg
	*		 in the graph @ug_. if @pBBsg is nullptr, computes a coloring for @ug_	* 
	* @returns: number of colors used in the coloring
	**/
	int seq_coloring(const bitset* pBBsg = nullptr) {

		//sets the subgraph to be colored in bbUnsel_
		if (pBBsg) { bbUnsel_ = *pBBsg; }
		else { bbUnsel_.set_bit(0, ug_.size()-1); }	//all vertices are colored

		this->nCol_ = 1;
		int pc = bbUnsel_.size();
		int v = bbo::noBit;
		while (true) {

			//set bb_sel to the remaining vertices to be colored
			this->bbSel_ = this->bbUnsel_;

			//build a new iset with vertices from bb_sel
			this->bbSel_.init_scan(bbo::NON_DESTRUCTIVE);
			while ((v = this->bbSel_.next_bit()) != bbo::noBit) {

				//update coloring
				this->lc_[v] = this->nCol_;

				//early exit - all vertices colored
				if ((--pc) == 0) { return this->nCol_; }


				//removes neighbors of v from the color set under construction
				this->bbSel_.erase_block(WDIV(v), -1, ug_.neighbors(v));

				//remove colored vertex from uncolored set
				this->bbUnsel_.erase_bit(v);
			}

			//open a new color
			++this->nCol_;
		}

		///////////////////	
		assert(false);				//should not reach here
		//////////////////

		return this->nCol_;
	}

	/////////
	//data members
private:
	bitset bbSel_;					//color iset under construction
	bitset bbUnsel_;					//set of uncolored vertices
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
	const int SIZE = 5;

	ugraph ug{ SIZE };
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 3);
	ug.add_edge(1, 3);

	//A)
	//SEQ<bitarray> sq{ 5, 3 };
	//int nCol = sq.seq_coloring(ug);


	//B)
	//seqIset<bitarray> sqI(SIZE);
	//int nCol = sqI.seq_coloring(ug);

	//cout << "number of colors: " << nCol << endl;	
	//assert(sqI.color(0) == 1);
	//assert(sqI.color(1) == 2);
	//assert(sqI.color(2) == 1);
	//assert(sqI.color(3) == 3);

	//C)
	typename ugraph::_bbt bbsg{ SIZE, {1, 2, 4} };

	bbsg.print();

	seqIset sqI(ug);
	int nCol = sqI.seq_coloring(&bbsg);

	cout << "number of colors: " << nCol << endl;
	assert(sqI.color(1) == 1);
	assert(sqI.color(2) == 2);
	assert(sqI.color(4) == 1);



	/*std::vector<bitarray> kk{ 2 };
	kk[0].reset(10);*/
	



	


}