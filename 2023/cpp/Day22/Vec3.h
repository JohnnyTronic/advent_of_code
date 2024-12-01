#pragma once

#include <cstddef>

struct Vec3 {
public:
    int x;
    int y;
    int z;

    Vec3(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    std::size_t Hash() {
        return x + (y << 8) + (z << 16);
    }
};

static Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

static Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

static bool operator==(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

static bool operator!=(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

static Vec3 operator*(const Vec3& lhs, const int rhs) {
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

static Vec3 operator*(const Vec3& lhs, const double rhs) {
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}