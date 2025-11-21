#include "config.h"

typedef struct particle{
    Vector2 p_pos;
    Vector2 p_vel;
    Color color;
    int radius;
    double mass;
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
void update_pos(System *s, int screenWidth, int screenHeight);
void fix_contacts(System *s, int screenWidth, int screenHeight);
void resolve_collision(Particles *p1, Particles *p2);
void remove_particle(System *s);
bool is_empty(System *s);
