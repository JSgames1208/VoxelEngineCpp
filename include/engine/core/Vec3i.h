//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once

class Vec3i {
public:
    Vec3i(const int x, const int y, const int z);
    bool equals(const Vec3i& other);

    int getX() const;
    int getY() const;
    int getZ() const;

    float getDistance(const Vec3i& other);

    Vec3i operator+(const Vec3i& other);
    Vec3i operator-(const Vec3i& other);
    Vec3i operator*(const int scalar);
private:
    void setX(const int x);
    void setY(const int y);
    void setZ(const int z);
private:
    int x;
    int y;
    int z;
};

