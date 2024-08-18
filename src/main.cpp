#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Cell.h"
#include "Grid.h"


int main()
{
    auto window = sf::RenderWindow{{1920u, 1080u}, "CMake SFML Project"};
    window.setFramerateLimit(60);

    // grid variables
    const int gridRows = 16;
    const int gridCols = 16;
    const float cellSize = 40.f; // casting as float

    // positioning the grid
    const float gridWidth = gridCols * cellSize;
    const float gridHeight = gridRows * cellSize;
    const float offsetX = (window.getSize().x - gridWidth) / 2.0f;
    const float offsetY = (window.getSize().y - gridHeight) / 2.0f;

    Grid grid(gridRows, gridCols, cellSize, offsetX, offsetY);

    // mouse text
    sf::Font font;
    if (!font.loadFromFile("/Users/raheem/Desktop/Code/Projects/C++/PathfindingAlgorithmVisualizer/Fonts/Roboto-Regular.ttf")) {
        return -1;
    }

    sf::Text mousePositionText;
    mousePositionText.setFont(font);
    mousePositionText.setCharacterSize(20);
    mousePositionText.setFillColor(sf::Color::White);
    mousePositionText.setPosition(10, 10);

    sf::Text lastClickedText;
    lastClickedText.setFont(font);
    lastClickedText.setCharacterSize(20);
    lastClickedText.setFillColor(sf::Color::White);
    lastClickedText.setPosition(10, 80); 

    // Instructions
    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(18);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(5, 70);
    instructionsText.setString(
        "Controls:\n\n"
        "Set a Start Cell: Left-click + S\n\n"
        "Set a Goal Cell: Left-click + G\n\n"
        "Set an Obstacle Cell: Left-click + O\n\n"
        "Set a cell as Empty: Left-click + E\n\n"
        "Run BFS: Shift + B\n\n"
        "Run DFS: Shift + D\n\n"
        "Run A* Search: Shift + A\n\n"
        "Reset/Clear Grid: Shift + R\n\n\n"
        "Legend:\n\n"
        "Green Cell: Start\n\n"
        "Red Cell: Goal\n\n"
        "Dark Blue Cells: Empty\n\n"
        "Bright Blue Cells: Visited\n\n"
        "Yellow: Path From Start to Goal"
    );

    bool algorithmRunning = false;
    Algorithm currentAlgo = Algorithm::NONE;
    sf::Clock clock;
    float searchStepTime = 0.1f; // 0.1 seconds between steps

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (!algorithmRunning && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // mouse positioning
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    // in terms of grid coords
                    int mouseGridX = (mousePosition.x - offsetX) / cellSize;
                    int mouseGridY = (mousePosition.y - offsetY) / cellSize;

                    if (mouseGridX >= 0 && mouseGridY < gridCols && mouseGridY >= 0 && mouseGridY < gridRows) {
                        char keyPressed = '\0';
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                            keyPressed = 's';
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                            keyPressed = 'g';
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                            keyPressed = 'o';
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                            keyPressed = 'e';
                        }
                        if (keyPressed != '\0') {
                            grid.handleMouseClick(mousePosition.x, mousePosition.y, keyPressed);
                        }
                        lastClickedText.setString("Last Clicked Position (Grid): (" + std::to_string(mouseGridX) + ", " + std::to_string(mouseGridY) + ")");
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                algorithmRunning = true;
                currentAlgo = Algorithm::BFS;
                grid.resetVisuals();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                algorithmRunning = true;
                currentAlgo = Algorithm::DFS;
                grid.resetVisuals();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                algorithmRunning = true;
                currentAlgo = Algorithm::ASTAR;
                grid.resetVisuals();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            grid.clearGrid();
        }

        if (algorithmRunning) {
            if (clock.getElapsedTime().asSeconds() >= searchStepTime) {
                bool searchFinished;
                switch(currentAlgo) {
                    case Algorithm::BFS:
                        searchFinished = grid.breadthFirstSearch();
                        break;
                    case Algorithm::DFS:
                        searchFinished = grid.depthFirstSearch(); 
                        break;
                    case Algorithm::ASTAR:
                        searchFinished = grid.AStarSearch();
                        break;
                    default:
                        break;
                }
                if (searchFinished) {
                    grid.showPath();
                    algorithmRunning = false;
                    
                }
                clock.restart();
            }
        }
        
        // mouse positioning
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        // in terms of grid coords
        int mouseGridX = (mousePosition.x - offsetX) / cellSize;
        int mouseGridY = (mousePosition.y - offsetY) / cellSize;
        mousePositionText.setString("Mouse Position (Win): (" + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y) + ")" + 
        "\n" + "Mouse Position (Grid): (" + std::to_string(mouseGridX) + ", " + std::to_string(mouseGridY) + ")");
        window.clear();

        grid.draw(window);
        window.draw(instructionsText);
        window.display();
    }
}
