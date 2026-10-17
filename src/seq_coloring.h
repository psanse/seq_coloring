/**
 * @file seq_coloring.h
 * @brief Greedy sequential vertex coloring heuristics for undirected bit graphs.
 *
 * @details Provides an abstract base class holding the coloring state and two
 * concrete heuristics built on the BitGraph library (bitgraph::ugraph):
 *  - GreedyIsetSequentialColoring: builds one independent set (color class) at a time.
 *  - GreedySequentialColoring: assigns each vertex to the first compatible color class.
 *
 * Colors are numbered starting at 1. Both heuristics can color the full graph or
 * the subgraph induced by a given set of vertices.
 *
 * @author pss
 * @date 2025-05-18
 */

#ifndef SEQ_COLORING_H
#define SEQ_COLORING_H

#include <vector>
#include <numeric>
#include "graph/graph.h"
#include "bitscan/bitscan.h"

#include <cassert>

/**
 * @brief Abstract base class for greedy sequential coloring heuristics.
 *
 * @details Stores a reference to the graph to be colored, the resulting
 * vertex-to-color mapping and the number of colors used. Derived classes
 * implement the coloring algorithm itself. The graph must outlive the object.
 *
 * @tparam GraphT graph type (default bitgraph::ugraph)
 */
template<class GraphT = bitgraph::ugraph>
class BaseSequentialColoring {
public:

	using graph_type = GraphT;										///< graph type
	using bitset_type = typename graph_type::bitset_type;			///< bitset type of the graph
	using Vertex = bitgraph::vertex_t;								///< vertex index
	using IsetId = int;												///< color (independent set) number
	using VertexToIsetMapping = std::vector<IsetId>;				///< maps vertex index to color number

protected:
	/**
	 * @brief Constructs the base state for a non-empty graph.
	 * @param ug graph to be colored; stored by reference, must outlive this object
	 * @pre ug.size() > 0
	 */
	explicit BaseSequentialColoring(const bitgraph::ugraph& ug):
		ugraph_{ ug },
		vertex_coloring_(ugraph_.size(), 0)
	{
		assert(ug.size() > 0);			//assert graph is not empty
	}

public:
	/** @brief Number of colors used by the last coloring (0 if none has been computed). */
	int num_colors() const { return num_col_; }

	/**
	 * @brief Color assigned to a vertex by the last coloring.
	 * @param v vertex index
	 * @returns color number (starting at 1), or 0 if @p v was never colored
	 */
	int color(Vertex v) const { return vertex_coloring_[v]; }

	/** @brief Graph being colored. */
	const bitgraph::ugraph& graph()	const { return ugraph_; }

protected:
	const graph_type& ugraph_;					///< reference graph
	VertexToIsetMapping vertex_coloring_;		///< [vertex_index] -> color number --- standard color encoding
	int num_col_ = 0;							///< number of colors of the current coloring
};

/**
 * @brief Greedy independent-set sequential coloring heuristic (ISEQ) for bitgraph::ugraph.
 *
 * @details Builds one color class at a time: scans the uncolored vertices in
 * increasing index order, adds each to the current class and discards its
 * neighbors from the candidates. This is the heuristic used as upper bound in
 * the CliSAT maximum clique algorithm.
 *
 * Uses minimum memory (just two bitsets), at the price of a potential overhead
 * in running time.
 *
 * @note A previous coloring in vertex_coloring_ is not cleared between runs;
 *       entries of vertices outside the colored subgraph keep stale values.
 *
 * @todo Clear the previous coloring between runs.
 */
class GreedyIsetSequentialColoring : public BaseSequentialColoring<bitgraph::ugraph> {
public:

	using bitset_type = typename bitgraph::ugraph::bitset_type;				///< bitset type
	using Iset = bitset_type;												///< alias for semantic type: independent set
	using VertexBitset = bitset_type;										///< alias for semantic type: set of vertices

	/**
	 * @brief Constructs the heuristic for a graph.
	 * @param ug graph to be colored; stored by reference, must outlive this object
	 * @pre ug.size() > 0
	 */
	explicit GreedyIsetSequentialColoring(const bitgraph::ugraph& ug) : BaseSequentialColoring(ug),
		bbIset_{ ugraph_.size() },
		bbUncolored_{ ugraph_.size() , true }
	{}

	/**
	 * @brief Computes a greedy independent-set sequential coloring.
	 *
	 * @details Colors the subgraph induced by the vertices in @p pBBsg, or the
	 * whole graph if @p pBBsg is nullptr. The result is available through
	 * color() and num_colors().
	 *
	 * @param pBBsg bitset of the vertices inducing the subgraph to color;
	 *              nullptr (default) colors the full graph
	 * @returns number of colors used
	 */
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
	}

private:
	Iset bbIset_;							///< color iset under construction - candidate vertices that can still enlarge the current iset
	VertexBitset bbUncolored_;				///< set of uncolored vertices
};

/**
 * @brief Greedy sequential coloring heuristic for bitgraph::ugraph.
 *
 * @details Processes the vertices in the given order (increasing index for the
 * full graph) and assigns each one to the first color class that contains none
 * of its neighbors, opening a new color if there is none.
 *
 * Requires an upper bound on the number of colors at construction, used to
 * preallocate the color classes.
 *
 * @note A previous coloring in vertex_coloring_ is not cleared between runs;
 *       entries of vertices outside the colored subgraph keep stale values.
 *
 * @todo Clear the previous coloring between runs.
 */
class GreedySequentialColoring : public BaseSequentialColoring<bitgraph::ugraph>
{
public:

	using bitset_type = typename bitgraph::ugraph::bitset_type;		///< bitset type
	using IsetPartition = std::vector<bitset_type>;					///< color classes, indexed by color number

	/**
	 * @brief Constructs the heuristic for a graph.
	 * @param ug graph to be colored; stored by reference, must outlive this object
	 * @param MAX_COL upper bound on the number of colors that will be needed
	 *                (e.g. the number of vertices to be colored)
	 * @pre 0 < MAX_COL <= ug.size()
	 */
	GreedySequentialColoring(const bitgraph::ugraph& ug, int MAX_COL) : BaseSequentialColoring(ug),
		iset_partition_(MAX_COL + 1, bitset_type{ ug.size() })			// color number index starts at 1
	{
		assert(MAX_COL > 0);								//assert number of colors is positive
		assert(MAX_COL <= ug.size());						//assert number of colors is less than number of vertices
	}

	/**
	 * @brief Computes a greedy sequential coloring.
	 *
	 * @details Colors the subgraph induced by the vertices in @p psg, taken in
	 * list order, or the whole graph if @p psg is nullptr. The result is
	 * available through color() and num_colors(). Does not clear a previous
	 * coloring between runs.
	 *
	 * @param psg list of vertices inducing the subgraph to color;
	 *            nullptr (default) colors the full graph
	 * @returns number of colors used
	 * @pre the number of colors needed does not exceed the MAX_COL given at construction
	 */
	int run(const std::vector<int>* psg = nullptr) {

		const int pc = static_cast<int>(psg ? psg->size() : this->ugraph_.size());

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
		for (const Vertex v : uncol) {

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

private:
	IsetPartition iset_partition_;						///< color (bit) sets, index 0 unused
};

#endif // SEQ_COLORING_H