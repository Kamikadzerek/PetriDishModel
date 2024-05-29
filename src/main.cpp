#include <iostream>
#include "Surface.h"
const float NUMBEROFANGLES = 360;
extern const int ITERATIONBYONE = 500;
int main(int argc, char* argv[]) {
  float maxRadius = atof(argv[1])*2+30;
  for (int i = atoi(argv[2]); i < atoi(argv[3]); i++) {
    Surface surface(i, NUMBEROFANGLES, atof(argv[1]), maxRadius);
    float radius = surface.getMeanRadiusOfLivingCells();
    while (radius <= maxRadius) {
      radius = surface.getMeanRadiusOfLivingCells();
      surface.update(ITERATIONBYONE);
    }
    surface.saveToFileAllNumberOfCellsEnclosedByRadius("DENSITY" + std::to_string(atof(argv[1])).substr(0, 4) + "Id" + std::to_string(i) + ".csv");
  }
  return 0;
}
