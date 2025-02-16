#ifndef CLASS_UTILITIES_H
#define CLASS_UTILITIES_H

#include <boost/core/noncopyable.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>


class Printable {
public:
  virtual ~Printable(){};
  virtual void printItself(std::ostream &os) const noexcept;
};

inline std::ostream &operator<<(std::ostream &os, const Printable &printable) {
  printable.printItself(os);
  return os;
}

struct RandomEngine : public Printable, private boost::noncopyable {
  explicit RandomEngine();
  float getRandomFloat() const;
  int getRandomIntInRange(int min, int max) const;
  void printItself(std::ostream &os) const noexcept override;
};

#endif