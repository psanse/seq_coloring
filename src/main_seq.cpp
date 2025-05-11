/**
* @file main_seq.cpp
* @brief main file for an efficient algorithm to solve the graph isomorphism problem 
* @author: pss
* @details created 27/03/2025
**/

using namespace std;

#include <iostream>
#include "graph/graph.h"
//#include "graph_isomorphism.h"

int main() {
	
	
	//two input simple undirected graphs
	ugraph g1;
	ugraph g2;

	//example A
	/*g1.reset(10);
	g2.reset(10);	
	g1.add_edge(0, 1);
	g1.add_edge(0, 2);
	g2.add_edge(3, 4);
	g2.add_edge(3, 5);*/

	//example B
	/*g1 = ugraph::make_clique(2000);
	g2 = ugraph::make_clique(2000);
	g1.remove_edge(0, 1);
	g1.remove_edge(0, 2);
	g2.remove_edge(100, 1099);
	g2.remove_edge(101, 1000);*/
		

	///////////////////////////////////////////////////
	////TODO - determine if g1 and g2 are isomorphic
	//////////////////////////////////////////////////
	//GraphIsomorphism<> gi(g1, g2);

	////computes distance invariant features for all vertices
	//gi.compute_orderings();

	////check if the two graphs are isomorphic according to the orderings
	//if (gi.is_isomorphic(gi.get_orderings()))  {
	//	LOG_INFO("Graphs are isomorphic");
	//}
	//else {
	//	LOG_INFO("Graphs are not isomorphic - not 100%% sure but almost :-");
	//}


	std::cout << " hello world" << std::endl;	
}