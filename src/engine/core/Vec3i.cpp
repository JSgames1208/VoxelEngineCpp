//
// Created by Jasper Soete on 2-4-2026.
//

#include <cmath>
#include "engine/core/Vec3i.h"

Vec3i::Vec3i(const int x, const int y, const int z)
    : x(x)
    , y(y)
    , z(z)
{
}

bool Vec3i::equals(const Vec3i &other)
{
    return (this->x == other.x && this->y == other.y && this->z == other.z);
}

int Vec3i::getX() const { return x; }
int Vec3i::getY() const { return y; }
int Vec3i::getZ() const { return z; }

float Vec3i::getDistance(const Vec3i& other)
{
    return (float)sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2) + pow(this->z - other.z, 2));
}

Vec3i Vec3i::operator+(const Vec3i& other)
{
    return Vec3i(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3i Vec3i::operator-(const Vec3i& other)
{
    return Vec3i(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3i Vec3i::operator*(const int scalar)
{
    return Vec3i(this->x * scalar, this->y * scalar, this->z * scalar);
}

void Vec3i::setX(const int x) { this->x = x; }
void Vec3i::setY(const int y) { this->y = y; }
void Vec3i::setZ(const int z) { this->z = z; }