#include "main/simulator.h"

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const ModelInterface &model, int numCollectors) {

  if (model.empty()) {
    throw std::invalid_argument("Empty model");
  }
  if (numCollectors < 4) {
    std::stringstream ss;
    ss << "numCollectors: " << numCollectors << " is less then 4";
    throw std::invalid_argument(ss.str());
  }

  // We distribute collectors along 4 quarter circles above the model, so
  // |numCollectors| has to be divisible by 4. Or |numCollectors|-1 has to be
  // divisible by 4 (if it is odd), in which case there is one more collector
  // placed right above the model.

  if (numCollectors % 4 != 0 && (numCollectors - 1) % 4 != 0) {
    std::stringstream ss;
    ss << "numCollectors or numCollectors-1 has to be divisible by 4, got "
       << "numCollectors = " << numCollectors;
    throw std::invalid_argument(ss.str());
  }

  // Minimum distance from Vec3(0, 0, 0) that each collector must meet.
  // Value of the minDistance can't be less then 4 [m]
  float minDistance =
      4 * std::max<float>(
              {model.height(), model.sideSize(),
               /*makes sure that minDistance is at least equal to 4 */ 1.0f});

  // For even number of ray collectors, in order to equally cover two simulation
  // half-circle (half-circle that cross each collectors origin), we need to
  // calculate angle between Vec3(0, 0, 0) and 2 points where energy collector
  // radius will cross simulation half-circle. Calculation will varies depending
  // on if number of collectors is even or not.

  const int numCollectorIsOdd = numCollectors % 2;
  const float angle = 2 * constants::kPi /
                      static_cast<float>(numCollectors + numCollectorIsOdd - 2);

  // After gathering previously mentioned angle, we can calculate radius of each
  // energy collector, from Law of Cosines:
  // https://en.wikipedia.org/wiki/Law_of_cosines
  // and after couple fo transformations formula will look like this:
  // r = R * sqrt(2 - 2 * cos(angle))

  float energyCollectorRadius =
      minDistance * static_cast<float>(std::sqrt(2 - 2 * std::cos(angle)));

  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors;
  // When Num Collectors is not even, we need to put one collector at (0, 0,
  // minRadius)`
  if (numCollectorIsOdd) {
    energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
        core::Vec3(0, 0, minDistance), energyCollectorRadius));
  }
  // and decrease number remaining collectors to create remaining ones
  const int numToGo = numCollectors - (numCollectorIsOdd);

  // Remaining energy collectors:
  std::vector<core::Vec3> origins;
  for (int times = 0; times * 4 < numToGo; ++times) {

    float groundCoordinate = minDistance * std::cos(times * angle);
    float zCoordinate = minDistance * std::sin(times * angle);

    origins = {core::Vec3(groundCoordinate, 0, zCoordinate),
               core::Vec3(-groundCoordinate, 0, zCoordinate),
               core::Vec3(0, -groundCoordinate, zCoordinate),
               core::Vec3(0, groundCoordinate, zCoordinate)};

    for (const core::Vec3 origin : origins) {
      energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
          origin, energyCollectorRadius));
    }
  }
  return energyCollectors;
}