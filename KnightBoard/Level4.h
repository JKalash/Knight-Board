//
//  BoardTile.hpp
//  KnightBoard
//
//  Created by Joseph Kalash on 3/12/16.
//  Copyright © 2016 Joseph Kalash. All rights reserved.
//

#ifndef BoardTile_h
#define BoardTile_h

#include <stdio.h>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "Level3.h"

typedef enum : int {
    Regular,
    Water,
    Barrier,
    Rock,
    Lava,
    Teleport
} Type;

class BoardTile {
    Type t;
    Position p = Position(0, 0);
public:
    BoardTile(char type, int row, int col) {
        switch (type) {
            case 'R':
                this->t = Rock;
                break;
            case 'W':
                this->t = Water;
                break;
            case 'B':
                this->t = Barrier;
                break;
            case 'L':
                this->t = Lava;
                break;
            case 'T':
                this->t = Teleport;
                break;
            default:
                this->t = Regular;
                break;
        }
        
        this->p.col = col;
        this->p.row = row;
    }
    
    Type getType() {return this->t;}
    Position getPosition() {return this->p;}
    friend bool operator== (const BoardTile& lhs, const BoardTile& rhs){
        return lhs.p.row == rhs.p.row && lhs.p.col == rhs.p.col;
    }
};

vector<vector<BoardTile>> readMapFromFile(string fileName) {
    
    vector<vector<BoardTile>> board;
    fstream infile(fileName);
    char ch;
    int row = 0, col = 0;
    while(infile >> noskipws >> ch) {
        if(ch == '\n') {
            row++;
            col = 0;
            continue;
        }
        
        if(board.size() <= row) {
            //Create new row
            vector<BoardTile> row;
            board.push_back(row);
        }
        
        BoardTile b = BoardTile(ch, row, col);
        board[row].push_back(b);
        
        col++;
    }
    
    if(board.size() == 0) {
        cout << "Invalid Map file path. Make sure correct path is set in main.cpp\n";
        exit(-1);
    }
    
    return board;
}

//Checks if there's a barrier preventing a jump from a given start to end
bool barrierBetween(Position start, Position end, vector<vector<BoardTile>> board) {
    
    //We need to check the two tiles lying between start and end.
    //If EITHER is barrier, then there's a barrier in between.
    
    int dX = end.col - start.col;
    int dY = end.row - start.row;
    
    //Get the X & Y of the two blocks of interest based on the delta of Col and row
    int block1DX, block1DY, block2DX, block2DY;
    if(dX == 1 && dY == -2)
        block1DX = 0, block1DY = -1,block2DX = 1, block2DY = -1;
    else if(dX == 2 && dY == -1)
        block1DX = 1, block1DY = -1,block2DX = 1, block2DY = 0;
    else if(dX == 2 && dY == 1)
        block1DX = 1, block1DY = 0,block2DX = 1, block2DY = 1;
    else if(dX == 1 && dY == 2)
        block1DX = 1, block1DY = 1,block2DX = 0, block2DY = 1;
    else if(dX == -1 && dY == 2)
        block1DX = 0, block1DY = 1,block2DX = -1, block2DY = 1;
    else if(dX == -2 && dY == 1)
        block1DX = -1, block1DY = 1,block2DX = -1, block2DY = 0;
    else if(dX == -2 && dY == -1)
        block1DX = -1, block1DY = 0,block2DX = -1, block2DY = -1;
    else if(dX == -1 && dY == -2)
        block1DX = -1, block1DY = -1,block2DX = 0, block2DY = -1;
    
    Position block1Pos = Position(start.col + block1DX, start.row + block1DY);
    Position block2Pos = Position(start.col + block2DX, start.row + block2DY);
    
    BoardTile block1 = board[block1Pos.row][block1Pos.col];
    BoardTile block2 = board[block2Pos.row][block2Pos.col];
    
    if(block1.getType() == Barrier || block2.getType() == Barrier)
        return true;
    
    return false;
}

