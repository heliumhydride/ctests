/*  build cmd:
cc -Wall -Wextra -Wpedantic -std=c99 main.c -lm
*/

/* controls:

    arrow keys  shift view horizontally/vertically
    +/-         zoom in/out
    0           reset zoom

*/

#define FOREGROUND "\033[38;2;255;0;102m"
#define BACKGROUND "\033[48;2;30;30;30m"
#define FPS 60.0

#define _XOPEN_SOURCE 500 // usleep needs this 
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>

static struct winsize w;
#define WIDTH  w.ws_col
#define HEIGHT w.ws_row

static bool running = true;
void ctrl_c(int unused) {
  (void)unused;
  running = false;
}

typedef struct {
  double x,y;
} Vec2;

typedef struct {
  double x,y,z;
} Vec3;

const Vec3 vs[8] = {
  {.x = 0.25,  .y = 0.25,  .z = 0.25 },
  {.x = -0.25, .y = 0.25,  .z = 0.25 },
  {.x = -0.25, .y = -0.25, .z = 0.25 },
  {.x = 0.25,  .y = -0.25, .z = 0.25 },
  {.x = 0.25,  .y = 0.25,  .z = -0.25 },
  {.x = -0.25, .y = 0.25,  .z = -0.25 },
  {.x = -0.25, .y = -0.25, .z = -0.25 },
  {.x = 0.25,  .y = -0.25, .z = -0.25 }
};

const size_t fs[12][2] = {
  {0, 1}, {1, 2}, {3, 2}, {3, 0},
  {4, 5}, {5, 6}, {7, 6}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

void InitMap(char* map) {
  size_t i = 0;
  for(; i < WIDTH*HEIGHT; i++) {
    map[i] = ' ';
  }
  map[i] = '\0';
}

void Pixel(char* map, const Vec2 p, char c) {
  if(isnan(p.x) || isnan(p.y)) return;
  ssize_t x = (ssize_t)(p.x);
  ssize_t y = (ssize_t)(p.y);
  if(x < 0 || x > WIDTH) return;
  if(y < 0 || y > HEIGHT) return;
  map[y*WIDTH + x] = c;
}

void Line(char* map, const Vec2 a, const Vec2 b) {
  int x0 = (int)(a.x + 0.5);
  int y0 = (int)(a.y + 0.5);
  int x1 = (int)(b.x + 0.5);
  int y1 = (int)(b.y + 0.5);

  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;

  int err = dx - dy;
  for(;;) {
    if(x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT)
      map[y0 * WIDTH + x0] = '#';

    if(x0 == x1 && y0 == y1) break;
    int e2 = 2 * err;

    if(e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if(e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

Vec2 Screenable(Vec2 p) {
  Vec2 new = {
    .x = (p.x + 1)/2 * WIDTH,
    .y = (1 - (p.y + 1)/2) * HEIGHT
  };
  return new;
}

Vec2 Project(Vec3 in) {
  Vec2 out;
  if(in.z != 0) {
    out.x = (in.x) / (in.z); // x' = x/z
    out.y = (in.y) / (in.z); // y' = y/z
  } else {
    out.x = 10.0;
    out.y = 10.0; // make sure the point is not displayed
  }
  return out;
}

void Vertex(char* map, const Vec3 p) {
  Pixel(map, Screenable(Project(p)), '#');
}

Vec3 Translate(Vec3 p, Vec3 v) {
  Vec3 q = {
    .x = p.x + v.x,
    .y = p.y + v.y,
    .z = p.z + v.z,
  };
  return q;
}

Vec3 RotateXZ(Vec3 p, double theta) {
  const double c = cos(theta);
  const double s = sin(theta);
  Vec3 q = {
    .x = p.x * c - p.z * s,
    .y = p.y,
    .z = p.x * s + p.z * c
  };
  return q;
}

Vec3 RotateXY(Vec3 p, double theta) {
  const double c = cos(theta);
  const double s = sin(theta);
  Vec3 q = {
    .x = p.x * c - p.y * s,
    .y = p.x * s + p.y * c,
    .z = p.z,
  };
  return q;
}

#define VERTICES sizeof(vs)/sizeof(vs[0])
#define FACES    sizeof(fs)/sizeof(fs[0])

int main(void) {
  signal(SIGINT, ctrl_c);
  signal(SIGTERM, ctrl_c);

  setbuf(stdout, NULL);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  
  struct termios old_tio, new_tio;
  tcgetattr(STDOUT_FILENO, &old_tio);
  new_tio = old_tio;
  new_tio.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDOUT_FILENO, TCSANOW, &new_tio);

  struct pollfd fds[1];
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  char* map = malloc(WIDTH*HEIGHT + 1);
  if(map == NULL) return 2;
  InitMap(map);

  Vec3 V = {.x = 0, .y = 0, .z = 1.0};
  double dtheta = 0;
  const double dt = 1/FPS;
  printf("\033[1m" FOREGROUND BACKGROUND);
  while(running) {
    int polled = poll(fds, 1, 0);
    if(polled == -1) {
      running = false;
    } else if(polled == 0) {
      dtheta += M_PI*dt;
      /*
      for(size_t i = 0; i < 8; i++)
        Vertex(map, Translate(RotateXZ(vs[i], dtheta), V));
      */
      for(size_t i = 0; i < FACES; ++i) {
        int v1 = fs[i][0];
        int v2 = fs[i][1];
        Vec3 a = Translate(RotateXY(RotateXZ(vs[v1], dtheta), -dtheta), V);
        Vec3 b = Translate(RotateXY(RotateXZ(vs[v2], dtheta), -dtheta), V);
        Line(map, Screenable(Project(a)), Screenable(Project(b)));
      }

      printf("%s", map);
      usleep((useconds_t)(1e6/FPS)); // 1/60 s
      printf("\033[2J");
      InitMap(map); // clear map for next write
      fflush(stdout);
    } else {
      char buf[14]; // i can't think of any escape sequence >13 chars long
      (void)read(STDIN_FILENO, buf, sizeof(buf));

      switch(buf[0]) {
        case '\033':
          if(buf[1] == '[') {
            switch(buf[2]) {
              case 'A': // up
                V.y += 0.1;
                break;
              case 'B': // down
                V.y -= 0.1;
                break;
              case 'C': // left
                V.x += 0.1;
                break;
              case 'D': // right
                V.x -= 0.1;
                break;
            }
          }
          break;
        case '+':
          V.z -= 0.1;
          break;
        case '-':
          V.z += 0.1;
          break;
        case '0':
          V.x = 0.0;
          V.y = 0.0;
          V.z = 1.0;
          break;
      }
    }
  }
  printf("\033[0m\033[H\033[J");
  tcsetattr(STDOUT_FILENO, TCSANOW, &old_tio); // so we can type again after exiting
  free(map);
  return 0;
}
