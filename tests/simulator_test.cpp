#include "core/constants.h"
#include "main/simulator.h"
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

#include <fstream>
#include <iostream>
#include <string_view>

// Checks if |TRY_BLOCK| throws right |EXCPETION_TYPE| with exception message
// equal to const char* |MESSAGE|
// TODO: check substring only, because every little change requirers of changing
// whole test, which is not desired
#define ASSERT_EXCEPTION_MSG(TRY_BLOCK, EXCEPTION_TYPE, MESSAGE)               \
  try {                                                                        \
    { TRY_BLOCK; }                                                             \
    FAIL() << "exception '" << MESSAGE << "' not thrown at all!";              \
  } catch (const EXCEPTION_TYPE &e) {                                          \
    EXPECT_STREQ(MESSAGE, e.what())                                            \
        << " exception message is incorrect. Expected the following "          \
           "message:\n\n"                                                      \
        << MESSAGE << "\n";                                                    \
  } catch (...) {                                                              \
    FAIL() << "exception " << MESSAGE << " not thrown with expected type "     \
           << #EXCEPTION_TYPE << "'!";                                         \
  }

using constants::kPi;
using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::TriangleObj;
using Json = nlohmann::json;

float deg2rad(float deg) { return 2 * constants::kPi * deg / 360; }

const float kSkipFrequency = 1000;
const int kSkipNumber = 37;

class FakeModel : public ModelInterface {

public:
  explicit FakeModel(bool empty) : triangles_({}), empty_(empty){};
  const std::vector<TriangleObj> &triangles() const override {
    return triangles_;
  }
  float height() const override { return 0; }
  float sideSize() const override { return 0; }
  bool empty() const override { return empty_; }

private:
  void printItself(std::ostream &os) const noexcept override {
    os << "Fake Model";
  }
  std::vector<TriangleObj> triangles_;
  bool empty_;
};

class EnergyCollectorTest : public ::testing::Test {
protected:
  using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

  // performs ray hit at at given energy collectors, |hitData| is modified to
  // hold information where ray hit energyCollector. Returns true if hit
  // occurred, false when there was no hit
  [[nodiscard]] bool performHitCollector(Collectors &energyCollectors,
                                         const Ray &ray, RayHitData *hitData) {
    bool hit = false;
    RayHitData closestHitData;

    for (auto &collector : energyCollectors) {
      if (collector->hitObject(ray, kSkipFrequency, hitData)) {
        hit = true;
        if (hitData->time < closestHitData.time) {
          closestHitData = *hitData;
        }
      }
    }
    *hitData = closestHitData;
    return hit;
  }

  float getCollectorRadius(const Collectors &energyCollectors) const {
    return energyCollectors[0]->getRadius();
  }

  // returns value of angle between two neighborhood collectors and
  // Vec3(0, 0, 0)
  float getCollectorAngle(const int numCollector) const {
    return 2 * kPi / (numCollector + (numCollector % 2) - 2);
  }

  void printCollectors(const Collectors &energyCollectors) const {
    int i = 0;
    for (const auto &collector : energyCollectors) {
      std::cout << i++ << " " << *collector << std::endl;
    }
  }

  float getMaxZ(const Collectors &energyCollectors) {

    float maxZ = 0;
    for (const auto &collector : energyCollectors) {

      maxZ = std::max(maxZ, collector->getOrigin().z());
    }
    return maxZ;
  }

  float getCollectorPositionRadius(const Collectors &energyCollectors) {
    float maxX = 0;
    for (const auto &collector : energyCollectors) {
      maxX = std::max(maxX, collector->getOrigin().x());
    }
    return maxX;
  }
};

TEST_F(EnergyCollectorTest, ModelEmptyNotBuildingCollectors) {
  const FakeModel emptyModel(true);
  DoubleAxisCollectorBuilder collectorBuilder;
  ASSERT_EXCEPTION_MSG(
      collectorBuilder.buildCollectors(&emptyModel, kSkipNumber),
      std::invalid_argument,
      "Model given to buildCollectors() cannot be Empty!");

  const FakeModel nonEmptyModel(false);
  EXPECT_NO_THROW(
      collectorBuilder.buildCollectors(&nonEmptyModel, kSkipNumber));
}

TEST_F(EnergyCollectorTest, ThrowExceptionWhenInvalidNumCollector) {
  const FakeModel nonEmptyModel(false);
  DoubleAxisCollectorBuilder collectorBuilder;
  ASSERT_EXCEPTION_MSG(
      collectorBuilder.buildCollectors(&nonEmptyModel, 38),
      std::invalid_argument,
      "given number to buildCollectors() numCollectors or numCollectors - 1"
      "\nhas to be divisible by 4. Instead got numCollectors:  38");

  ASSERT_EXCEPTION_MSG(
      collectorBuilder.buildCollectors(&nonEmptyModel, 3),
      std::invalid_argument,
      "given numCollectors in buildCollectors(): 3, is less then 4");

  // Test case when numCollector - 1 % 4 = 0
  EXPECT_NO_THROW(collectorBuilder.buildCollectors(&nonEmptyModel, 37));

  // Test case when numCollector % 4 = 0
  EXPECT_NO_THROW(collectorBuilder.buildCollectors(&nonEmptyModel, 36));
}