vector<vector<int>> adjacencyMatrixFromBoardMap(vector<vector<BoardTile>> board) {
    
    //The construction of the adjacency matrix follows this logic
    // Step 1: Check if there's a barrier between node a and b
    //          -> Barrier? -> No link (Cost = HUGE)
    //          -> No Barrier? -> Check node b type -> Regular or Teleport -> Cost = 1
    //                                              -> Water -> Cost = 2
    //                                              -> Rock -> Cost = HUGE
    //                                              -> Lava -> Cost = 5
    // Step 2: Correct adjacencies of teleport *source* nodes.
    
    int boardSize = (int)board.size();
    vector<vector<int>> adjacency(boardSize*boardSize, vector<int>(boardSize*boardSize, 0));
    
    vector<BoardTile> teleportTiles;
    
    for(int row = 0; row < boardSize*boardSize; ++row) {
        for(int column = 0; column < boardSize*boardSize; column++) {
            
            //No need to check if node to itself
            if(row == column) {
                adjacency[row][column] = HUGE;
                continue;
            }
            
            //Make sure legal knight jump between to nodes
            Position node1 = positionFromID(row, boardSize);
            Position node2 = positionFromID(column, boardSize);
            
            //If source is a barrier or a rock, Don't bother
            BoardTile source = board[node1.row][node1.col];
            if(source.getType() == Barrier || source.getType() == Rock) {
                adjacency[row][column] = HUGE;
                continue;
            }
            
            //If teleport, add to our array of teleport tiles
            if(board[node2.row][node2.col].getType() == Teleport) {
                if(find(teleportTiles.begin(), teleportTiles.end(), board[node2.row][node2.col]) == teleportTiles.end())
                    teleportTiles.push_back(board[node2.row][node2.col]);
            }
            
            int deltaColumn = abs(node1.col - node2.col);
            int deltaRow = abs(node1.row - node2.row);
            
            if(!(deltaColumn==1 && deltaRow==2) && !(deltaColumn==2 && deltaRow==1)) {
                //Not a valid knight jump
                adjacency[row][column] = HUGE;
                continue;
            }
            
            //Step 1
            if(barrierBetween(node1, node2, board)) {
                adjacency[row][column] = HUGE;
                continue;
            }
            //No barrier, proceed
            switch(board[node2.row][node2.col].getType()) {
                case Regular:
                case Teleport:
                    adjacency[row][column] = 1;
                    break;
                case Water:
                    adjacency[row][column] = 2;
                    break;
                case Rock:
                case Barrier:
                    adjacency[row][column] = HUGE;
                    break;
                case Lava:
                    adjacency[row][column] = 5;
                    break;
                default: break;
            }
        }
    }
    
    //Fix for teleport tiles
    //1. Get a list of all teleport tiles
    //2. Let V1, V2 .. Vn be the set of vertices that Teleport tile N can reach.
    //3. To fix, each vertex's set of vertices it can reach becomes the union of all N sets.
    vector<set<pair<int, int>>> adjacencySets;
    for(int i = 0; i < teleportTiles.size(); i++) {
        //Keep a set of pair <Destination Node, Cost> to all nodes we can jump to
        //From node i which is a teleport node.
        int teleportNodeID = boardSize*teleportTiles[i].getPosition().row + teleportTiles[i].getPosition().col;
        set<pair<int, int>> adjacencySet;
        for(int j = 0; j < boardSize*boardSize; j++)
            if(adjacency[teleportNodeID][j] != HUGE) {
                pair<int, int> pair(j, adjacency[teleportNodeID][j]);
                adjacencySet.insert(pair);
            }
        adjacencySets.push_back(adjacencySet);
    }
    
    //Now compute the union of all sets in the array
    set<pair<int, int>> unionSet;
    for(int i = 0; i < adjacencySets.size(); i++)
        unionSet.insert(adjacencySets[i].begin(), adjacencySets[i].end());
    
    //Update the adjacency matrix
    //For each row whose ID is that of a teleport node,
    //update each column in the unionSet to match the updated cost
    for(int i = 0; i < teleportTiles.size(); i++) {
        int teleportNodeID = boardSize*teleportTiles[i].getPosition().row + teleportTiles[i].getPosition().col;
        for(set<pair<int, int>>::iterator it = unionSet.begin(); it != unionSet.end(); ++it) {
            pair<int, int> pair = *it;
            adjacency[teleportNodeID][pair.first] = pair.second;
        }
    }
    return adjacency;
}

//Prints a snapshot of the board
//Given the knight's position and a desired start and end
void printBoard(vector<vector<BoardTile>> board, Position start, Position end, Position current) {
    for(int row = 0; row < board.size(); row++) {
        for(int col = 0; col < board.size(); col++) {
            //Print by priority: 1. Start/end
            //                   2. Current
            //                   3. Board Tile
            if(current.row == row && current.col == col)
                cout << "K";
            else if(start.row == row && start.col == col)
                cout << "S";
            else if(end.row == row && end.col == col)
                cout << "E";
            else {
                switch(board[row][col].getType()) {
                    case Regular: cout << "."; break;
                    case Lava: cout << "L"; break;
                    case Rock: cout << "R"; break;
                    case Water: cout << "W"; break;
                    case Barrier: cout << "B"; break;
                    case Teleport: cout << "T"; break;
                    default: break;
                }
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void simulateLevel4(string filePath) {
    //Read the board from the text file
    vector<vector<BoardTile>> board =  readMapFromFile(filePath);
    
    //The change in this level is that the weights of the edges are no longer all equal to 1.
    //Some edges cost more than others and for that,
    //Problem remains in being able to properly represent the graph using an adjacency matrix
    vector<vector<int>> adjacency = adjacencyMatrixFromBoardMap(board);
    
    cout << "**Level 4**\n";
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
    
    //Given this adjacency, we can apply Dijkstra's algorithm for finding the shortest path from Tile A to Tile B
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

#endif /* BoardTile_hpp */
