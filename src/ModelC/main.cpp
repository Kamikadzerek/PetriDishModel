#include "Surface.h"
#include <iostream>
const float NUMBEROFANGLES = 360;
extern const int ITERATIONBYONE = 500;
int main(int argc, char *argv[]) {
  float maxRadius = 30;
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
