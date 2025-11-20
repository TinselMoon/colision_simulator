#ifndef PARTICLE
#include "particles.h"
#define PARTICLE
#endif

Color values[] = {DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE,
                    DARKBROWN, GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN,
                    LIGHTGRAY, PINK, YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE};

System* create () {
    System *l = (System *)malloc(sizeof(System));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void insert_particle(System *s, int screenWidth, int screenHeight){
    Particles *p = (Particles *)malloc(sizeof(Particles));
    p->next = NULL;
    p->radius = (rand() % (CIRCLE_RAD_MAX - CIRCLE_RAD_MIN))+CIRCLE_RAD_MIN;
    p->p_pos.x = rand() % (screenWidth - 2*p->radius) + p->radius;
    p->p_pos.y = rand() % (screenHeight - 2*p->radius) + p->radius;
    p->p_vel.x = rand() % MAX_SPEED;
    p->p_vel.y = rand() % MAX_SPEED;
    p->color = values[rand()%21];
    if(s->head == NULL){
        s->head = p;
        s->tail = p;
    }
    else{
        s->tail->next = p;
        s->tail = p;
    }
}

void destroy (System *l) {
    if (l != NULL) {	
        Particles *h = l->head;	
        int counter = 0;
        while (h != NULL) {
            counter++;
            Particles *t = h->next; 
            free (h); 
            h = t;
        }
    }
}

void draw_particles(System *s){
    for(Particles *l = s->head; l != NULL; l=l->next){
        DrawCircleV(l->p_pos, l->radius, l->color);
    }
}
