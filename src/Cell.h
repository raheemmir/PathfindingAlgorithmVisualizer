#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>

enum CellStates {EMPTY, START, GOAL, OBSTACLE, VISITED, PATH};

class Cell {
    public:
        Cell(float x, float y, float size);
        sf::RectangleShape rect;
        CellStates state;
        bool visited;
        Cell* parent;
        int gCost;
        int hCost;
        int fCost;

        void updateState(CellStates newState);
        CellStates getState();
        void draw(sf::RenderWindow &window);
        void calculateFCost();
};

#endif // CELL_H