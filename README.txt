Knight’s Path Challenge
————————————————————————

HOW TO RUN:
———————————

1. Modify main.cpp: replace “ENTER_PATH_TO_MAP_TXT_FILE” with the path to a map.
2. Compile.
3. Run and follow on-screen instructions.
4. Enjoy!


Solution Explanation:
——————————————————————

(LEVEL 3)
—> Shortest Path from Node A to Node B in a simple chess board:
	-> Construct adjacency matrix of the unweighted cyclic graph of n*n = 64 nodes.
	-> Apply Dijkstra algorithm (could have also used DFS) from Start to end

(LEVEL 4)
-> Still a shortest path. However, now adjacency matrix construction is more complex since we are dealing with a *weighted* cyclic graph of n*n = 1024 nodes with some special features (teleport, barrier etc).
-> Once we have a proper adjacency matrix, can also apply Dijkstra for shortest path

(LEVEL 5)
-> This is an NP-Complete problem since its a reduction from a hamiltonian problem. Not sure if it can be considered exclusively NP-Hard. It appeared that brute-force was the only option. However, I came up with the following solution: Finding the longest path in graph G resolves to finding the shortest path in -G.
-> Negate the adjacency matrix (Keeping the HUGE-9999 positive)
-> Re-apply Dijkstra’s algorithm.
-> Enjoy watching the knight’s path