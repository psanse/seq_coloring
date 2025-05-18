/**
* @file seq_coloring.h
* @brief header class for sequential coloring heuristics
* @dev: pss
* @details created 18/05/2025
**/

#include <iostream>
#include <vector>
#include <numeric>				
#include "graph/graph.h"
#include "bitscan/bitscan.h"
#include "utils/common.h"

#include <cassert>

///////////////////////
//
// Base class for greedy sequential coloring heuristic
// (cannot create an instance)
//
///////////////////////

template<class Grapht_t = ugraph>
class baseSeq {
public:
	
	using _gt = Grapht_t;						//graph type
	using _bbt = typename _gt::_bbt;			//bitset type

protected:
	//construction / destruction
	baseSeq(const ugraph& ug):
		ug_{ ug },
		lc_(ug_.size(), 0)
	{
		assert(ug.size() > 0);			//assert graph is not empty
	}
	
public:
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
// 
// @brief: computes greedy independent set sequential coloring heuristic for 
//		   ugraph type
// @details:  derives from baseSeq class
// @details:  minimum memory allocation - requires just two bitsets. Potential overehead in running time. 
//			  Heuristic used by CliSAT algorithm.
// 
// TODO: does not clear a previous coloring @lc_ between runs	
//
///////////////////////

class seqIset : public  baseSeq<ugraph> {
public:

	using _bbt = typename ugraph::_bbt;				//bitset type

	//construction / destruction
	seqIset(const ugraph& ug) : baseSeq(ug),
		bbSel_{ (int)ug_.size() },
		bbUnsel_{ (int)ug_.size() , true }
	{}
	
	/**
	* @brief computes greedy sequential coloring for the induced subgraph by the (bit) set @pBBsg
	*		 in the graph @ug_. If @pBBsg is nullptr, computes a coloring for the full graph @ug_. 
	* @returns: number of colors used in the coloring
	**/
	int seq_coloring(const _bbt* pBBsg = nullptr) {

		//sets the subgraph to be colored in bbUnsel_
		if (pBBsg) { bbUnsel_ = *pBBsg; }
		else { bbUnsel_.set_bit(0, ug_.size() - 1); }	//all vertices are colored

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
	_bbt bbSel_;					//color iset under construction - selected vertices that can potentially enlarge the current iset
	_bbt bbUnsel_;					//set of uncolored vertices
};

///////////////////////
//
// class seq
// 
// @brief: computes greedy sequential coloring heuristic for 
//		   ugraph type
// @details:  derives from baseSeq class
// @details: requires an initial estimate of the maximum number of colors
// 
// TODO: does not clear a previous coloring @lc_ between runs
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

	/**
	* @brief computes greedy sequential coloring for the induced subgraph
	*		 in @ug_ by @psg. If @psg is nullptr, computes a coloring for the full graph @ug_.
	* @returns : number of colors required
	* @details: the coloring is provided in @lc_ (base class)
	* @details: does not clear previous coloring @lc_ between runs
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

		//main loop
		this->nCol_ = 1;
		bool open_color = false;
		bbc_[this->nCol_].erase_bit();							//cleans the first bitset color (must be open)
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
					
										
					//set flag - do not open a new color
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

/////////
//data members
private:
	std::vector<_bbt> bbc_;						//list of color (bit) sets.

};

