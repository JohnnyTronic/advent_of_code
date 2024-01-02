#pragma once
#include<cstddef>

class Vec3 {
public:
    long long x{};
    long long y{};
    long long z{};

    Vec3(long long x = 0, long long y = 0, long long z = 0) : x(x), y(y), z(z) {}

    struct HashFunction {
        std::size_t operator()(const Vec3& v) const {
            return static_cast<std::size_t>(v.x) + (static_cast<std::size_t>(v.y) << 64) + (static_cast<std::size_t>(v.z) << 128);
        }
    };

    bool operator==(const Vec3& rhs) const {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    }
};

static Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

static Vec3 operator-(Vec3 lhs, Vec3 rhs) {
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

static bool operator!=(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

static Vec3 operator*(const Vec3& lhs, const double rhs) {
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

static Vec3 operator/(const Vec3& lhs, const double rhs) {
    return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

const Vec3 UP = Vec3(0, -1);
const Vec3 DOWN = Vec3(0, 1);
const Vec3 RIGHT = Vec3(1, 0);
const Vec3 LEFT = Vec3(-1, 0);
const Vec3 FORWARD = Vec3(0, 0, -1);
const Vec3 BACKWARD = Vec3(0, 0, 1);