#pragma once

#include <cmath>

struct Vec3 {
    float x, y, z;

    Vec3(const float x, const float y, const float z): x(x), y(y), z(z) {};
    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    Vec3 operator-(const Vec3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    };
    Vec3 operator*(float scaler) const {
        return {x * scaler, y * scaler, z * scaler};
    }
    Vec3 operator/(float scaler) const {
        return {x / scaler, y / scaler, z / scaler};
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    };

    Vec3& operator*=(const Vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vec3& operator/=(const Vec3& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vec3 operator-() const {
        return {-x, -y, -z};
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    Vec3 normalized() const {
        float l = length();
        if (l < 0.0001f) {
            return {0.0f, 0.0f, 0.0f};
        }

        return {x / l, y / l, z / l};
    }
};
