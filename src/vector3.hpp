#pragma once

#include <iostream>
#include <cmath>

// ==================== 3D Vector ====================
struct Vec3 {
    double x, y, z;

    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(double s) const { return Vec3(x / s, y / s, z / s); }

    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    double length() const { return std::sqrt(x * x + y * y + z * z); }
    double lengthSquared() const { return x * x + y * y + z * z; }

    Vec3 normalized() const {
        double len = length();
        return len > 0 ? *this / len : Vec3(0, 0, 0);
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};
