# Greedy sequential coloring heuristics
This repository contains implementations of the greedy sequential coloring heuristic (SEQ) and a variant called the independent set greedy sequential coloring heuristic (ISEQ). SEQ is a well-known vertex coloring heuristic that assigns color numbers to the vertices of a graph G=(V, E) sequentially. Specifically, it assigns to a vertex v from V taken in a predefined order the smallest **available color** (i.e., the smallest integer not used by its already-colored neighbors). ISEQ is a variant of SEQ that produces an independent (color) set at each iteration. 

The implementation uses the BitGraph library for efficiency. 
