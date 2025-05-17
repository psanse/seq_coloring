/**
* @file test_seq_coloring.cpp
* @brief Unit tests for sequential coloring
* @details created 11/05/2025
* @author pss
**/

#include "gtest/gtest.h"
#include "graph/graph.h"


#include <iostream>

using namespace std;

//
//
//TEST(Coloring, SEQ_Iset) {
//
//
//	const int NV = 6;
//
//	ugraph ug(NV);
//	ug.add_edge(0, 1);
//	ug.add_edge(0, 3);
//	ug.add_edge(0, 4);
//	ug.add_edge(2, 5);
//	ug.add_edge(3, 4);
//
//
//	SEQ
//
//	std::vector<int> clq;
//	//typename ugraph::_bbt bbsg{ static_cast<int>(ug.size()), { 0, 1, 2, 3, 4, 5} };
//
//
//	////////////////////////////////////////
//	int ub[NV];
//	int nCol = gfunc::clq::SEQ(ug, bbsg, ub);
//	///////////////////////////////////////
//
//	EXPECT_EQ(3, nCol);		//{0, 2} {1, 3, 5} {4} - 3 colors
//	EXPECT_EQ(1, ub[0]);
//	EXPECT_EQ(1, ub[2]);
//	EXPECT_EQ(2, ub[1]);
//	EXPECT_EQ(2, ub[3]);
//	EXPECT_EQ(2, ub[5]);
//	EXPECT_EQ(3, ub[4]);
//
//}