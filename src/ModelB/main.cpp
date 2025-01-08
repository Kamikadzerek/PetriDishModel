#include "Surface.h"
#include <iostream>
#include <string>
const float NUMBEROFANGLES = 360;
extern const int ITERATIONBYONE = 100;
int main(int argc, char *argv[]) {
  float maxRadius = atof(argv[1]) * 2 + 30;
  maxRadius = 60;
  int j = 0;
  for (int i = atoi(argv[2]); i < atoi(argv[3]); i++) {
    Surface surface(i, NUMBEROFANGLES, atof(argv[1]), maxRadius);
    float radius = surface.getMeanRadiusOfLivingCells();
    while (radius <= maxRadius) {
      radius = surface.getMeanRadiusOfLivingCells();
      surface.update(ITERATIONBYONE);
      std::cout << radius << "/" << maxRadius << "\n";
    }
    surface.saveToFile("Space1.csv");
  }
  return 0;
}
