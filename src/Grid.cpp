#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Grid.h"


Grid::Grid(int rows, int cols, float cellSize, float offsetX, float offsetY) 
    : gridRows(rows), gridCols(cols), cellSize(cellSize), offsetX(offsetX), offsetY(offsetY),
      startX(-1), startY(-1), goalX(-1), goalY(-1) {
        for (int i = 0; i < gridRows; i++) {
        std::vector<Cell> row;
        for (int j = 0; j < gridCols; ++j) {
            Cell cell(i * cellSize + offsetX, j * cellSize + offsetY, cellSize);
            row.push_back(cell);
        }
        grid.push_back(row);
    }
    }

void Grid::handleMouseClick(int mousePositionX, int mousePositionY, char keyPressed) {
    int gridX = (mousePositionX - offsetX) / cellSize;
    int gridY = (mousePositionY - offsetY) / cellSize;

    if (gridX >= 0 && gridX < gridCols && gridY >= 0 && gridY < gridRows) {
        Cell &cell = grid[gridX][gridY];
        if (keyPressed == 's') {
            if (startX != -1 && startY != -1) {
                grid[startX][startY].updateState(CellStates::EMPTY);
            }
            startX = gridX;
            startY = gridY;
            cell.updateState(CellStates::START);
        }
        else if (keyPressed == 'g') {
            if (goalX != -1 && goalY != -1) {
                grid[goalX][goalY].updateState(CellStates::EMPTY);
            }
                goalX = gridX;
                goalY = gridY;
                cell.updateState(CellStates::GOAL);
            }
        else if (keyPressed == 'o') {
            if (cell.getState() == CellStates::OBSTACLE) {
                return;
            }
            if (cell.getState() == CellStates::START) {
                startX = -1;
                startY = -1;
            }
            if (cell.getState() == CellStates::GOAL) {
                goalX = -1;
                goalY = -1;
            }
            cell.updateState(CellStates::OBSTACLE);
        }
        else if (keyPressed == 'e') {
            if (cell.getState() == CellStates::START) {
                startX = -1;
                startY = -1;
            }
            if (cell.getState() == CellStates::GOAL) {
                goalX = -1;
                goalY = -1;
            }
            cell.updateState(CellStates::EMPTY);
        }
        else {
            std::cout << "Key pressed: " << keyPressed << " not associated with any command." << std::endl;
        }

    }
}

void Grid::draw(sf::RenderWindow &window) {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            window.draw(cell.rect);
        }
    }
}

void Grid::resetVisuals() {
    for (auto &row : grid) {
        for (auto &cell : row) {
            cell.visited = false;
            cell.parent = nullptr; 
            cell.gCost = std::numeric_limits<int>::max();  
            cell.hCost = 0;
            cell.fCost = std::numeric_limits<int>::max();  

            if (cell.getState() != CellStates::START && cell.getState() != CellStates::GOAL && cell.getState() != CellStates::OBSTACLE) {
                cell.updateState(CellStates::EMPTY);
            }
        }
    }
    while (!openSet.empty()) {
        openSet.pop();
    }

    while (!bfsQueue.empty()) {
        bfsQueue.pop();
    }

    while (!dfsStack.empty()) {
        dfsStack.pop();
    }

    discoveredCells.clear();
    closedSet.clear();
}

bool Grid::breadthFirstSearch() {
    if (startX == -1 || startY == -1 || goalX == -1 || goalY == -1) {
        std::cout << "There is no start and/or goal cell set" << std::endl;
        return false;
    }

    if (bfsQueue.empty()) {
        //resetVisuals();
        Cell* startCell = &grid[startX][startY];
        bfsQueue.push(startCell);
        startCell->visited = true;
    }
    
    if(!bfsQueue.empty()) {
        // Process a single cell
        Cell* current = bfsQueue.front();
        bfsQueue.pop();

        if (current->getState() != CellStates::START && current->getState() != CellStates::GOAL) {
            current->updateState(CellStates::VISITED);
        }

        if (current->getState() == CellStates::GOAL) {
            return true;
        }

        for (Cell* neighbour : getNeighbours(current, Algorithm::BFS)) {
            if (!neighbour->visited && neighbour->getState() != CellStates::OBSTACLE) {
                neighbour->visited = true;
                neighbour->parent = current;
                bfsQueue.push(neighbour);
            }
        }
    }
    return false;
}

