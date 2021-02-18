#include "main/model.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::FakeOffseter;
using generators::PointSpeakerRayFactory;

class MainTest : public ::testing::Test {
public:
  MainTest()
      : sourcePower_(500), frequency_(1e3), numOfCollectors_(37),
        numOfRayAlongEachAxis_(1) {
    model_ = Model::NewReferenceModel(1.0);

    trackers::saveModelToJson("/tmp", model_.get());
  }

protected:
  float sourcePower_;
  float frequency_;
  int numOfCollectors_;
  int numOfRayAlongEachAxis_;
  std::unique_ptr<Model> model_;
};

TEST_F(MainTest, buildingSimulation) {

  RayTracer rayTracer(model_.get());
  trackers::PositionTracker positionTracker("/tmp");
  PointSpeakerRayFactory pointSpeaker(numOfRayAlongEachAxis_, sourcePower_,
                                      model_.get());
  collectionRules::LinearEnergyCollection energyCollectionRules;

  FakeOffseter rayOffseter;
  Simulator simulator(&rayTracer, model_.get(), &pointSpeaker, &rayOffseter,
                      &positionTracker, &energyCollectionRules);

  std::vector<objects::EnergyCollector> collectors =
      buildCollectors(model_.get(), numOfCollectors_);
  std::promise<std::vector<float>> accumulatedPromise;
  std::future<std::vector<float>> accumulatedFuture =
      accumulatedPromise.get_future();

  simulator.run(frequency_, collectors, &accumulatedPromise);
  std::vector<float> result = accumulatedFuture.get();
  ASSERT_TRUE(result.size() > 0);
  // positionTracker.saveAsJson();
  // FAIL();
}
