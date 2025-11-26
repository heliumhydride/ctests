#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <limits>
#include <math.h>
#include <assert.h>

long gcd(long n, long m) {
  while(n != m) {
    if(n > m)
      n -= m;
    else
      m -= n;
  }
  return n;
}

class Fraction {
  private:
    long numerateur, denominateur;

  public:
    Fraction(long a, long b) {
      assert(b > 0 && "!!! Le diviseur doit être strictement positif !!!");
      numerateur = a;
      denominateur = b;
    }

    // == (a/b)(c/d)
    Fraction operator*(Fraction &f) {
      return Fraction(this->numerateur * f.numerateur, this->denominateur * f.denominateur);
    }

    // == (a/b)/(c/d)
    Fraction operator/(Fraction &f) {
      return Fraction(this->numerateur * f.denominateur, this->denominateur * f.numerateur);
    }

    // == n(a/b)
    Fraction operator*(long n) {
      return Fraction(this->numerateur * n, this->denominateur);
    }

    // == (a/b)/n
    Fraction operator/(long n) {
      return Fraction(this->numerateur, this->denominateur * n);
    }

    // == (a/b)^(-1)
    Fraction inverse(void) {
      return Fraction(this->denominateur, this->numerateur);
    }

    // == -(a/b)
    Fraction operator-(void) {
      return Fraction(-(this->numerateur), this->denominateur);
    }

    operator float() {
      // techniquement faux, on devrait return undefined mais jsp comment faire :')
      if(this->denominateur == 0) {
        return std::numeric_limits<float>::infinity();
      }
      return this->numerateur / this->denominateur;
    }

    // == (a/b) + (c/d)
    Fraction operator+(Fraction &f) {
      long a,b,c,d;
      a = this->numerateur; b = this->denominateur;
      c = f.numerateur; d = f.denominateur;
      return Fraction(a*d + b*c, b*d).simplifie();
    }

    Fraction simplifie(void) {
      // cas particuliers
      if((this->denominateur < 0) && (this->numerateur >= 0)) {
        this->denominateur = -(this->denominateur);
        this->numerateur = -(this->numerateur);
      }

      return Fraction(
        (int)(this->numerateur   / gcd(labs(this->numerateur), labs(this->denominateur))),
        (int)(this->denominateur / gcd(labs(this->numerateur), labs(this->denominateur)))
      );
    }

    int operator==(Fraction &f) {
      long a,b,c,d;
      a = this->numerateur; b = this->denominateur;
      c = f.numerateur; d = f.denominateur;
      return (int)((a*d) == (b*c));
    }

    void affiche(void) {
      printf("%ld/%ld", this->numerateur, this->denominateur);
    }
};



int main(void) {
  Fraction f1(1,2);
  Fraction f2(1,3);
  // (f1*f2).affiche();
  // (f1/(long)2).affiche();
  // Fraction(18,24).simplifie().affiche();
  // (f1+f2).affiche();
  // printf("\n%f\n", (float)Fraction(1,0));
  // Fraction(28,-4).simplifie().affiche();
  printf("%d\n", Fraction(1,3) == Fraction(2,4));
  return 0;
}
