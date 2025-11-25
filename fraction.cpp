#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

class Fraction {
  private:
    long numerateur, denominateur;

  public:
    Fraction(long a, long b) {
      numerateur = a;
      denominateur = b;
    }

    Fraction operator*(Fraction &f) {
      return Fraction(this->numerateur * f.numerateur, this->denominateur * f.denominateur);
    }

    operator float() {
      return this->numerateur / this->denominateur;
    }

    /*
    Fraction operator+(Fraction &f) {
      return Fraction(...).simplifie()
    }

    Fraction simplifie(void) {
      return Fraction(...)
    }
    */

    void affiche(void) {
      printf("%ld/%ld", this->numerateur, this->denominateur);
    }
};

int main(void) {
  Fraction f1(1,2);
  Fraction f2(1,3);
  printf("%f", (float)f2);
  return 0;
}
