#ifndef GENERATORS_H
#define GENERATORS_H

#include "core/vec3.h"
#include "core/ray.h"
#include "constants.h"
#include "core/exceptions.h"

#include <initializer_list>
#include <iostream>

namespace generators
{
    class PointSource // This object is similar to Camera in graphics ray-tracer, that why name convension is simillar to camera
    {
    public:
        PointSource() = delete;
        PointSource(const double &freq, const size_t &rayNum);
        PointSource(const double &freq, const size_t &rayNum, const core::Vec3 &origin);
        PointSource(const double &freq, const size_t &raynum, const std::initializer_list<double> &origin);
        PointSource(const PointSource &) = default;
        ~PointSource() = default;

        // OPERATORS
        PointSource &operator=(const PointSource &) = default;
        bool operator==(const PointSource &other) const;
        friend std::ostream &operator<<(std::ostream &os, const PointSource &pointSource);

        // GETTERS AND SETTERS
        double getFrequency() const;
        void setFrequency(const double &freq);

        size_t getRayNum() const;
        void setRayNum(const size_t &raynum);

        core::Vec3 getOrigin() const;
        void setOrigin(const core::Vec3 &point);

    private:
        core::Vec3 _origin;
        double _frequency;
        size_t _rayNum;
    };
}; // namespace generators

#endif