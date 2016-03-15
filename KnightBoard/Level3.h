//
//  Dijkstra.cpp
//  KnightBoard
//
//  Created by Joseph Kalash on 3/12/16.
//  Copyright © 2016 Joseph Kalash. All rights reserved.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>

#define HUGE 9999999
#define BOARD_SIZE 8

using namespace std;

struct Position {
    int row;
    int col;
    Position(int col, int row) {
        this->row = row;
        this->col = col;
    }
};

Position positionFromID(const int id, const int boardSize) {
    int row = id / boardSize;
    int col = id % boardSize;
    return Position(col, row);
}

//Prints a snapshot of the board
//Given the knight at position 'current', and a start and end position
void printBoard(Position current, Position start, Position end) {
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int column = 0; column < BOARD_SIZE; column++) {
            if(current.col == column && current.row == row)
                cout << "K";
            else if(row == start.row && column == start.col)
                cout << "S";
            else if(row == end.row && column == end.col)
                cout << "E";
            else cout << ".";
        }
        cout << "\n";
    }
    
    cout << "\n";
}

//Checks if p is a legal board position
bool isLegalPosition(Position p, int boardSize) {
    return p.row >= 0 && p.row < boardSize && p.col >= 0 && p.col < boardSize;
}

//Checks if a knight jump is a legal one
bool validKnightJump(Position from, Position to) {
    if(!isLegalPosition(from, BOARD_SIZE) || !isLegalPosition(to, BOARD_SIZE))
        return false;
    
    //Valid Jump when dRow is +-1 and dCol is +-2 or vice versa
    int dRow = abs(from.row-to.row);
    int dCol = abs(from.col-to.col);
    
    return (dRow == 1 && dCol == 2) || (dRow ==2 && dCol == 1);
}

//LEVEL 1 - Checking if a sequence of knight jumps is valid
// OPTIONAL - Also prints a snapshot of the board at every step
bool validKnightSequence(vector<Position> sequence) {
    
    //Go over array sequentially making sure each move by itself is valid
    for(int i = 0; i < sequence.size()-1; i++) {
        if(!validKnightJump(sequence[i], sequence[i+1]))
            return false;
    }
    
    for(int i = 0; i < sequence.size(); i++)
        printBoard(sequence[i], sequence[0], sequence[sequence.size()-1]);
    
    return true;
}

vector<vector<int>> buildAdjacency(const int boardSize) {
    
    const int nodeCount = boardSize*boardSize;
    
    //Let nodeid = 8*(row) + column
    //Two nodes are adjacent if delX = +- 2 and delY = +-1 or vice versa
    vector<vector<int>> adjacency(nodeCount, vector<int>(nodeCount, HUGE));
    for(int row = 0; row < adjacency.size(); ++row)
        for(int column = 0; column < adjacency.size(); ++column) {
            
            if(row == column)
                continue;
            
            Position node1 = positionFromID(row, boardSize);
            Position node2 = positionFromID(column, boardSize);
            
            int deltaColumn = abs(node1.col - node2.col);
            int deltaRow = abs(node1.row - node2.row);
            
            if((deltaColumn==1 && deltaRow==2) || (deltaColumn==2 && deltaRow==1))
                adjacency[row][column] = 1;
        }
    
    return adjacency;
}

vector<int> dijkstra(vector<vector<int>> adjacencyMatrix, int start, int destination) {
    
    //In this problem, the cost matrix is simply the adjacency matrix as costs are all equal in de graph (vertices weights = 1)
    
    //Numer of nodes is simply the num of rows (or cols) of our adjacency matrix
    int nodeCount = (int)adjacencyMatrix.size();
    
    //pred[] stores the predecessor of each node. Init all to the start node
    //Visited array to keep track of which nodes we already visited
    vector<int> pred(nodeCount, start);
    vector<bool> visited(nodeCount, false);
    
    //Build a Cost matrix from the adjacency. 0 in adjacency is mapped to inifinite and 1 (linked) to a cost of 1
    vector<vector<int>> cost(nodeCount, vector<int>(nodeCount, HUGE));
    for(int i = 0; i < nodeCount; i++) {
        for(int j = 0; j < nodeCount; j++)
                cost[i][j] = adjacencyMatrix[i][j];
    }
    
    //Distance holds the distance from the start node to each other node
    vector<int> distance(nodeCount, 0);
    for(int i = 0; i < nodeCount; ++i)
            distance[i] = cost[start][i];

    //Start with the start node
    distance[start] = 0;
    visited[start] = 1;
    //count gives the number of nodes seen so far
    int visitedCount = 1;
    
    while(visitedCount < nodeCount) {
        int minDistance = HUGE;
        int nextnode = 0;
        //Loop over all unvisited nodes to find the one with minimum distance
        for(int i=0; i<nodeCount; ++i) {
            if(distance[i] < minDistance && !visited[i]) {
                minDistance = distance[i];
                nextnode = i;
            }
        }
        
        //Check if a better path exists through nextnode
        visited[nextnode] = 1;
        for(int i =0; i < nodeCount; ++i)
            if(!visited[i])
                if(minDistance+cost[nextnode][i]<distance[i]) {
                    distance[i] = minDistance+cost[nextnode][i];
                    pred[i] = nextnode;
                }
        
        visitedCount++;
    }
    
    vector<int>path;
    path.push_back(destination);
    //Compute the path to destination
    do {
        destination = pred[destination];
        path.push_back(destination);
    }while(destination != start);
    
    //Now reverse the path to have it go from start to destination
    reverse(path.begin(), path.end());
    
    return path;
}

//LEVEL 2 & 3 - Computing the shortest distance from start to end
// Shortest Distance Problem -> Can use Dijkstra's Algorithm
// Note that since all paths in this level have the same weight, a BFS search would have also worked.
vector<Position> shortestDistance(Position start, Position end) {
    
    ////Start by creating a graph of 64 nodes with edges from any node to another where
    //The knight can perform a legal move
    //An adjacency matrix needs to be constructed.
    vector<vector<int>> adjacency = buildAdjacency(BOARD_SIZE);
    
    //Now use this adjacency with Dijkstra's algorithm
    int startID = BOARD_SIZE*start.row + start.col;
    int endID = BOARD_SIZE*end.row + end.col;
    
    vector<int> rawPath =  dijkstra(adjacency, startID, endID);
    
    //Convert path of ints to a path of Positions
    vector<Position> path;
    for(int i = 0; i < rawPath.size(); ++i) {
        int row = rawPath[i] / BOARD_SIZE;
        int col = rawPath[i] % BOARD_SIZE;
        path.push_back(Position(col, row));
    }
    
    return path;
}

void simulateLevels1To3() {
    cout << "**Level 1 - 2 - 3**\n";
    cout << "Enter Start X & Y: ";
    int startX, startY;
    cin >> startX >> startY;
    Position start = Position(startX, startY);
    cout << "Enter Destination X & Y: ";
    int endX, endY;
    cin >> endX >> endY;
    Position end = Position(endX, endY);
    
    if(!isLegalPosition(start, BOARD_SIZE) || !isLegalPosition(end, BOARD_SIZE)) {
        cout << "Illegal start or end position\n";
        return;
    }
    
    vector<Position> path = shortestDistance(start, end);
    
    std::cin.ignore();
    
    //Print the path from start to end
    for(int i = 0 ; i < path.size(); i++) {
        system("clear");
        cout << "Step " << i << ":\n";
        printBoard(path[i], start, end);
        if(i != path.size()-1) {
            cout << "Press enter for next step\n";
            std::cin.ignore();
        }
    }
}


#endif