#pragma once

#include <iostream>
#include <cmath>
#include <vector>

class Vec2
{
public:
  float x;
  float y;

  static constexpr float DIFFERENCE_TOLERATION = 1e-6;

  Vec2(float x_arg = 0.0, float y_arg = 0.0) : x(x_arg), y(y_arg) {}

  // Get zero vector
  inline static Vec2 ZERO() { return Vec2(0, 0); }

  // For debugging
  inline void print(const char *tag)
  {
    printf("{Vec2} %s: %f, %f\n", tag, x, y);
  }

  // Normalizes the vector
  inline Vec2 &normalize()
  {
    if (x || y)
    {
      float mag = magnitude();

      x /= mag;
      y /= mag;
    }

    return *this;
  }
  // Get normalized vector without modifying the vecotr
  inline Vec2 getNormalized() const { return Vec2(*this).normalize(); }

  // The magnitude of this vector
  inline float magnitude() const { return sqrt((x * x) + (y * y)); }

  // Magnitude squared for faster calculations (no sqrt())
  inline float magnitudeSquared() const { return (x * x) + (y * y); }

  // Distance from this vector to a vector
  inline float distanceTo(const Vec2 &right) const { return (*this - right).magnitude(); }

  // Distance distance from this vector to a vector
  inline float distanceToSquared(const Vec2 &right) const { return (*this - right).magnitudeSquared(); }

  // Scale the vector by a factor
  inline Vec2 &scale(float factor)
  {
    x *= factor;
    y *= factor;

    return *this;
  };

  // Get scaled form without modifying the vector
  inline Vec2 getScaled(float factor) const { return Vec2(x * factor, y * factor); }

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

  inline bool operator==(Vec2 const &right)
  {
    float deltaX = std::fabs(x - right.x);
    float deltaY = std::fabs(y - right.y);

    return deltaX < DIFFERENCE_TOLERATION && deltaY < DIFFERENCE_TOLERATION;
  }
};
