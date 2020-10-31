#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
const float kAccuracy = 0.000005;
const float kSoundSpeed = 343.216; // This is measured in 20'C at 1000 hPa
const float kPi = std::acos(-1);
const float kDefaultSimulationRadius = 4; // [m]
const int kPopulation = 37;
const float kHitAccuracy = 0.0001;

// ( 2 *  pi * R ) / 2 is for dividing whole sphereWall to equal distances
const float kDefaultEnergyCollectorRadius =
    kPi * kDefaultSimulationRadius / kPopulation;

} // namespace constants


#endif