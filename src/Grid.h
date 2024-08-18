#ifndef GRID_H
#define GRID_H
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_set>
#include <string>
#include "Cell.h"

enum Algorithm {BFS, DFS, ASTAR, NONE};


class Grid {
    public:
        int gridRows;
        int gridCols;
        float cellSize;
        float offsetX;
        float offsetY;
        int startX;
        int startY;
        int goalX;
        int goalY;

        Grid(int rows, int cols, float size, float offsetX, float offsetY);
        void handleMouseClick(int mousePositionX, int mousePositionY, char keyPressed);
        void resetVisuals();
        void clearGrid();
        bool breadthFirstSearch();
        bool depthFirstSearch();
        bool AStarSearch();
        std::vector<Cell*> getNeighbours(Cell* cell, Algorithm algo);

        std::vector<std::vector<Cell>> grid;
        std::queue<Cell*> bfsQueue;
        std::stack<Cell*> dfsStack;
        
        struct CompareCells {
            bool operator()(Cell* a, Cell* b) {
                return a->fCost > b->fCost;
            }
        };

        std::priority_queue<Cell*, std::vector<Cell*>, CompareCells> openSet;
        std::unordered_set<Cell*> discoveredCells; // for searching the openSet

        std::vector<Cell*> closedSet;

        void draw(sf::RenderWindow &window);
        void showPath();
        int manhattanDistance(Cell* a, Cell* g);
};

#endif //GRID_H