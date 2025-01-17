#include "Cell.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <random>
#include <string>
#include <vector>
extern const int ITERATIONBYONE;
class Surface // store all cells
{

private:
  std::vector<std::vector<int>> conflictQueue = {
      std::vector<int>({0, 0, 0}),   std::vector<int>({1, 0, 0}),
      std::vector<int>({-1, 0, 0}),  std::vector<int>({0, 1, 0}),
      std::vector<int>({0, -1, 0}),  std::vector<int>({0, 0, 1}),
      std::vector<int>({0, 0, -1}),  std::vector<int>({1, 1, 0}),
      std::vector<int>({1, -1, 0}),  std::vector<int>({1, 0, 1}),
      std::vector<int>({1, 0, -1}),  std::vector<int>({-1, 1, 0}),
      std::vector<int>({-1, -1, 0}), std::vector<int>({-1, 0, 1}),
      std::vector<int>({-1, 0, -1}), std::vector<int>({0, 1, 1}),
      std::vector<int>({0, 1, -1}),  std::vector<int>({0, -1, 1}),
      std::vector<int>({0, -1, -1}), std::vector<int>({1, 1, 1}),
      std::vector<int>({1, 1, -1}),  std::vector<int>({1, -1, 1}),
      std::vector<int>({1, -1, -1}), std::vector<int>({-1, 1, 1}),
      std::vector<int>({-1, 1, -1}), std::vector<int>({-1, -1, 1}),
      std::vector<int>({-1, -1, -1})};
  float maxRadius;
  int treeSize;
  int halfOfTreeSize;
  int treeSizeZ;
  int halfOfTreeSizeZ;
  std::vector<std::vector<std::vector<std::vector<Cell *>>>> tree;
  int densityOfMeasurements = 5;
  float lastMeanRadius = 0;
  float currentMeanRadius = 0;
  struct angle {
    float sin;
    float cos;
    angle(float sin, float cos) : sin(sin), cos(cos) {}
  };
  struct pairAngles {
    angle theta;
    angle phi;
    pairAngles(angle theta, angle phi) : theta(theta), phi(phi) {}
  };
  struct vector2d {
    float x;
    float y;
    vector2d(float x, float y) : x(x), y(y) {}
  };
  std::vector<Cell> cells;
  std::vector<Cell *> aliveCells;
  std::vector<angle> angles;
  std::vector<pairAngles> combinationsAngles;
  float initialX = 0;
  float initialY = 0;
  float initialZ = 0;
  int id;
  int numOfAngles;
  int stateCounter = 0;
  int iterationCounter = 0;
  int aliveCellsCounter;
  float thickness;
  float halfOfThickness;
  float spawnDistance = 1;
  float cellRadius = spawnDistance / 2;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<float> dis;
  bool circleIsOverCircle(const Cell &cell1, const Cell cell2) const {
    float distZ = cell1.getZ() - cell2.getZ();
    float distX = cell1.getX() - cell2.getX();
    float distY = cell1.getY() - cell2.getY();
    if (std::abs(distX) < 1) {
      if (std::abs(distZ) < 1) {
        if (pow(distX, 2) + pow(distY, 2) + pow(distZ, 2) < 1) {
          return true;
        }
      }
    }
    return false;
  }

public:
  explicit Surface(int i, int numberOfAngles, float thick, float maxRadius)
      : id(i), numOfAngles(numberOfAngles), thickness(thick),
        maxRadius(maxRadius) {
    treeSize = maxRadius * 4;
    halfOfTreeSize = maxRadius * 2;
    treeSizeZ = 5 + thickness * 3;
    halfOfTreeSizeZ = treeSizeZ / 2;
    halfOfThickness = thickness / 2;
    gen = std::mt19937(
        std::chrono::system_clock::now().time_since_epoch().count());
    dis = std::uniform_real_distribution<float>(0.0, 1.0);
    aliveCellsCounter = 1;
    cells.reserve(pow(maxRadius / cellRadius, 2) * (thickness + 1) +
                  2 * ITERATIONBYONE);
    for (float angle = 0; angle < 2 * M_PI; angle += (2 * M_PI) / numOfAngles) {
      angles.emplace_back(sin(angle), cos(angle));
    }
    for (auto angle1 : angles) {
      for (auto angle2 : angles) {
        combinationsAngles.emplace_back(angle1, angle2);
      }
    }
    for (int i = 0; i < treeSize; i++) {
      tree.emplace_back();
      for (int j = 0; j < treeSize; j++) {
        tree.back().emplace_back();
        for (int k = 0; k < treeSizeZ; k++) {
          tree.back().back().emplace_back();
          tree.back().back().back().reserve(1);
        }
      }
    }
    addCell(initialX, initialY, initialZ);
  }
  ~Surface() { cells.clear(); }
  void addCell(float x, float y, float z) {
    cells.emplace_back(x, y, z);
    aliveCells.emplace_back(&cells.back());
    aliveCellsCounter++;
    tree[std::floor(x) + halfOfTreeSize][std::floor(y) + halfOfTreeSize]
        [std::floor(z) + halfOfTreeSizeZ]
            .push_back(&cells.back());
  }
  void deadCell(Cell *c) {

    c->death();
    for (auto it = aliveCells.begin(); it != aliveCells.end(); it++) {
      if ((*it)->getX() == c->getX() && (*it)->getY() == c->getY()) {
        std::swap(*it, aliveCells.back());
        aliveCells.pop_back();
        aliveCellsCounter--;
        break;
      }
    }
    // auto it = find(aliveCells.begin(), aliveCells.end(), c);
    // if (it != aliveCells.end()) {
    // std::swap(*it, aliveCells.back());
    // aliveCells.pop_back();
    // }
    // aliveCellsCounter--;
  }
  double getHeight() {
    double maxZ = 0;
    double minZ = 0;
    for (auto cell : cells) {
      double cellHeight = cell.getZ();
      if (cellHeight > maxZ) {
        maxZ = cellHeight;
      } else if (cellHeight < minZ) {
        minZ = cellHeight;
      }
    }
    return maxZ-minZ;
  }
  bool cellIsConflicting(const Cell cell) {
    int x = std::floor(cell.getX()) + halfOfTreeSize;
    int y = std::floor(cell.getY()) + halfOfTreeSize;
    int z = std::floor(cell.getZ()) + halfOfTreeSizeZ;
    for (auto &dir : conflictQueue) {
      if (!tree[x + dir[0]][y + dir[1]][z + dir[2]].empty()) {
        for (auto const &cellTemp : tree[x + dir[0]][y + dir[1]][z + dir[2]]) {
          if (circleIsOverCircle(*cellTemp, cell)) {
            return true;
          }
        }
      }
    }
    return false;
  }
  template <typename T>
  T remove_at(std::vector<T> &v, typename std::vector<T>::size_type n) {
    T ans = std::move_if_noexcept(v[n]);
    v[n] = std::move_if_noexcept(v.back());
    v.pop_back();
    return ans;
  }
  vector2d getCenterOfMassFromLiving() {
    float sumX = 0;
    float sumY = 0;
    int counter = 0;
    for (const Cell *cell : aliveCells) {
      counter++;
      sumX += cell->getX();
      sumY += cell->getY();
    }
    return vector2d(sumX / counter, sumY / counter);
  }
  int getAliveCellsCounter() const { return aliveCellsCounter; }
  void update(int numberOfIteration) {
    float halfOfThicknessTrue = halfOfThickness - 0.5;
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i = 0; i < numberOfIteration; i++) {
      auto combOfAngles = combinationsAngles;
      auto numOfPairs = combOfAngles.size();
      bool isAdded = false;
      iterationCounter++;
      unsigned long index = gen() % aliveCells.size();
      Cell *cell = aliveCells[index];
      if (cell->getStatus()) {
        bool isConflicting;
        std::shuffle(angles.begin(), angles.end(), gen);
        auto theta = angles;
        std::shuffle(angles.begin(), angles.end(), gen);
        auto phi = angles;
        for (const angle &angle1 : theta) {
          z = cell->getZ() + angle1.sin;
          isConflicting = z > halfOfThicknessTrue || z < -halfOfThicknessTrue;
          if (!isConflicting) {
            for (const angle &angle2 : phi) {
              x = cell->getX() + angle1.cos * angle2.cos;
              y = cell->getY() + angle1.cos * angle2.sin;
              isConflicting = cellIsConflicting(Cell(x, y, z));
              if (!isConflicting) {
                addCell(x, y, z);
                break;
              }
            }
          }
        }
        if (isConflicting) {
          deadCell(cell);
        }
      }
      if (iterationCounter % densityOfMeasurements == 0) {
        // currentMeanRadius = getMeanRadiusOfLivingCells();
        // if (currentMeanRadius > lastMeanRadius) {
        //   lastMeanRadius = currentMeanRadius;
        //   saveToFileMeanRadiusOfLivingCellsNumOfLivingCells("MRNLC"+std::to_string(thickness).substr(0,4)+
        //   "Id" + std::to_string(id) + ".csv");
        // saveToFileSDMean("SD" + std::to_string(thickness).substr(0, 4) + "Id" +
        //                  std::to_string(id) + ".csv");
        // }
        // saveToFileMeanRadiusOfLivingCellsNumOfAllCells(
        //     "TrueDensity" + std::to_string(thickness).substr(0, 4) + "Id" +
        //     std::to_string(id) + ".csv");

      }
    }
  }
  static float dstBtw2Pts(const Cell c1, const Cell c2) {
    return sqrt(pow(c1.getX() - c2.getX(), 2) + pow(c1.getY() - c2.getY(), 2) +
                pow(c1.getZ() - c2.getZ(), 2));
  }
  static float dstBtw2Pts(const vector2d v, const Cell c) {
    return sqrt(pow(v.x - c.getX(), 2) + pow(v.y - c.getY(), 2));
  }
  static float dstBtw2Pts(const vector2d v, const Cell *c) {
    return sqrt(pow(v.x - c->getX(), 2) + pow(v.y - c->getY(), 2));
  }
  float getMeanRadiusOfLivingCells() {
    int counter = 0;
    float sum = 0;
    const vector2d center = getCenterOfMassFromLiving();
    for (const Cell *cell : aliveCells) {
      counter++;
      sum += dstBtw2Pts(center, cell);
    }
    return sum / counter;
  }
  int getNumberOfCellsEnclosedByRadius(float radius) {
    int counter = 0;
    vector2d center = getCenterOfMassFromLiving();
    for (const Cell cell : cells) {
      if (dstBtw2Pts(center, cell) <= radius) {
        counter++;
      }
    }
    return counter;
  }
  void saveToFileAllNumberOfCellsEnclosedByRadius(const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath);
    float R = getMeanRadiusOfLivingCells() + 10;
    float step = 1;
    for (float r = step; r <= R; r += step) {
      fout << std::pow(r / cellRadius, 2) << "\t"
           << getNumberOfCellsEnclosedByRadius(r) << "\n";
    }
    fout.close();
  }
  void saveToFile(const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath);
    for (auto const cell : cells) {
      fout << cell.getX() * 10 << "\t" << cell.getY() * 10 << "\t"
           << cell.getZ() * 10 << "\t";
      if (cell.getStatus()) {
        fout << "1";
      } else {
        fout << "0";
      }
      fout << "\n";
    }
    fout.close();
  }
  void
  saveToFileMeanRadiusOfLivingCellsNumOfAllCells(const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath, std::ios_base::app);
    fout << /*2 * */ getMeanRadiusOfLivingCells() / cellRadius << "\t"
         << cells.size() << "\n"; // poprawka błąd w pracy USUNIETA
    fout.close();
  }
  void saveToFileDensity(int j, const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath, std::ios_base::app);
    fout << /*2 * */ getMeanRadiusOfLivingCells() / cellRadius << "\t"
    << getHeight() / cellRadius << "\t"
         << cells.size() <<"\t" <<j<<"\n"; // poprawka błąd w pracy USUNIETA
    fout.close();
  }
  void saveToFileMeanRadiusOfLivingCellsNumOfLivingCells(
      const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath, std::ios_base::app);
    fout << /*2 * */ getMeanRadiusOfLivingCells() / cellRadius << "\t"
         << aliveCellsCounter << "\n"; // poprawka błąd w pracy USUNIETA
    fout.close();
  }
  float getSD() {
    float meanRadius = getMeanRadiusOfLivingCells();
    const vector2d center = getCenterOfMassFromLiving();
    std::vector<float> deviations;
    for (const Cell *cell : aliveCells) {
      deviations.push_back(pow(meanRadius - dstBtw2Pts(center, cell), 2));
    }
    float variance = 0;
    for (float v : deviations) {
      variance += v;
    }
    variance /= deviations.size();
    return sqrt(variance) / cellRadius;
  }
  void saveToFileSDMean(const std::string &fileName) {
    std::ofstream fout;
    std::string fullPath = fileName;
    fout.open(fullPath, std::ios_base::app);
    fout << getMeanRadiusOfLivingCells() / cellRadius << "\t" << getSD()
         << "\n";
    fout.close();
  }
};
