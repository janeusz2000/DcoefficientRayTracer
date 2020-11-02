#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <cmath>
#include <random>
#include <sstream>

using constants::kDefaultEnergyPerRay;
using constants::kSkipFreq;
using core::Ray;
using core::RayHitData;
using core::Vec3;

namespace objects {

TEST(SphereWallObjectTest, RayInsideSphereHit) {
  Vec3 kO(123, 1, 2);
  const float kR = 3;
  Sphere sphere(kO, kR);

  RayHitData ignore;

  Vec3 dir(1, 2, 3);
  Ray rayInsideSphere(sphere.getOrigin(), dir, kDefaultEnergyPerRay);
  ASSERT_TRUE(sphere.hitObject(rayInsideSphere, kSkipFreq, &ignore));

  Ray rayOutsideSphere(sphere.getOrigin() + Vec3(2 * kR, 0, 0), dir,
                       kDefaultEnergyPerRay);
  ASSERT_FALSE(sphere.hitObject(rayOutsideSphere, kSkipFreq, &ignore));
}
} // namespace objects
