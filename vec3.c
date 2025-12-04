#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
  double x,y,z;
} vec3;

void vec3_init(double x, double y, double z, vec3* u) {
  u->x = x;
  u->y = y;
  u->z = z;
}

void vec3_copy(vec3* dest, vec3* src) {
  dest->x = src->x;
  dest->y = src->y;
  dest->z = src->z;
}

void vec3_disp(vec3* u) {
  (void)printf("(%lf, %lf, %lf)", u->x, u->y, u->z);
}

void vec3_add(vec3* u, vec3* v, vec3* dest) {
  dest->x = u->x + v->x;
  dest->y = u->y + v->y;
  dest->z = u->z + v->z;
}

void vec3_sub(vec3* u, vec3* v, vec3* dest) {
  dest->x = u->x - v->x;
  dest->y = u->y - v->y;
  dest->z = u->z - v->z;
}

void vec3_scale(vec3* u, double scalar, vec3* dest) {
  dest->x = u->x * scalar;
  dest->y = u->y * scalar;
  dest->z = u->z * scalar;
}

double vec3_dot(vec3* u, vec3* v) {
  double res = 0.0l;
  res += u->x * v->x;
  res += u->y * v->y;
  res += u->z * v->z;
  return res;
}

double vec3_mag(vec3* u) {
  return sqrt(
    pow(u->x, 2) +
    pow(u->y, 2) +
    pow(u->z, 2)
  );
}

void vec3_norm(vec3* u, vec3* dest) {
  double vn = vec3_mag(u);
  vec3_init(
    u->x / vn,
    u->y / vn,
    u->z / vn,
    dest
  );
}

bool vec3_iseq(vec3* u, vec3* v) {
  return (u->x == v->x) && (u->y == v->y) && (u->z == v->z);
}

void vec3_xprod(vec3* u, vec3* v, vec3* dest) {
  vec3_init(
    u->y * v->z - u->z * v->y,
    u->z * v->x - u->x * v->z,
    u->x * v->y - u->y * v->x,
    dest
  );
}
// a x b = ||a|| * ||b|| * sin(theta) n

double vec3_angle(vec3* u, vec3* v) {
  return acos(vec3_dot(u, v) / (vec3_mag(u) * vec3_mag(v)));
}

void vec3_project(vec3* u, vec3* v, vec3* dest) {
  double a = vec3_dot(u, v);
  double b = vec3_dot(v, v);
  vec3_scale(v, a/b, dest);
}

// non vector funcs
double to_deg(double theta) {
  return theta * 180 / M_PI;
}

int main(void) {
  vec3 u,v,w;
  vec3_init(1, 3, -5, &u);
  vec3_init(3, 2, -2, &v);
  vec3_project(&u, &v, &w);

  printf("u = "); vec3_disp(&u); printf("\n");
  printf("v = "); vec3_disp(&v); printf("\n");
  printf("proj u on v = "); vec3_disp(&w); printf("\n");

  return 0;
}
