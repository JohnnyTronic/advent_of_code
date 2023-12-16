#pragma once

class Vec2 {
public:
	int x{};
	int y{};

	Vec2(int x, int y) : x(x), y(y) {}
};

static Vec2 operator+(Vec2 lhs, Vec2 rhs) {
	return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}