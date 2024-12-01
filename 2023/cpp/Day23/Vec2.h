#pragma once

#include<cstddef>

class Vec2 {
public:
    short x{};
    short y{};

    Vec2(short x = 0, short y = 0) : x(x), y(y) {}
        
    struct HashFunction {
        std::size_t operator()(const Vec2& v) const {
            return static_cast<std::size_t>(v.x) + (static_cast<std::size_t>(v.y) << 16);
        }
    };

    bool operator==(const Vec2& rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }
};

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static Vec2 operator-(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
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