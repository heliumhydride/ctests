#include <iostream>
#include <cmath>

using namespace std;

class vertex {
  public:
    double x, y, z;
    
    vertex(double xpos, double ypos, double zpos) {
      x = xpos;
      y = ypos;
      z = zpos;
    }

    void display() {
      printf("(%lf, %lf, %lf)", this->x, this->y, this->z);
    }
};

class rotation_matrix {
  public:
    double row1[3];
    double row2[3];
    double row3[3];

    rotation_matrix(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
      row1[0] = a;
      row1[1] = b;
      row1[2] = c;
      row2[0] = d;
      row2[1] = e;
      row2[2] = f;
      row3[0] = g;
      row3[1] = h;
      row3[2] = i;
    }

    vertex operator*(vertex &v) {
      return vertex(
        this->row1[0] * v.x + this->row1[1] * v.y + this->row1[2] * v.z,
        this->row2[0] * v.x + this->row2[1] * v.y + this->row2[2] * v.z,
        this->row3[0] * v.x + this->row3[1] * v.y + this->row3[2] * v.z
      );
    }

    void display() {
      printf("| %lf %lf %lf |\n", row1[0], row1[1], row1[2]);
      printf("| %lf %lf %lf |\n", row2[0], row2[1], row2[2]);
      printf("| %lf %lf %lf |\n", row3[0], row3[1], row3[2]);
    }
};

int main(void) {
  rotation_matrix A(cos(M_PI), 0, sin(M_PI), 0, 1, 0, -sin(M_PI), 0, cos(M_PI));
  vertex p(1, 1, 1);

  cout << "Rotation matrix A (around y axis):\n";
  A.display();
  cout << "\nVertex p:\n   ";
  p.display();

  cout << "\nVertex p rotated 90 degrees via matrix A:\n   ";
  (A*p).display();

  return 0;
}
