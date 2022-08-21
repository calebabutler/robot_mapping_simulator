
// Includes
#include "data_types.h"

Vector2::Vector2(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2()
{
}

Vector2 Vector2::operator+(Vector2 other) const
{
    return Vector2(x + other.x, y + other.y);
}

bool Vector2::operator==(Vector2 other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(Vector2 other) const
{
    return !(*this == other);
}
