
#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
//#include <string.h>

/* PREFACE
 * I know there's a similar implementation of quaternions in c (https://github.com/MartinWeigel/Quaternion)
 * but I wanted to see if I could do one on my own from what I already knew
 */

/* Compile with:
 * 'cc -std=c99 -Wall -Wextra -Wpedantic quaternion_test.c'
 */

// TODO rotations

typedef struct Quaternion {
  double w;
  double v[3];
} Quaternion;

void Quaternion_init(double scalar, double x, double y, double z, Quaternion* q) {
  q->w = scalar;
  q->v[0] = x;
  q->v[1] = y;
  q->v[2] = z;
}

void Quaternion_add(Quaternion* q1, Quaternion* q2, Quaternion* qout) {
  qout->w = q1->w + q2->w;
  qout->v[0] = q1->v[0] + q2->v[0];
  qout->v[1] = q1->v[1] + q2->v[1];
  qout->v[2] = q1->v[2] + q2->v[2];
}

void Quaternion_multiply(Quaternion* q1, Quaternion* q2, Quaternion* qout) {
  /* Quaternion multiplication table
   * v*>| 1  i  j  k
   * ---|-----------
   *  1 | 1  i  j  k
   *  i | i -1  k -j
   *  j | j -k -1  i
   *  k | k  j -i -1
   */

  // get coefficients for readability
  double a,b,c,d,e,f,g,h;
  a = q1->w; b = q1->v[0]; c = q1->v[1]; d = q1->v[2];
  e = q2->w; f = q2->v[0]; g = q2->v[1]; h = q2->v[2];

  // distributed (a+bi+cj+dk)(e+fi+gj+hk), rearranged to order real, i, j and k parts
  qout->w    = a*e - b*f - c*g - d*h;
  qout->v[0] = a*f + b*e + c*h - d*g;
  qout->v[1] = a*g - b*h + c*e + d*f;
  qout->v[2] = a*h + b*g - c*f + d*e;
}

void Quaternion_conj(Quaternion* q1, Quaternion* qstar) {
  // https://en.wikipedia.org/wiki/Quaternion#Conjugation,_the_norm,_and_reciprocal
  // q* = a - bi - cj - dk
  qstar->w = q1->w;
  qstar->v[0] = -(q1->v[0]);
  qstar->v[1] = -(q1->v[1]);
  qstar->v[2] = -(q1->v[2]);
}

void Quaternion_print(Quaternion* q) {
  printf("%.lf", q->w);
  printf("%+.lfi", q->v[0]);
  printf("%+.lfj", q->v[1]);
  printf("%+.lfk", q->v[2]);
  printf("\n");
}

//int main(int argc, char* argv[]) {
int main(void) {
  Quaternion q1, q2;

  /*
  Quaternion_init(1,0,1,0, &q1); // q1 = 1+j
  Quaternion_init(0,1,0,2, &q2); // q2 = i+2k
  */

  Quaternion_init(2,3,-1,4, &q1); // q1 = 2+3i-j+4k
  Quaternion_init(1,-2,3,-1, &q2); // q2 = 1-2i+3j-1

  printf("q1 = "); Quaternion_print(&q1);
  printf("q2 = "); Quaternion_print(&q2);
  printf("\n");

  Quaternion qout;
  Quaternion_add(&q1, &q2, &qout); // qout = 1+i+j+2k
  printf("q1 + q2 = "); Quaternion_print(&qout);
  printf("\n");

  Quaternion_multiply(&q1, &q2, &qout); // qout = 3i+k
  printf("q1q2 = "); Quaternion_print(&qout);
  printf("\n");

  Quaternion_multiply(&q2, &q1, &qout);
  printf("q2q1 = "); Quaternion_print(&qout); // qout = -i+3k
  printf("\n");

  Quaternion_conj(&q1, &qout); // qout = 
  printf("q1* = "); Quaternion_print(&qout);

  return 0;
}