std::vector<Cell*> Grid::getNeighbours(Cell* cell, Algorithm algo) {
    std::vector<Cell*> neighbors;
    int gridX = (cell->rect.getPosition().x - offsetX) / cellSize;
    int gridY = (cell->rect.getPosition().y - offsetY) / cellSize;

    if (gridY > 0) neighbors.push_back(&grid[gridX][gridY-1]);  // Up
    if (gridX > 0) neighbors.push_back(&grid[gridX-1][gridY]);  // Left
    if (gridX < gridCols-1) neighbors.push_back(&grid[gridX+1][gridY]);  // Right
    if (gridY < gridRows-1) neighbors.push_back(&grid[gridX][gridY+1]);  // Down

    if (algo == Algorithm::DFS) {
        std::reverse(neighbors.begin(), neighbors.end());
    } 
    
    return neighbors;
}

void Grid::showPath() {
    if (goalX == -1 || goalY == -1) {
        return;
    }
    Cell* current = &grid[goalX][goalY];
    while (current != nullptr && current->getState() != CellStates::START) {
        if (current->getState() != CellStates::GOAL) {
            current->updateState(CellStates::PATH);
        }
        current = current->parent;
    }
}
// Add a way to reset after 
bool Grid::depthFirstSearch() { 

    if (startX == -1 || startY == -1 || goalX == -1 || goalY == -1) {
        std::cout << "There is no start and/or goal cell set" << std::endl;
        return false;
    }

    if (dfsStack.empty()) {
        //resetVisuals();
        Cell* startCell = &grid[startX][startY];
        dfsStack.push(startCell);
    }
    
    if (!dfsStack.empty()) {
        Cell* current = dfsStack.top();
        dfsStack.pop();

        if (!current->visited) {
            current->visited = true;
            if (current->getState() != CellStates::START && current->getState() != CellStates::GOAL) {
                current->updateState(CellStates::VISITED);
            }

            if (current->getState() == CellStates::GOAL) {
                return true;
            }

            for (Cell* neighbour : getNeighbours(current, Algorithm::DFS)) {
                if (!neighbour->visited && neighbour->getState() != CellStates::OBSTACLE) {
                    neighbour->parent = current;
                    dfsStack.push(neighbour);
                }
            }
        }
    }
    
    return false;
}

int Grid::manhattanDistance(Cell* a, Cell* g) {
    return std::abs(a->rect.getPosition().x - g->rect.getPosition().x) + std::abs(a->rect.getPosition().y - g->rect.getPosition().y);
}

bool Grid::AStarSearch() {
    if (startX == -1 || startY == -1 || goalX == -1 || goalY == -1) {
        std::cout << "There is no start and/or goal cell set" << std::endl;
        return false;
    }

    if (openSet.empty()) {
        Cell* startCell = &grid[startX][startY];
        startCell->gCost = 0;
        startCell->hCost = manhattanDistance(startCell, &grid[goalX][goalY]);
        startCell->calculateFCost();
        openSet.push(startCell);
        discoveredCells.insert(startCell);
    }

    if (!openSet.empty()) {
        Cell* current = openSet.top();
        openSet.pop();
        discoveredCells.erase(current);

        if (current->getState() == CellStates::GOAL) {
            return true;
        }

        closedSet.push_back(current);
        if (current->getState() != CellStates::START && current->getState() != CellStates::GOAL) {
            current->updateState(CellStates::VISITED);
        }

        for (Cell* neighbour : getNeighbours(current, Algorithm::ASTAR)) {
            if (std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end() || 
                neighbour->getState() == CellStates::OBSTACLE) {
                    continue; // skip obstacle and proccessed cells
                }

            int tentativeGCost = current->gCost + 1;

            if (tentativeGCost < neighbour->gCost) {
                neighbour->parent = current;
                neighbour->gCost = tentativeGCost;
                neighbour->hCost = manhattanDistance(neighbour, &grid[goalX][goalY]);
                neighbour->calculateFCost();

                if (discoveredCells.find(neighbour) == discoveredCells.end()) {
                    openSet.push(neighbour);
                    discoveredCells.insert(neighbour);
                }
            }
        }
    }
    return false;
}

void Grid::clearGrid() {
    resetVisuals();
    for (auto &row: grid) {
        for (auto &cell: row) {
            cell.updateState(CellStates::EMPTY);
        }
    }
    startX = -1;
    startY = -1;
    goalX = -1;
    goalY = -1;
}