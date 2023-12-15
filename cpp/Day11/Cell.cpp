#ifndef CELL_CPP
#define CELL_CPP

#include "Vec2.h"

class Cell {
public:
    char content;
    Vec2 position;

    Cell(char content) : content(content), position(Vec2(0,0)) {}
};

#endif