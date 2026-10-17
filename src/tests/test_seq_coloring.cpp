/**
 * @file test_seq_coloring.cpp
 * @brief Unit tests for the sequential coloring heuristics in seq_coloring.h.
 *
 * @details Checks GreedyIsetSequentialColoring and GreedySequentialColoring on a
 * small graph, both for a vertex subset and for the full graph.
 *
 * @author pss
 * @date 2025-05-18
 */

#include "gtest/gtest.h"
#include "seq_coloring.h"

#include <iostream>

using namespace bitgraph;


/**
 * @brief Test fixture: 5-vertex undirected graph with edges 0-1, 1-2, 2-3, 1-3 (vertex 4 isolated).
 */
class SeqColorTest : public ::testing::Test {
protected:
	void SetUp() override {
		ug.reset(NV);
		ug.add_edge(0, 1);
		ug.add_edge(1, 2);
		ug.add_edge(2, 3);
		ug.add_edge(1, 3);
	}
	void TearDown() override {}

	//undirected graph instance	
	const int NV = 5;
	bitgraph::ugraph ug;
};

/** @brief GreedyIsetSequentialColoring on a vertex subset and on the full graph. */
TEST_F(SeqColorTest, seqIset) {

	////////////////////////////
	//color a subset of vertices
	typename ugraph::bitset_type bbsg{ static_cast<size_t>(NV), {1, 2, 4} };

	GreedyIsetSequentialColoring iset_seq_col(ug);
	int nCol = iset_seq_col.run(&bbsg);
	
	EXPECT_EQ(iset_seq_col.num_colors(), 2);
	EXPECT_EQ(iset_seq_col.color(1), 1);
	EXPECT_EQ(iset_seq_col.color(2), 2);
	EXPECT_EQ(iset_seq_col.color(4), 1);

	////////////////////////////
	//color the full graph
	nCol = iset_seq_col.run();

	EXPECT_EQ(iset_seq_col.num_colors(), 3);
	EXPECT_EQ(iset_seq_col.color(0), 1);
	EXPECT_EQ(iset_seq_col.color(1), 2);
	EXPECT_EQ(iset_seq_col.color(2), 1);
	EXPECT_EQ(iset_seq_col.color(3), 3);
	EXPECT_EQ(iset_seq_col.color(4), 1);

}

/** @brief GreedySequentialColoring on a vertex subset and on the full graph. */
TEST_F(SeqColorTest, seq) {

	using VertexList = std::vector<int>;

	////////////////////////////
	//color a subset of vertices
	VertexList subgraph = { 1, 2, 4 };

	GreedySequentialColoring seq_col(ug, static_cast<int>(subgraph.size()));
	int nCol = seq_col.run(&subgraph);

	EXPECT_EQ(seq_col.num_colors(), 2);
	EXPECT_EQ(seq_col.color(1), 1);
	EXPECT_EQ(seq_col.color(2), 2);
	EXPECT_EQ(seq_col.color(4), 1);

	////////////////////////////
	//color the full graph
	nCol = seq_col.run();

	EXPECT_EQ(seq_col.num_colors(), 3);
	EXPECT_EQ(seq_col.color(0), 1);
	EXPECT_EQ(seq_col.color(1), 2);
	EXPECT_EQ(seq_col.color(2), 1);
	EXPECT_EQ(seq_col.color(3), 3);
	EXPECT_EQ(seq_col.color(4), 1);

}

