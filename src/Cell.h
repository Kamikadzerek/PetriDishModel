class Cell {
  private:
  bool status;
  float x;
  float y;
  float z;


  public:
  Cell(float X, float Y, float Z):x(X),y(Y),z(Z),status(true){}
  void death() {
    status = false;
  }
  Cell &operator=(Cell cell) {
    status = cell.getStatus();
    x = cell.getX();
    y = cell.getY();
    z = cell.getZ();
    return *this;
  }
  bool getStatus() const { return status; }
  float getX() const { return x; }
  float getY() const { return y; }
  float getZ() const { return z; }
};
