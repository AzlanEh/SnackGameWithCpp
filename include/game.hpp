#ifndef GAME_H
#define GAME_H

#include "snake.hpp"
#include <utility>

class Game
{
public:
    Game();
    ~Game();
    void run(); // Main game loop
private:
    void processInput();
    void update();
    void render();
    void generateFood();

    bool isRunning; 
    Snake snake;
    std::pair<int, int> food;
    const int gridWidth = 20;
    const int gridHeight = 20;

    int directionX = 1; // Snake's current horizontal direction
    int directionY = 0; // Snake's current vertical direction
    int score = 0;
};

#endif
