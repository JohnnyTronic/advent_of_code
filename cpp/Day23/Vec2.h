#pragma once

#include<cstddef>

class Vec2 {
public:
    long long x{};
    long long y{};

    Vec2(long long x = 0, long long y = 0) : x(x), y(y) {}

    std::size_t Hash() const {
        return x + (y << 16);
    }
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

static Vec2 operator*(const Vec2& lhs, const int rhs) {
    return Vec2(lhs.x * rhs, lhs.y * rhs);
}

const Vec2 NORTH = Vec2(0, -1);
const Vec2 SOUTH = Vec2(0, 1);
const Vec2 EAST = Vec2(1, 0);
const Vec2 WEST = Vec2(-1, 0);