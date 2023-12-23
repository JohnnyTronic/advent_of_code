#pragma once

class Vec2 {
public:
    int x{};
    int y{};
    Vec2();
    Vec2(int x, int y) : x(x), y(y) {}

    //bool operator<(const Vec2& other) const { return x < other.x && y < other.y; };
};

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static Vec2 operator-(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

static bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

static bool operator!=(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

const Vec2 NORTH = Vec2(0, -1);
const Vec2 SOUTH = Vec2(0, 1);
const Vec2 EAST = Vec2(1, 0);
const Vec2 WEST = Vec2(-1, 0);