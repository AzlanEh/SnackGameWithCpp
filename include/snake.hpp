#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <utility> // For std::pair

class Snake
{
public:
    Snake(int startX, int startY);

    void move(int directionX, int directionY);
    void grow();
    bool checkCollision(int gridWidth, int gridHeight) const;
    std::vector<std::pair<int, int>> getBody() const;

private:
    std::vector<std::pair<int, int>> body; // Vector of body segments (pairs of coordinates)
    int directionX, directionY;            // Movement direction (x and y)
    bool hasGrown = false;
};

#endif
