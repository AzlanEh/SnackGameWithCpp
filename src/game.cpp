#include "game.hpp"
#include "snake.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include <chrono> // For std::chrono::milliseconds
#include <thread> // For std::this_thread::sleep_for

Game::Game() : isRunning(true), snake(10, 10)
{
    srand(static_cast<unsigned>(time(0)));
    // srand(time(0));
    generateFood();

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Non-blocking input
    curs_set(0);           // Hide the cursor
}

void Game::run()
{
    // std::cout << "Entering game loop..." << std::endl;

    const auto frameDuration = std::chrono::milliseconds(1000 / 2);

    const auto snakeSpeed = std::chrono::milliseconds(500);

    auto lastMoveTime = std::chrono::high_resolution_clock::now();

    while (isRunning)
    {
        // std::cout << "Inside game loop... (isGameOver = " << isRunning << ")" << std::endl;

        auto frameStart = std::chrono::high_resolution_clock::now();

        processInput();

        auto now = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime);

        if (elapsedTime >= snakeSpeed)
        {
            update();           // Move the snake
            lastMoveTime = now; // Reset the last move time
        }
        render();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (frameTime < frameDuration)
        {
            std::this_thread::sleep_for(frameDuration - frameTime); // Sleep for the remaining time to cap FPS
        }
    }
    // std::cout << "Game Over!" << std::endl;
}

void Game::processInput()
{
    int ch = wgetch(stdscr); // Get the pressed key

    switch (ch)
    {
    case KEY_UP:
        if (directionY == 0)
        { // Prevent reversing direction
            directionX = 0;
            directionY = -1;
        }
        break;
    case KEY_DOWN:
        if (directionY == 0)
        {
            directionX = 0;
            directionY = 1;
        }
        break;
    case KEY_LEFT:
        if (directionX == 0)
        {
            directionX = -1;
            directionY = 0;
        }
        break;
    case KEY_RIGHT:
        if (directionX == 0)
        {
            directionX = 1;
            directionY = 0;
        }
        break;
    case 'q': // Quit the game
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    snake.move(directionX, directionY);

    auto head = snake.getBody().front();

    // std::cout << "Head position: (" << head.first << ", " << head.second << ")\n";
    // std::cout << "Food position: (" << food.first << ", " << food.second << ")\n";

    if (head.first < 0 || head.first >= gridWidth || head.second < 0 || head.second >= gridHeight)
    {
        // std::cout << "Game Over! You hit the boundary.\n";
        isRunning = false;
        return;
    }

    if (snake.getBody().front() == food)
    {
        snake.grow();
        score += 5;
        generateFood();
    }

    if (snake.checkCollision(gridWidth, gridHeight))
    {
        // std::cout << "Game Over! You collided with yourself.\n";
        isRunning = false;
    }

    // std::cout << "Snake size: " << snake.getBody().size() << "\n";
}

void Game::render()
{
    clear(); // Clears the screen

    // Loop through the grid to render the snake, food, and empty spaces
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            if (std::make_pair(x, y) == snake.getBody().front())
            {
                ~mvprintw(y, x, "O"); // Snake's head
            }
            else if (std::find(snake.getBody().begin() + 1, snake.getBody().end(), std::make_pair(x, y)) == snake.getBody().begin())
            {
                mvprintw(y, x, "#"); // Snake's body
            }
            else if (std::make_pair(x, y) == food)
            {
                mvprintw(y, x, "F"); // Food
            }
            else
            {
                mvprintw(y, x, "."); // Empty space
            }
        }
    }

    // Make sure score is printed within the screen bounds
    if (gridHeight < LINES - 1) // Check if the gridHeight doesn't overflow the terminal screen
    {
        mvprintw(gridHeight, 0, "Score: %d", score);
    }
    else
    {
        mvprintw(LINES - 1, 0, "Score: %d", score); // Print score at the bottom of the screen
    }

    refresh(); // Refresh to update the display
}

void Game::generateFood()
{
    int x, y;
    do
    {
        x = rand() % gridWidth;
        y = rand() % gridHeight;
    } while ([&]()
             {
        // Ensure food doesn't spawn on the snake's body
        for (const auto& segment : snake.getBody()) {
            if (segment == std::make_pair(x, y)) return true;
        }
        return false; }());

    food = {x, y};
    std::cout << "Food generated at: (" << food.first << ", " << food.second << ")\n";
}

Game::~Game()
{
    endwin(); // Clean up ncurses
}
