# PFAPI21_Grilli_10630029

This project is made for the Polimi Course Algoritmi e Principi dell'Informatica of Politecnico di Milano.
## Goals:
The goal of this project is to manage the ranking of k best direct weighted graph.

## Inputs and commands:
The input values are passed only once:  
  - d -> number of nodes for each graph
  - k -> length of the ranking list
  
Then a sequence of commands is passed to the program:
  - AggiungiGrafo -> which requires in input an adjacent matrix of a graph of length d
  - TopK -> that will print in the output the top best graph so far

## AggiungiGrafo:
Every time a new AggiungiGrafo command is passed to the program, the graph will be evaluated using the Dijkstra Algorithm. Starting from the node (0,0) will then calculate the distance to all other nodes and sum them up. Each graph is then tagged with the total distance and an index. Then is put inside the ranking list.
The rank will keep only the best k element (best means the smallest sum of distances).

## TopK:
This command requires the program to print the indices of the smallest k graph seen so far. If there are graphs with the same distance, then the oldest graph has precedence over the newer ones.
  
## Implementation solution:
  
For evaluating the distances from the source node (0,0) to all other nodes in the graph I have used the Dijkstra Algorithm. The structure to support the algorithm is a MinHeap to extract always the node with the smallest path to the considered node.

To support the ranking list I have used a MaxHeap data structure to have always the element with the greatest distance at the top. In this case node at the root can be easily checked against new graphs and in case, can be replaced.

Both Maxheap and Minheap are implemented using a static array. The complexity is:
- Getting the max/min node: O(1)
- Insert a new node or remove the max/min node: O(Log(n)) where n is the number of nodes in the structure

## Author:
Grilli Francesco




	
