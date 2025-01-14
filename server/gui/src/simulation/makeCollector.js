"use strict";
import { hsl } from "./hsl";
import { makeInstance } from "./makeInstance";

const THREE = require("three");

export function makeCollector(number, x, y, z, radius, scene) {
  const geometry = new THREE.SphereGeometry(radius, 32, 32);
  const energyCollector = makeInstance(
    geometry,
    hsl(1 / 8, 1, 0.5),
    0.3,
    x,
    z,
    y
  );
  scene.add(energyCollector);
  return energyCollector;
}
