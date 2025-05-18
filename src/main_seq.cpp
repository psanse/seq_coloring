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

///////////////////////
//
// Base class for greedy independent set sequential coloring heuristic
//
///////////////////////

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
// class seqIset
// (computes greedy independent set sequential coloring heuristic for 
// ugraph type) 
//
///////////////////////

class seqIset : public  baseSeq<ugraph> {
public:

	using _bbt = typename ugraph::_bbt;	//bitset type

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
	int seq_coloring(const _bbt* pBBsg = nullptr) {

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
	_bbt bbSel_;					//color iset under construction
	_bbt bbUnsel_;					//set of uncolored vertices
};


///////////////////////
//
// class seq
// (computes greedy sequential coloring heuristic for 
// ugraph type) 
//
///////////////////////

class seq : public  baseSeq<ugraph> {
public:
	using _bbt = typename ugraph::_bbt;						//bitset type

	//construction / destruction
	seq(const ugraph& ug, int MAX_COL) : baseSeq(ug),
		bbc_(MAX_COL + 1, _bbt{ (int)ug.size() })			// color number index starts at 1 			
	{
		assert(MAX_COL > 0);								//assert number of colors is positive
		assert(MAX_COL <= ug.size());						//assert number of colors is less than number of vertices
	}

	///////////////
	//drivers

	/**
	* @brief computes greedy sequential coloring for the induced subgraph
	*		 in @ug_ by @psg
	* @returns : number of colors required
	* @details: the coloring is provided in @lc_ (base class)
	**/
	int seq_coloring(const std::vector<int>* psg = nullptr) {

		int pc;
		(psg != nullptr) ? pc = psg->size() : pc = this->ug_.size();
			

		//set list of all vertices
		std::vector<int> uncol;
		if (psg == nullptr) {
			uncol.resize(pc);
			std::iota(uncol.begin(), uncol.end(), 0);			//all of vertices in ug_
		}
		else {
			uncol = *psg;
		}

		//I/O
		//com::stl::print_collection(uncol);


		//TODO...
		this->nCol_ = 1;
		bool open_color = false;
		for (const auto& v : uncol) {

			//find first open bit color set with no vertices adjacent to v
			open_color = true;
			for (auto c = 1; c <= nCol_; ++c) {
				int w = find_first_common_block(0, WDIV(v), this->ug_.neighbors(v), bbc_[c]);
				
				//if no adjacent vertex is found in color c, add vertex v to color c
				if (w == bbo::noBit) {

					//add vertex to color c
					lc_[v] = c;
					bbc_[c].set_bit(v);
					
										
					//set
					open_color = false;
					break;
				}
			}

			//open a new color if no iset is available for v
			if (open_color) {
				++this->nCol_;

				//cleans the open bitset color - necessary for seq_coloring to work for different runs
				bbc_[this->nCol_].erase_bit();

				//add vertex to color set
				lc_[v] = this->nCol_;
				bbc_[this->nCol_].set_bit(v);
			}
		}

		return this->nCol_;
	}


private:
	std::vector<_bbt> bbc_;						//list of color (bit)sets.

};


int main() {
	const int SIZE = 5;

	ugraph ug{ SIZE };
	ug.add_edge(0, 1);
	ug.add_edge(1, 2);
	ug.add_edge(2, 3);
	ug.add_edge(1, 3);

	//A)

	std::vector<int> sg = { 1, 2, 4 };

	seq sq{ug, (int)ug.size()};
	int nCol = sq.seq_coloring(&sg);
	assert(sq.color(1) == 1);
	assert(sq.color(2) == 2);
	assert(sq.color(4) == 1); 

	cout << "number of colors: " << nCol << endl;


	//B)
	typename ugraph::_bbt bbsg{ SIZE, {1, 2, 4} };

	seqIset sqI(ug);
	nCol = sqI.seq_coloring(&bbsg);

	cout << "number of colors: " << nCol << endl;
	assert(sqI.color(1) == 1);
	assert(sqI.color(2) == 2);
	assert(sqI.color(4) == 1);


}