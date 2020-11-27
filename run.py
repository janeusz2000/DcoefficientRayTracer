import pythonTools.core.energyCollectorsTools as tools

# creates picture of energyCollectors from energyCollectors.txt
# txt file and draws all hits from sectionData.txt

projectDirectory = "D:\\cpp_projects\\magisterkaCPP\\"

def run():
  plotter = tools.Plotter(
    modelPath= projectDirectory + "pythonTools\\data\\triangles.txt",
    collectorPath = projectDirectory + "pythonTools\\data\\energyCollectors.txt",
    sectionPath = projectDirectory + "pythonTools\\data\\sectionsData.txt" )
  plotter.save( projectDirectory + "pythonTools\\data\\test", plot=False)

if __name__ == "__main__":
  run()