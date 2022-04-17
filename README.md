# Assignment-4
Garbage Collection through Reference Count Method Algorithm and Mark and Sweep Algorithm (In C Language).

The heap implementation uses the First-Fit-Allocation Algorithm. It is implemented with the data structure doubly linked list. Contains two list for allocated head and free heap . Both gets updated whenever data is allocated or deallocated, so all the free blocks are merged together.

For the Garbage Collection: 

Reference Count Algorithm : In this whenever the heap is allocated the refernce count of the node it is pointing to is incremented by one and decremented whenever dellocated.
So the nodes with the refrence count zero are deallocated from the heap as they are garbage. The same is implemented here, the all refrences are stored in graph through adjacency list
and the reference count are judged respective to the directed graph nodes. So whenever the reference counter is zero that node is freed and the refernce count of all the nodes are updated and same repeats 
till no zero count node is present .Thus all the garbage are freed but the garbage with cycle refernce among themselves are not collected in this algorithm.

Mark and Sweep Algorithm : In this the same as reference count method the graph is implemented and for the garbage collection the nodes are checked if they are reachable
from the root node and if they are then they are marked with a mark bit set as one. And the nodes which are not reachable are market as zero as they are not reachable. 
Then the nodes which are not marked set one are freed and thus the storage is freed and this can work with the cycle nodes as if they are not reachable from root they will 
be marked zero. This is how this mark and sweep algorithm works and same is implemented here.
