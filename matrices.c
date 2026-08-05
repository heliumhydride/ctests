#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
  size_t rows, cols;
  double* data;
} matrix_t;

#define MATRIX_FLOAT_FMT "%g"
#define MATRIX_SAMESIZE(M1,M2) (((M1)->rows == (M2)->rows) && ((M1)->cols == (M2)->cols))

void matrix_init(matrix_t* M, size_t rows, size_t cols) {
  if(rows <= 0 || cols <= 0) return;
  M->rows = rows;
  M->cols = cols;
  M->data = malloc(sizeof(double)*rows*cols);
  memset(M->data, 0, sizeof(double)*rows*cols);
}

void matrix_destroy(matrix_t* M) {
  if(M->data != NULL)
    free(M->data);
  M->data = NULL;
}

double matrix_get(matrix_t* M, size_t i, size_t j) {
  if((i > M->rows) || (j > M->cols)) return NAN;
  return M->data[i + j * M->cols];
}

void matrix_set(matrix_t* M, size_t i, size_t j, double x) {
  if((i > M->rows) || (j > M->cols)) return;
  M->data[i + j * M->cols] = x;
}

void matrix_identity(matrix_t* M) {
  if(M->rows != M->cols) return;
  for(size_t i = 0; i < M->rows; i++) {
    matrix_set(M, i, i, 1.0f);
  }
}

void matrix_display(matrix_t* M) {
  for(size_t i = 0; i < M->rows; i++) {
    printf("{");
    for(size_t j = 0; j < M->cols; j++)
      printf(MATRIX_FLOAT_FMT", ", matrix_get(M, i, j));
    printf("},\n");
  }
}

void matrix_add(matrix_t* M_out, matrix_t* M1, matrix_t* M2) {
  if(!MATRIX_SAMESIZE(M1,M2)) return;
  if(!MATRIX_SAMESIZE(M1,M_out)) return;
  for(size_t i = 0; i < M1->rows; i++) {
    for(size_t j = 0; j < M1->cols; j++)
      matrix_set(M_out, i, j, matrix_get(M1, i, j) + matrix_get(M2, i, j));
  }
}

void matrix_scale(matrix_t* M, float x) {
  for(size_t i = 0; i < M->rows; i++) {
    for(size_t j = 0; j < M->cols; j++)
      matrix_set(M, i, j, matrix_get(M,i,j) * x);
  }
}

int main(void) {
  matrix_t M1, M2, M_buf;
  matrix_init(&M1, 2, 3);
  matrix_init(&M2, 2, 3);
  matrix_init(&M_buf, 2, 3);

  matrix_set(&M1, 0, 0, 1.0);
  matrix_set(&M1, 0, 1, 2.0);
  matrix_set(&M1, 0, 2, 3.0);
  matrix_set(&M1, 1, 0, 4.0);
  matrix_set(&M1, 1, 1, 5.0);
  matrix_set(&M1, 1, 2, 6.0);

  for(size_t i = 0; i < M2.rows; i++) {
    for(size_t j = 0; j < M2.cols; j++) {
      matrix_set(&M2, i, j, 1.0);
    }
  }

  printf("M1:\n");
  matrix_display(&M1);
  printf("M2:\n");
  matrix_display(&M2);

  matrix_add(&M_buf, &M1, &M2);
  printf("M1+M2:\n");
  matrix_display(&M_buf);

  matrix_scale(&M_buf, .5);
  printf(".5(M1+M2):\n");
  matrix_display(&M_buf);

  matrix_destroy(&M1);
  matrix_destroy(&M2);
  matrix_destroy(&M_buf);
  // ^^^^ TODO memory issues
  return 0;
}
