#include "snake.hpp"
#include <iostream>

Snake::Snake(int startX, int startY)
{
    body.push_back({startX, startY});
    body.push_back({startX, startY});     // Head segment
    body.push_back({startX - 1, startY}); // Second segment (tail)
    directionX = 1;                       // Initial direction (right)
    directionY = 0;                       // No vertical movement
    hasGrown = false;                     // Initialize growth flag
}

void Snake::move(int dirX, int dirY)
{
    // Calculate new head position
    int newX = body.front().first + dirX;
    int newY = body.front().second + dirY;

    // Add the new head to the body
    body.insert(body.begin(), {newX, newY});

    // Remove the tail if the snake hasn't grown
    if (!hasGrown)
    {
        body.pop_back();
    }
    else
    {
        hasGrown = false; // Reset growth flag
    }

    // Debugging: Log body after moving
    // std::cout << "Snake moved. New size: " << body.size() << "\n";
    for (const auto &segment : body)
    {
        std::cout << "(" << segment.first << ", " << segment.second << ") ";
    }
    std::cout << "\n";
}

void Snake::grow()
{
    // Add a new segment at the tail position
    hasGrown = true; // Set the growth flag
    // std::cout << "Snake growing! Current size: " << body.size() << "\n";
}

bool Snake::checkCollision(int gridWidth, int gridHeight) const
{
    // Check if the snake collides with itself
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (body[i] == body.front())
        {
            return true; // Snake collides with itself
        }
    }
    // Check boundary collision
    int headX = body.front().first;
    int headY = body.front().second;

    if (headX < 0 || headX >= gridWidth || headY < 0 || headY >= gridHeight)
    {
        return true;
    }

    return false;
}

std::vector<std::pair<int, int>> Snake::getBody() const
{
    return body;
}
