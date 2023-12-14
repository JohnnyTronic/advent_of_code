#ifndef VEC2_CPP
#define VEC2_CPP

#include "Vec2.h"
#include <iostream>

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
	return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
	os << "(" << vec.x << "," << vec.y << ")";
	return os;
}

static bool operator==(Vec2& lhs, Vec2& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

static bool operator!=(Vec2& lhs, Vec2& rhs) {
	return lhs.x != rhs.x || lhs.y != rhs.y;
}
#endif