TEST_F(EnergyCollectorTest, NotEvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);
  DoubleAxisCollectorBuilder collectorBuilder;
  const int numCollectors = 37;
  auto energyCollectors =
      collectorBuilder.buildCollectors(&nonEmptyModel, numCollectors);
  ASSERT_EQ(numCollectors, energyCollectors.size());

  Ray straightUp(Vec3::kZero, Vec3::kZ);
  RayHitData hitData;
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  const float collectorPositionRadius =
      getCollectorPositionRadius(energyCollectors);
  ASSERT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray straightDown(Vec3::kZero, -Vec3::kZ);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(Vec3::kZero, Vec3::kX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray alongY(Vec3::kZero, Vec3::kY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  const float collectorAngle = getCollectorAngle(energyCollectors.size());
  Ray at2Angle = Ray::fromSphericalCoords(Vec3::kZero, 0, 2 * collectorAngle);
  ASSERT_TRUE(performHitCollector(energyCollectors, at2Angle, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray atSixtyXY =
      Ray::fromSphericalCoords(Vec3::kZero, deg2rad(60), deg2rad(60));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray atSixtyXYOther =
      Ray::fromSphericalCoords(Vec3::kZero, deg2rad(120), deg2rad(120));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, EvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);
  DoubleAxisCollectorBuilder collectorBuilder;
  const int numCollectors = 36;
  auto energyCollectors =
      collectorBuilder.buildCollectors(&nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  Ray straightUp(Vec3::kZero, Vec3::kZ);
  RayHitData hitData;
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float collectorPositionRadius =
      getCollectorPositionRadius(energyCollectors);
  float collectorsMaxZ = getMaxZ(energyCollectors);
  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  // this comes from the fact, two origins of neighborhood collectors and
  // collision point are creates equilateral triangle which side is equal to
  // collector radius. Thats why collision point its just the point between
  // two collectors origin - height of the equilateral triangle.
  ASSERT_FLOAT_EQ(collectorsMaxZ - refCollectorRadius * std::sqrt(3) / 2,
                  hitData.time);

  Ray straightDown(Vec3::kZero, -Vec3::kZ);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(Vec3::kZero, Vec3::kX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray alongY(Vec3::kZero, Vec3::kY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  const float collectorAngle = getCollectorAngle(energyCollectors.size());
  Ray atAngle = Ray::fromSphericalCoords(Vec3::kZero, 0, collectorAngle);
  ASSERT_TRUE(performHitCollector(energyCollectors, atAngle, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray at30XY = Ray::fromSphericalCoords(Vec3::kZero, deg2rad(30), deg2rad(60));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray at30XYOther =
      Ray::fromSphericalCoords(Vec3::kZero, deg2rad(210), deg2rad(30));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, NoHoleNextToTheTopCollectorOddNum) {
  const FakeModel nonEmptyModel(false);
  DoubleAxisCollectorBuilder collectorBuilder;
  const int numCollectors = 37;
  auto energyCollectors =
      collectorBuilder.buildCollectors(&nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  const float collectorPositionRadius = 4;
  // this is how previous implementation was caclualating radius of energy
  // collector
  const float invalidEnergyCollectorRadius =
      2 * kPi * collectorPositionRadius / numCollectors;

  RayHitData hitData;
  Ray previousNotHit1(Vec3::kZero, Vec3(0, 1.01 * invalidEnergyCollectorRadius,
                                        collectorPositionRadius));
  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit1, &hitData));

  Ray previousNotHit2(Vec3::kZero, Vec3(0, -1.01 * invalidEnergyCollectorRadius,
                                        collectorPositionRadius));

  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit2, &hitData));
}

TEST_F(EnergyCollectorTest, HitRayStraightUpEvenCollectors) {
  const FakeModel nonEmptyModel(false);
  DoubleAxisCollectorBuilder collectorBuilder;
  const int numCollectors = 20;
  auto energyCollectors =
      collectorBuilder.buildCollectors(&nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  RayHitData hitData;
  Ray straightUp(Vec3::kZero, Vec3::kZ);
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  float collectorsMaxZ = getMaxZ(energyCollectors);
  // See EvenNumOfEnergyCollectorTest for explanation
  ASSERT_FLOAT_EQ(collectorsMaxZ - refCollectorRadius * std::sqrt(3) / 2,
                  hitData.time);
}
