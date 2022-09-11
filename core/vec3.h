#ifndef VEC3_H
#define VEC3_H

#include "classUtlilities.h"
#include "constants.h"

#include <iostream>
#include <random>
#include <sstream>

namespace core {
class Vec3 : public Printable {
public:
  static const Vec3 kZero;
  static const Vec3 kX;
  static const Vec3 kY;
  static const Vec3 kZ;

  // X cord: represents left (-) to right (+) coordinates
  // Y cord: represents backwards (-) to forward (+) coordinates
  // Z cord: represents down (-) to up (+) coordinates
  explicit Vec3(float x = 0, float y = 0, float z = 0) : x_(x), y_(y), z_(z){};

  Vec3 &operator=(const Vec3 &other);
  Vec3 &operator+=(const Vec3 &other);
  Vec3 &operator+=(float num);
  Vec3 &operator-=(const Vec3 &other);
  Vec3 &operator-=(float num);
  Vec3 &operator*=(float num);
  Vec3 operator-() const { return *this * -1; }
  bool operator==(const Vec3 &other) const;
  bool operator!=(const Vec3 &other) const;

  friend Vec3 operator+(const Vec3 &left, const Vec3 &right);
  friend Vec3 operator+(const Vec3 &left, float right);
  friend Vec3 operator+(float left, const Vec3 &right);
  friend Vec3 operator-(const Vec3 &left, const Vec3 &right);
  friend Vec3 operator-(const Vec3 &left, float right);
  friend Vec3 operator*(float num, const Vec3 &vec);
  friend Vec3 operator*(const Vec3 &vec, float num);
  friend Vec3 operator/(const Vec3 &vec, float num);

  float scalarProduct(const Vec3 &other) const;
  Vec3 crossProduct(const Vec3 &other) const;
  float magnitude() const;
  float magnitudeSquared() const;
  Vec3 normalize() const;

  float x() const;
  void setX(float num);
  float y() const;
  void setY(float num);
  float z() const;
  void setZ(float num);

  void printItself(std::ostream &os) const noexcept override;

private:
  float x_, y_, z_;
};




} // namespace core
#endif
