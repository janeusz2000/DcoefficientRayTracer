#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSource;
using objects::Object;
using objects::TriangleObj;

class SceneManagerTest : public ::testing::Test
{
protected:
    bool performHitAtEnergyCollectors(const Ray &inputRay, RayHitData *hitData)
    {
        for (objects::EnergyCollector *energyCol : manager.getEnergyCollectors())
        {
            const float kSkipFreq = 1000;
            if (energyCol->hitObject(inputRay, kSkipFreq, hitData))
            {
                return true;
            }
        }
        return false;
    }
    
    SceneManager manager;
    core::Vec3 kVZero;
};

TEST_F(SceneManagerTest, EnergyCollectorPositionsCheck)
{
    for (const auto &collectorPtr : manager.getEnergyCollectors())
    {
        std::cout << *collectorPtr << std::endl;
    }

    ASSERT_EQ(manager.getEnergyCollectors().size(), constants::kPopulation) << "wrong numebr of energy collectors";

    RayHitData hitData;
    Ray straightUp(kVZero, core::Vec3(0, 0, 1));

    ASSERT_TRUE(performHitAtEnergyCollectors(straightUp, &hitData)) << "no hit: " << straightUp;

    core::Vec3 referenceCollisionPoint(0, 0, manager.simulatiorRadius() / 2 - manager.collectorRadius());
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "point from: " << straightUp;

    Ray parallelXAxis(kVZero, core::Vec3(-1, 0, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(parallelXAxis, &hitData)) << "no hit: " << parallelXAxis;

    Vec3 referenceCollisionPoint2(
        -(manager.simulatiorRadius() / 2 - manager.collectorRadius()), 0, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint2)
        << " point from: " << parallelXAxis;

    Ray parallelYaxis(kVZero, core::Vec3(0, 1, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(parallelYaxis, &hitData))
        << "no hit: " << parallelYaxis;

    Vec3 referenceCollisionPoint3(0, manager.simulatiorRadius() / 2 - manager.collectorRadius(), 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint3)
        << " point from: " << parallelYaxis;

    Ray atSixtyDegreesAlongX(kVZero, core::Vec3(std::cos(constants::kPi / 3), 0,
                                                std::sin(constants::kPi / 3)));
    ASSERT_TRUE(performHitAtEnergyCollectors(atSixtyDegreesAlongX, &hitData))
        << "no hit: " << atSixtyDegreesAlongX;

    Vec3 referenceCollisionAtSixty(
        manager.simulatiorRadius() / 2 * std::cos(constants::kPi / 3) -
            manager.collectorRadius() * std::cos(constants::kPi / 3),
        0,
        manager.simulatiorRadius() / 2 * std::sin(constants::kPi / 3) -
            manager.collectorRadius() * std::sin(constants::kPi / 3));

    ASSERT_EQ(hitData.collisionPoint, referenceCollisionAtSixty)
        << "Invalid hit from " << atSixtyDegreesAlongX
        << " to: " << referenceCollisionAtSixty;

    Ray atThirtyDegreesAlongX(kVZero,
                              core::Vec3(std::cos(constants::kPi / 6), 0,
                                         std::sin(constants::kPi / 6)));
    ASSERT_TRUE(performHitAtEnergyCollectors(atThirtyDegreesAlongX, &hitData))
        << "no hit: " << atThirtyDegreesAlongX;

    Vec3 referenceCollisionAtThirty(
        manager.simulatiorRadius() / 2 * std::cos(constants::kPi / 6) -
            manager.collectorRadius() * std::cos(constants::kPi / 6),
        0,
        manager.simulatiorRadius() / 2 * std::sin(constants::kPi / 6) -
            manager.collectorRadius() * std::sin(constants::kPi / 6));

    ASSERT_EQ(hitData.collisionPoint, referenceCollisionAtThirty)
        << "Invalid hit from " << atThirtyDegreesAlongX
        << " to: " << referenceCollisionAtThirty;

    Ray straightDown(kVZero, core::Vec3(0, 0, -1));
    ASSERT_FALSE(performHitAtEnergyCollectors(straightDown, &hitData))
        << "hit registered in: " << hitData.collisionPoint
        << "from ray: " << straightDown;

    // Ray at the edge of EnergyCollector at: Vec3(0, 0, 2)
    Ray straightUpAbove(
        core::Vec3(0, 0,
                   manager.simulatiorRadius() / 2 + manager.collectorRadius()),
        core::Vec3(0, 0, 1));
    ASSERT_FALSE(performHitAtEnergyCollectors(straightUpAbove, &hitData))
        << "hit registered in: " << hitData.collisionPoint
        << "from ray: " << straightUpAbove;
}
