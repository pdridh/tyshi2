#pragma once

#include <cmath>
#include <vector>

class Vec2
{
public:
  double x = 0.0;
  double y = 0.0;

  Vec2() {}
  Vec2(double x_arg, double y_arg) : x(x_arg), y(y_arg) {}

  // Get zero vector
  inline static Vec2 ZERO() { return Vec2(0, 0); }

  // Normalizes the vector
  inline Vec2 &normalize()
  {
    double mag = magnitude();
    x /= mag;
    y /= mag;

    return *this;
  }
  // Get normalized vector without modifying the vecotr
  inline Vec2 getNormalized() const { return Vec2(*this).normalize(); }

  // The magnitude of this vector
  inline double magnitude() const { return sqrt((x * x) + (y * y)); }

  // Magnitude squared for faster calculations (no sqrt())
  inline double magnitudeSquared() const { return (x * x) + (y * y); }

  // Scale the vector by a factor
  inline Vec2 &scale(double factor)
  {
    x *= factor;
    y *= factor;

    return *this;
  };

  // Get scaled form without modifying the vector
  inline Vec2 getScaled(double factor) const { return Vec2(x * factor, y * factor); }

  // Operations
  inline Vec2 operator+(Vec2 const &right) const { return Vec2(x + right.x, y + right.y); }
  inline Vec2 operator-(Vec2 const &right) const { return Vec2(x - right.x, y - right.y); }
  inline void operator+=(Vec2 const &right)
  {
    x += right.x;
    y += right.y;
  }
  inline void operator-=(Vec2 const &right)
  {
    x -= right.x;
    y -= right.y;
  }
};