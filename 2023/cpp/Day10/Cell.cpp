#ifndef CELL_H
#define CELL_H

#include "Vec2.h"

class Cell {
public:
    Vec2 position;
    char content;
    bool isMainLoop;

    Cell(Vec2 position, char content, bool isMainLoop) : position(position), content(content), isMainLoop(isMainLoop) {}
};

#endif