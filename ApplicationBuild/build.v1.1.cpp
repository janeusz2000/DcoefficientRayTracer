#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>

int main() {

  std::string path = "./models/monkeyfull.obj";
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());
  float sourcePower = 500;

  int numOfCollectors = 37;
  int numOfRaysAlongEachAxis = 15;
  collectionRules::LinearEnergyCollection energyCollectionRules;

  std::vector<float> frequencies = {100, 200, 300, 400, 500};
  SimulationProperties properties("./data", frequencies,
                                  &energyCollectionRules);

  SceneManager manager(model.get(), properties);
  manager.run();
}
