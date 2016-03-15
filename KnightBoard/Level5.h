//
//  Level5.h
//  KnightBoard
//
//  Created by Joseph Kalash on 3/13/16.
//  Copyright © 2016 Joseph Kalash. All rights reserved.
//


/**Longest uncrossed knight's path problem*
This is an NP Hard problem, since there is a simple reduction
from a hamiltonian path to this problem. 
 Best solution is to brute-force*/

//We are interested in finding the longest path taken by the knight from (X,Y) to (X',Y') such that he visits every square exactly once.
//Solution: Finding Longest Path in G resolves to finding the shortest path in -G
//-->Make all weights negative and running Dijkstra.


#ifndef Level5_h
#define Level5_h

#include "Level3.h"
#include "Level4.h"

void simulateLevel5(string filePath) {
    
    //Read the board from the text file
    vector<vector<BoardTile>> board =  readMapFromFile(filePath);
    
    //The change in this level is that the weights of the edges are no longer all equal to 1.
    //Some edges cost more than others and for that,
    //Problem remains in being able to properly represent the graph using an adjacency matrix
    vector<vector<int>> adjacency = adjacencyMatrixFromBoardMap(board);
    
    //make the matrix negative
    for(int row = 0; row < adjacency.size(); row++)
        for(int col = 0; col < adjacency.size(); col++)
            if(adjacency[row][col] != HUGE)
                adjacency[row][col] *= -1;
    
    cout << "**Level 5**\n";
    cout << "Enter Start X & Y:";
    int startX, startY;
    cin >> startX >> startY;
    Position start = Position(startX, startY);
    cout << "Enter End X & Y: ";
    int endX, endY;
    cin >> endX >> endY;
    Position end = Position(endX, endY);
    
    if(!isLegalPosition(start, (int)board.size()) || !isLegalPosition(end, (int)board.size())) {
        cout << "Illegal start or end position\n";
        return;
    }
    
    //Make sure Start & End are not Rock or Barrier
    if(board[start.col][start.row].getType() == Rock ||
       board[start.col][start.row].getType() == Barrier ||
       board[end.col][end.row].getType() == Rock ||
       board[end.col][end.row].getType() == Barrier) {
        cout << "Cannot have Barrier or Rock as Start or End\n";
        return;
    }
    
    int startID = (int)board.size()*start.row + start.col;
    int endID = (int)board.size()*end.row + end.col;
    
    //Given this adjacency, we can apply Dijkstra's algorithm for finding the longest path in G
    //By applying shortest path in -G
    vector<int> rawPath =  dijkstra(adjacency, startID, endID);
    
    //Convert path of ints to a path of Positions
    vector<Position> path;
    for(int i = 0; i < rawPath.size(); ++i) {
        int row = rawPath[i] / board.size();
        int col = rawPath[i] % board.size();
        path.push_back(Position(col, row));
    }
    
    std::cin.ignore();
    
    //Print the path from start to end
    for(int i = 0 ; i < path.size(); i++) {
        system("clear");
        cout << "Step " << i << ":\n";
        printBoard(board, start, end, path[i]);
        if(i != path.size()-1) {
            cout << "Press enter for next step\n";
            std::cin.ignore();
        }
    }
    
}


#endif /* Level5_h */
