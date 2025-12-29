#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <math.h>

typedef struct {
  // ax²+bx+c
  float a, b, c;
} Polyn2;

char get_signf(float a) { // return '-', '+', 0(NUL char, not '0')
  if(a > 0) return '+';
  if(a < 0) return '-';
  return 0;
}

void Polyn2_init(Polyn2* p, float a, float b, float c) {
  p->a = a;
  p->b = b;
  p->c = c;
}

void Polyn2_scale(Polyn2* p, float s) {
  p->a = p->a * s;
  p->b = p->b * s;
  p->c = p->c * s;
}

void Polyn2_add(Polyn2* pout, Polyn2* p1, Polyn2* p2) {
  Polyn2_init(pout, p1->a + p2->a, p1->b + p2->b, p1->c + p2->c);
}

void Polyn2_disp(Polyn2* p) {
  if(p->a < 0) (void)printf("-");
  else if(p->a != 1) (void)printf("%.2f", p->a);
  (void)printf("x²");
  if(fabsf(p->b) == 1.0f) (void)printf("%c", get_signf(p->b));
  else (void)printf("%+.2f", p->b);
  (void)printf("x");
  (void)printf("%+.2f\n", p->c);
}

int main(void) {
  Polyn2 p1, p2, pout;
  Polyn2_init(&p1, 1.0, 1, 1);
  Polyn2_init(&p2, 1.0, -0.5, -1);
  (void)printf("p1 = "); Polyn2_disp(&p1);
  (void)printf("p1 = "); Polyn2_disp(&p2);

  Polyn2_add(&pout, &p1, &p2);
  (void)printf("p1+p2 = "); Polyn2_disp(&pout);
  return 0;
}
