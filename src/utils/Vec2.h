#pragma once

#include "../core/core.h"

#include <iostream>
#include <type_traits>
#include <cassert>
#include <cmath>

template <typename T>
class Vec2
{
private:
public:
  T x;
  T y;

  static constexpr f32 DIFFERENCE_TOLERATION = 1e-6;

  Vec2<T>(T x_arg = 0, T y_arg = 0) : x(x_arg), y(y_arg)
  {
    static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>, "Vec2 only available for floats and ints");
  }

  template <typename U>
  Vec2<T>(const Vec2<U> &other) : x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)}
  {
  }

  template <typename U>
  Vec2<T>(const Vec2<U> *other) : x{static_cast<T>(other->x)}, y{static_cast<T>(other->y)}
  {
  }

  // Get zero vector
  inline static Vec2<T> ZERO()
  {
    return Vec2<T>(0, 0);
  }

  // Get coords as std::pair
  inline std::pair<T, T> pair()
  {
    return {x, y};
  }

  // For debugging
  inline void print(const char *tag)
  {
    if (std::is_floating_point_v<T>)
    {
      printf("{Vec2f} %s: %f, %f\n", tag, x, y);
    }
    else
    {
      printf("{Vec2i} %s: %d, %d\n", tag, x, y);
    }
  }

  // Normalizes the vector

  inline Vec2<T> &normalize()
  {
    static_assert(std::is_floating_point_v<T>, "Vec2::normalize only works with floats");

    if (x || y)
    {
      f32 mag = magnitude();

      x /= mag;
      y /= mag;
    }

    return *this;
  }

  // Get normalized vector without modifying the vecotr
  inline Vec2<T> getNormalized() const
  {
    static_assert(std::is_floating_point_v<T>, "Vec2::getNormalized only works with floats");
    return Vec2<T>(*this).normalize();
  }

  // The magnitude of this vector
  inline f32 magnitude() const
  {
    return std::sqrt(x * x + y * y);
  }

  // Magnitude squared for faster calculations (no sqrt())
  inline f32 magnitudeSquared() const { return (x * x) + (y * y); }

  // Distance from this vector to a vector
  inline f32 distanceTo(const Vec2<T> &right) const
  {
    return (*this - right).magnitude();
  }

  // Distance distance from this vector to a vector
  inline f32 distanceToSquared(const Vec2<T> &right) const
  {
    return (*this - right).magnitudeSquared();
  }

  // Scale the vector by a factor
  inline Vec2<T> &scale(T factor)
  {
    x *= factor;
    y *= factor;

    return *this;
  };

  // Scale down the vector (divide)
  inline Vec2<T> &scaleDown(T factor)
  {
    assert(factor != 0 && "Vec2::scaleDown Cant scale down by 0, (dividing by 0)");
    return this->scale(1.f / factor);
  }

  // Get scaled form without modifying the vector
  inline Vec2<T> getScaled(T factor) const
  {
    return Vec2<T>(this).scale(factor);
  }

  // Get scaled down form without modifying the vector
  inline Vec2<T> getScaledDown(T factor) const
  {
    return Vec2<T>(this).scaleDown(factor);
  }

  // Operations

  inline Vec2<T> operator+(Vec2 const &right) const
  {
    return Vec2<T>(x + right.x, y + right.y);
  }

  inline Vec2 operator-(Vec2<T> const &right) const
  {
    return Vec2<T>(x - right.x, y - right.y);
  }

  inline Vec2<T> operator-() const
  {
    return Vec2<T>(-x, -y);
  }

  inline Vec2<T> &operator+=(Vec2<T> const &right)
  {
    x += right.x;
    y += right.y;
    return *this;
  }

  inline Vec2<T> &operator-=(Vec2 const &right)
  {
    x -= right.x;
    y -= right.y;
    return *this;
  }

  inline bool operator==(Vec2<T> const &right)
  {
    // Floating comparision
    if (std::is_floating_point_v<T>)
    {
      f32 deltaX = std::fabs(x - right.x);
      f32 deltaY = std::fabs(y - right.y);

      return deltaX < DIFFERENCE_TOLERATION && deltaY < DIFFERENCE_TOLERATION;
    }

    return (x == right.x) && (y == right.y);
  }
};

// Easy access
typedef Vec2<f32> Vec2f;
typedef Vec2<i32> Vec2i;
