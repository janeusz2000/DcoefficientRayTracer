#include "main/model.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <vector>

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSpeakerRayFactory;
using objects::TriangleObj;

class FakeModel : public ModelInterface {
public:
  std::vector<TriangleObj *> triangles() const override { return {}; }

  float height() const override { return 1; }
  float sideSize() const override { return 1; }
  bool empty() const override { return false; }
};

TEST(PointSpeakerRayFactoryTest, RayGenerator) {
  // FakeModel model;
  // PointSpeakerRayFactory rayFactory(9, &model);
  // Vec3 referenceDirection = Vec3(0, 0, model.height()) - rayFactory.origin();

  // Ray current(Vec3::kZero, Vec3::kZ);
  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceLeftLowerCorner(rayFactory.origin(),
  //                              referenceDirection - Vec3::kX / 2 - Vec3::kY);
  // ASSERT_EQ(referenceLeftLowerCorner, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceLowerMiddle(rayFactory.origin(), referenceDirection -
  // Vec3::kY); ASSERT_EQ(referenceLowerMiddle, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceRightLowerCorner(rayFactory.origin(),
  //                               referenceDirection + Vec3::kX - Vec3::kY);
  // ASSERT_EQ(referenceRightLowerCorner, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceLeft(rayFactory.origin(), referenceDirection - Vec3::kX);
  // ASSERT_EQ(referenceLeft, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceMiddle(rayFactory.origin(), referenceDirection);
  // ASSERT_EQ(referenceMiddle, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceRight(rayFactory.origin(), referenceDirection + Vec3::kX);
  // ASSERT_EQ(referenceRight, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceLeftUpperCorner(rayFactory.origin(),
  //                              referenceDirection - Vec3::kX + Vec3::kY);
  // ASSERT_EQ(referenceLeftLowerCorner, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceUpperMiddle(rayFactory.origin(), referenceDirection +
  // Vec3::kY); ASSERT_EQ(referenceUpperMiddle, current);

  // ASSERT_TRUE(rayFactory.genRay(&current));
  // Ray referenceRightUpperCorner(rayFactory.origin(),
  //                               referenceDirection + Vec3::kX + Vec3::kY);
  // ASSERT_EQ(referenceRightUpperCorner, current);

  // ASSERT_FALSE(rayFactory.genRay(&current));
}
