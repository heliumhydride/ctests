#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
//#include <stdlib.h>

#ifndef MAX_LABEL_SIZE
#define MAX_LABEL_SIZE 64
#endif

#ifndef M_PI // shouldn't be needed but just in case
#define M_PI 3.141592653589793
#endif

// Speed of light in a vaccum
#define CELERITY 299792458.0f // m/s

#ifndef TICK
#define TICK 0.1 // s
#endif

/* Compile with:
 * '$ cc -std=c99 -Wall -Wextra -Wpedantic particle_sim.c -lm'
 * optionally use the -DMAX_LABEL_SIZE=... flag to define the maximum characters for particle labels
 * optionally use -DTICK=... to change the duration of a tick
 */

typedef struct particle {
  char label[MAX_LABEL_SIZE];
  double position[2]; // positions {x,y} (m)
  double velocity; // speed (m·s^-1)
  double angle; // angle of direction (rad)
  double radius; // m
  double mass; // kg
} particle;

void init_particle(particle* p, char* label, double x, double y, double v, double theta, double r, double m) {
  if(strlen(label) >= MAX_LABEL_SIZE) {
    memcpy(p->label, label, MAX_LABEL_SIZE);
    p->label[MAX_LABEL_SIZE-1] = '\0'; // ensure string is terminated after being cut, prevents garbeled output from printf
  } else
    memcpy(p->label, label, strlen(label)+1);

  p->position[0] = x;
  p->position[1] = y;
  p->velocity = v;
  p->angle = theta;
  p->radius = r;
  p->mass = m;
}

void add_to_particle_data(particle* p, double delta_x, double delta_y, double delta_v, double delta_theta, double delta_r, double delta_m) {
  p->position[0] += delta_x;
  p->position[1] += delta_y;
  p->velocity += delta_v; // useful!
  p->angle += delta_theta;
  p->radius += delta_r;
  p->mass += delta_m;
}

void scale_particle_data(particle* p, double scale_x, double scale_y, double scale_v, double scale_theta, double scale_r, double scale_m) {
  p->position[0] *= scale_x;
  p->position[1] *= scale_y;
  p->velocity *= scale_v; // useful!
  p->angle *= scale_theta;
  p->radius *= scale_r;
  p->mass *= scale_m;
}

int collision(particle* p1, particle* p2) {
  // TODO workout a proper collision system by taking the radius in account
  if(((int)p1->position[0] == (int)p2->position[0]) &&
     ((int)p1->position[1] == (int)p2->position[1]))
    return 1;
  return 0;
}

double get_energy_of_particle(particle* p) {
  // TODO get energy of particle from mass and velocity
  return p->mass * (pow(CELERITY, 2)); // good old E=m·c², but implies no velocity
}

void update_particle(particle* p) {
  // get horizontal and vertical speeds
  double delta_x = p->velocity * cos(p->angle);
  double delta_y = p->velocity * sin(p->angle);

  add_to_particle_data(p, delta_x, delta_y, 0, 0, 0, 0); // update x and y positions
}

void print_particle_data(particle* p) {
  printf("particle \"%s\":\n", p->label);
  printf("  x position: %.3lf m\n", p->position[0]);
  printf("  y position: %.3lf m\n", p->position[1]);
  printf("  velocity: %.3lf m/s\n", p->velocity);
  printf("  angle of velocity: %.3lf radians\n", p->angle);
  printf("  radius: %.3lf m\n", p->radius);
  printf("  mass: %.3lf kg\n", p->mass);
  printf("  energy: %.3le J\n", get_energy_of_particle(p));
}


void clear(void) {
  printf("\033[2J\033[H"); // clear screen and return to home row & column
}

//int main(int argc, char* argv[]) {
int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0); // stop stdout from buffering, useful when running while(1) loops and printf's

  particle p1,p2;
  init_particle(&p1, "Particle 1", -20, 0, 1, 0, 0.01, 0.001);
  //init_particle(&p2, "Particle 2 (has a very long label of more than 32 characters for testing purposes)", 0, 20, 1, -M_PI/2, 1, 1);
  init_particle(&p2, "Particle 2", 0, 20, 1, -M_PI/2, 0.01, 0.001);

  double time = 0;
  while(1) {
    //printf("-----------------------------\n");
    printf("time: %.3lf s\n", time);
    print_particle_data(&p1);
    print_particle_data(&p2);
    update_particle(&p1);
    update_particle(&p2);
    //scale_particle_data(&p2, 1, 1, 0.99, 1, 1, 1); // particle 2 will decelerate by 0.01 m/s²

    if(collision(&p1, &p2)) {
      printf("collision detected between \"%s\" and \"%s\"\n", p1.label, p2.label);
      usleep(1000000);
    }

    usleep((int)(TICK*1000000)); // sleep for TICK seconds
    time += TICK;
    clear();
  }
  return 0;
}
