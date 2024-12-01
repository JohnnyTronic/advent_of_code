#pragma once

class Vec2{
public:
    int x;
    int y;
    Vec2(int x, int y) : x(x), y(y) {};
};

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}