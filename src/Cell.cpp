#include "Cell.h"

Cell::Cell(float x, float y, float size) {
    rect.setSize(sf::Vector2f(size-2, size-2));
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(sf::Color(0, 0, 128));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(1.0f);
    state = EMPTY;
    visited = false;
    parent = nullptr;
    int gCost = 0;
    int hCost = 0;
    calculateFCost();
}

void Cell::updateState(CellStates newState) {
    state = newState;
    switch (state) {
        case START:
            rect.setFillColor(sf::Color::Green);
            break;
        case GOAL:
            rect.setFillColor(sf::Color::Red);
            break;
        case OBSTACLE:
            rect.setFillColor(sf::Color::White);
            break;
        case EMPTY:
            rect.setFillColor(sf::Color(0, 0, 128)); // Dark blue
            break;
        case VISITED: 
            rect.setFillColor(sf::Color::Blue);  // The Cell has been explored (popped from stack, dequeued, or in closed set)
            break;
        case PATH:
            rect.setFillColor(sf::Color::Yellow);
            break;
        default:
            break;
    }
}

CellStates Cell::getState() {
    return state;
}

void Cell::draw(sf::RenderWindow &window) {
    window.draw(rect);
}

void Cell::calculateFCost() {
    fCost = gCost + hCost;
}