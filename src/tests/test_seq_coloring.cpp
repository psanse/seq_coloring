/**
* @file test_seq_coloring.cpp
* @brief Unit tests for sequential coloring
* @details created 18/05/2025
* @dev pss
**/

#include "gtest/gtest.h"
#include "seq_coloring.h"

#include <iostream>

using namespace std;

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

	//directed graph instance	
	const int NV = 5;
	ugraph ug;
};

TEST_F(SeqColorTest, seqIset) {

	////////////////////////////
	//color a subset of vertices
	typename ugraph::_bbt bbsg{ NV, {1, 2, 4} };

	seqIset sqI(ug);
	int nCol = sqI.seq_coloring(&bbsg);
	
	EXPECT_EQ(sqI.number_of_colors(), 2);
	EXPECT_EQ(sqI.color(1), 1);
	EXPECT_EQ(sqI.color(2), 2);
	EXPECT_EQ(sqI.color(4), 1);

	////////////////////////////
	//color the full graph
	nCol = sqI.seq_coloring();

	EXPECT_EQ(sqI.number_of_colors(), 3);
	EXPECT_EQ(sqI.color(0), 1);
	EXPECT_EQ(sqI.color(1), 2);
	EXPECT_EQ(sqI.color(2), 1);
	EXPECT_EQ(sqI.color(3), 3);
	EXPECT_EQ(sqI.color(4), 1);

}

TEST_F(SeqColorTest, seq) {

	////////////////////////////
	//color a subset of vertices
	std::vector<int> sg = { 1, 2, 4 };

	seq sq(ug, sg.size());
	int nCol = sq.seq_coloring(&sg);

	EXPECT_EQ(sq.number_of_colors(), 2);
	EXPECT_EQ(sq.color(1), 1);
	EXPECT_EQ(sq.color(2), 2);
	EXPECT_EQ(sq.color(4), 1);

	////////////////////////////
	//color the full graph
	nCol = sq.seq_coloring();

	EXPECT_EQ(sq.number_of_colors(), 3);
	EXPECT_EQ(sq.color(0), 1);
	EXPECT_EQ(sq.color(1), 2);
	EXPECT_EQ(sq.color(2), 1);
	EXPECT_EQ(sq.color(3), 3);
	EXPECT_EQ(sq.color(4), 1);

}

