# PFAPI21_Grilli_10630029

This project coming from the Polimi Course Algoritmi e Principi dell'Informatica.
Goals
Given two values:  
  -d -> number of nodes for each graph
  -k -> length of the ranking list
  -a set of graph (in matrix form) with d length
For each graph will calculate the distance (Dijkstra Algorithm) from the starting nodes (0,0) in the matrix to all nodes in the graph. Will then sum all distances and saved
in a rank.
The rank will keep the best k element (best = the smallest sum of distances).

There are two main commands:
  -AggiungiGrafo [adjacent matrix] that will evaluate the graph and will add it to the rank
  -TopK that will print the best graphs in the rank
  
Implementation solution:
  
   For evaluating the distances from the source node (0,0) to all nodes in the graph I have used the Dijkstra Algorithm and a MinHeap as structure, in order to extract always
   the node with the smallest path.
   For the ranking list I have used a MaxHeap in order to have always the element with the greatest distance at the top so it can be easy replaced (and checked) against new
   added graph.
		
NB:
	Nodes in the graph (adjacent matrix) that are not connected to the graph have a distance of 0.
	Graphs saved in the ranking list have only the ID (which is the order on which they have been inserted) and the total distance (sum of all distances from the root node).
	
