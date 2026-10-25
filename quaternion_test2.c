#include <stdio.h>
#include <math.h>

// no functions used (except main) by the way. absolute coding

typedef struct {
  double x,y,z,w;
} quaternion;

#define __SQUARE(x) (x)*(x)

#define QUAT_FMT "%lg%+lgi%+lgj%+lgk"
#define QUAT_ARG(q) (q).x, (q).y, (q).z, (q).w

#define QUAT_NEGATE(q)  \
  (quaternion) {           \
    .x = -(q).x,           \
    .y = -(q).y,           \
    .z = -(q).z,           \
    .w = -(q).w            \
  }

#define QUAT_CONJUGATE(q)  \
  (quaternion) {           \
    .x = (q).x,            \
    .y = -(q).y,           \
    .z = -(q).z,           \
    .w = -(q).w            \
  }

#define QUAT_ADD(q1,q2) \
  (quaternion) {           \
    .x = (q1).x + (q2).x,  \
    .y = (q1).y + (q2).y,  \
    .z = (q1).z + (q2).z,  \
    .w = (q1).w + (q2).w   \
  }

#define QUAT_SUB(q1,q2) QUAT_ADD((q1), QUAT_NEGATE((q2)))

/* Quaternion multiplication table
 * v*>| 1  i  j  k
 * ---|-----------
 *  1 | 1  i  j  k
 *  i | i -1  k -j
 *  j | j -k -1  i
 *  k | k  j -i -1
 */
#define QUAT_MULT(q1, q2) \
  (quaternion) {          \
    .x = (q1).x * (q2).x - (q1).y * (q2).y - (q1).z * (q2).z - (q1).w * (q2).w, \
    .y = (q1).x * (q2).y + (q1).y * (q2).x + (q1).z * (q2).w - (q1).w * (q2).z, \
    .z = (q1).x * (q2).z - (q1).y * (q2).w + (q1).z * (q2).x + (q1).w * (q2).y, \
    .w = (q1).x * (q2).w + (q1).y * (q2).z - (q1).z * (q2).y + (q1).w * (q2).x  \
  }

#define QUAT_MAGNITUDE(q) sqrt(QUAT_MULT((q), (QUAT_CONJUGATE((q)))).x)

#define QUAT_DIV_SCALAR(q,a) \
  (quaternion) {   \
    .x = (q).x / (a), \
    .y = (q).y / (a), \
    .z = (q).z / (a), \
    .w = (q).w / (a)  \
  }

#define QUAT_INVERSE(q) QUAT_DIV_SCALAR(QUAT_CONJUGATE((q)), __SQUARE(QUAT_MAGNITUDE((q))))
#define QUAT_DIV(q1,q2) QUAT_MULT((q1), QUAT_INVERSE((q2)))


int main(void) {
  quaternion q1 = {1,0,1,0};
  quaternion q2 = {2,2,0,3};

  printf("q_1 = "QUAT_FMT"\n", QUAT_ARG(q1));
  printf("q_2 = "QUAT_FMT"\n", QUAT_ARG(q2));
  printf("q_1 * q_2 = "QUAT_FMT"\n", QUAT_ARG(QUAT_MULT(q1, q2)));
  printf("q_2 * q_1 = "QUAT_FMT"\n", QUAT_ARG(QUAT_MULT(q2, q1)));
  printf("|q| = %lg\n", QUAT_MAGNITUDE(q1)); 
  printf("q_1 / q_2 = "QUAT_FMT"\n", QUAT_ARG(QUAT_DIV(q1,q2)));
  return 0;
}
