
// Begin header guard
#ifndef DATA_TYPES_H
#define DATA_TYPES_H

// Includes
#include <vector>

class Vector2 {
public:
    // Data members
    int x;
    int y;
    // Member functions
    Vector2(int, int);
    Vector2();
    Vector2 operator+(Vector2 other) const;
    bool operator==(Vector2 other) const;
    bool operator!=(Vector2 other) const;
};

// End header guard
#endif
