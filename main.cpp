#include <iostream>
#include "Surface.h"
const float NUMBEROFANGLES = 360;
extern const int ITERATIONBYONE = 500;
int main(int argc, char* argv[]) {
//  argv[1]=new char('1');
//  argv[2]=new char('0');
//  argv[3]=new char('1');
  float maxRadius = atof(argv[1])*2;
  if (maxRadius < 50){
    maxRadius=50;
  }
  for (int i = atoi(argv[2]); i < atoi(argv[3]); i++) {
    Surface surface(i, NUMBEROFANGLES, atof(argv[1]), maxRadius);
    float radius = surface.getMeanRadiusOfLivingCells();
    while (radius <= maxRadius) {
      //      std::cout << i << ": " << radius / MAXRADIUS * 100 << "%\n";
      radius = surface.getMeanRadiusOfLivingCells();
      surface.update(ITERATIONBYONE);
      //      std::cout << "\033[A\r\33[2K";
    }
    surface.saveToFileAllNumberOfCellsEnclosedByRadius("DENSITY" + std::to_string(atof(argv[1])).substr(0, 4) + "Id" + std::to_string(i) + ".csv");
  }
  return 0;
}
