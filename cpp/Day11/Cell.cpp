#ifndef CELL_CPP
#define CELL_CPP

#include "Vec2.h"

class Cell {
public:
    char content;
    Vec2 position;

    Cell(char content, Vec2 position) : content(content), position(position) {}
};

#endif