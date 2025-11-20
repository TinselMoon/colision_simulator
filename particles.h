#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CIRCLE_RAD_MAX 100
#define CIRCLE_RAD_MIN 40
#define MAX_SPEED 501

typedef struct particle{
    Vector2 p_pos;
    Vector2 p_vel;
    Color color;
    int radius;
    struct particle *next;
} Particles;

typedef struct{
    Particles *head;
    Particles *tail;
} System;

System* create();
void insert_particle(System *s, int screenWidth, int screenHeight);
void destroy (System *l);
void draw_particles(System *s);
