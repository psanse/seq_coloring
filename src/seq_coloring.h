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

#include <cassert>

///////////////////////
//
// Base Abstract class for greedy sequential coloring heuristic
//
///////////////////////

template<class GraphT = bitgraph::ugraph>
class BaseSequentialColoring {
public:
	
	using graph_type = GraphT;										
	using bitset_type = typename graph_type::bitset_type;			
	using Vertex = int;
	using IsetId = int;
	using VertexToIsetMapping = std::vector<IsetId>;			// maps vertex index to color number

protected:
	//construction / destruction
	BaseSequentialColoring(const bitgraph::ugraph& ug):
		ugraph_{ ug },
		vertex_coloring_(ugraph_.size(), 0)
	{
		assert(ug.size() > 0);			//assert graph is not empty
	}
	
public:
	//setters and getters
	int num_colors() const { return num_col_; }
	int color(int v) const { return vertex_coloring_[v]; }	
	const bitgraph::ugraph& graph()	const { return ugraph_; }

protected:
	const graph_type& ugraph_;					// reference graph
	VertexToIsetMapping vertex_coloring_;		// [vertex_index] -> color number --- standard color encoding 
	int num_col_ = 0;							// number of colors of the current coloring
};

///////////////////////
//
// class GreedyIsetSequentialColoring
// 
// @brief: computes greedy independent set sequential coloring heuristic for 
//		   ugraph type
// @details:  derives from BaseSequentialColoring class
// @details:  minimum memory allocation - requires just two bitsets. Potential overehead in running time. 
//			  Heuristic used by CliSAT algorithm.
// 
// TODO: does not clear a previous coloring @vertex_coloring_ between runs	
//
///////////////////////

class GreedyIsetSequentialColoring : public  BaseSequentialColoring<bitgraph::ugraph> {
public:

	using bitset_type = typename bitgraph::ugraph::bitset_type;				// bitset type
	using Iset = bitset_type;												// alias for semantic type
	using VertexBitset = bitset_type;										// alias for semantic type
	

	//construction / destruction
	GreedyIsetSequentialColoring(const bitgraph::ugraph& ug) : BaseSequentialColoring(ug),
		bbIset_{ ugraph_.size() },
		bbUncolored_{ ugraph_.size() , true }
	{}
	
	/**
	* @brief computes greedy sequential coloring for the subgraph induced by set of vertices in 
	*		 the graph @v ugraph_.
	*		 If @pBBsg is nullptr, computes a coloring for the full graph @ugraph_.
	* @param pBBsg: bitset of vertices that induce the subgraph to be colored;
	*				can be nullptr (default) to color the full graph @ugraph_
	* @returns: number of colors used in the coloring
	**/
	int run(const bitset_type* pBBsg = nullptr) {

		//sets the subgraph to be colored in bbUncolored_
		if (pBBsg) { bbUncolored_ = *pBBsg; }
		else { bbUncolored_.set_bit(0, static_cast<int>(ugraph_.size()) - 1); }	//all vertices are colored

		this->num_col_ = 1;
		int pc = bbUncolored_.count();
		int v = bitgraph::bbo::noBit;
		while (true) {

			//set bb_sel to the remaining vertices to be colored
			this->bbIset_ = this->bbUncolored_;

			//build a new iset with vertices from bb_sel
			this->bbIset_.init_scan(bitgraph::bbo::NON_DESTRUCTIVE);
			while ((v = this->bbIset_.next_bit()) != bitgraph::bbo::noBit) {

				//update coloring
				this->vertex_coloring_[v] = this->num_col_;

				//early exit - all vertices colored
				if ((--pc) == 0) { return this->num_col_; }

				//removes neighbors of v from the color set under construction
				this->bbIset_.erase_block(WDIV(v), -1, ugraph_.neighbors(v));

				//remove colored vertex from uncolored set
				this->bbUncolored_.erase_bit(v);
			}

			//open a new color
			++this->num_col_;
		}

		///////////////////	
		assert(false);				//should not reach here
		//////////////////

		return this->num_col_;
	}

/////////
//data members
private:
	Iset bbIset_;							// color iset under construction - selected vertices that can potentially enlarge the current iset
	VertexBitset bbUncolored_;				// set of uncolored vertices
};

///////////////////////
//
// class GreedySequentialColoring
// 
// @brief: computes greedy sequential coloring heuristic for 
//		   ugraph type
// @details:  derives from BaseSequentialColoring class
// @details: requires an initial estimate of the maximum number of colors
// 
// TODO: does not clear a previous coloring @vertex_coloring_ between runs
//
///////////////////////

class GreedySequentialColoring : public  BaseSequentialColoring<bitgraph::ugraph>
{
public:

	using bitset_type = typename bitgraph::ugraph::bitset_type;
	using IsetPartition = std::vector<bitset_type>;				

	//construction / destruction
	GreedySequentialColoring(const bitgraph::ugraph& ug, int MAX_COL) : BaseSequentialColoring(ug),
		iset_partition_(MAX_COL + 1, bitset_type{ ug.size() })			// color number index starts at 1 			
	{
		assert(MAX_COL > 0);								//assert number of colors is positive
		assert(MAX_COL <= ug.size());						//assert number of colors is less than number of vertices
	}

	/**
	* @brief computes greedy sequential coloring for the induced subgraph
	*		 in @ugraph_ by @psg. If @psg is nullptr, computes a coloring for the full graph @ugraph_.
	* @returns : number of colors required
	* @details: the coloring is provided in @vertex_coloring_ (base class)
	* @details: does not clear previous coloring @vertex_coloring_ between runs
	**/
	int run(const std::vector<int>* psg = nullptr) {

		int pc;
		pc = static_cast<int>(psg ? psg->size() : this->ugraph_.size());
		
		//set list of all vertices
		std::vector<int> uncol;
		if (psg == nullptr) {
			uncol.resize(pc);
			std::iota(uncol.begin(), uncol.end(), 0);			//all of vertices in ugraph_
		}
		else {
			uncol = *psg;
		}

		//main loop
		this->num_col_ = 1;
		bool open_color = false;
		iset_partition_[this->num_col_].erase_bit();							//cleans the first bitset color (must be open)
		for (const auto& v : uncol) {

			//find first open bit color set with no vertices adjacent to v
			open_color = true;
			for (auto c = 1; c <= num_col_; ++c) {

				int w = find_first_common_block(0, WDIV(v), this->ugraph_.neighbors(v), iset_partition_[c]);
				
				//if no adjacent vertex is found in color c, add vertex v to color c
				if (w == bitgraph::bbo::noBit) {

					//add vertex to color c
					vertex_coloring_[v] = c;
					iset_partition_[c].set_bit(v);
					
										
					//set flag - do not open a new color
					open_color = false;
					break;
				}
			}

			//open a new color if no iset is available for v
			if (open_color) {
				++this->num_col_;

				//cleans the open bitset color - necessary for run to work for different runs
				iset_partition_[this->num_col_].erase_bit();

				//add vertex to color set
				vertex_coloring_[v] = this->num_col_;
				iset_partition_[this->num_col_].set_bit(v);
			}
		}

		return this->num_col_;
	}

/////////
//data members
private:
	IsetPartition iset_partition_;						//list of color (bit) sets.
};

