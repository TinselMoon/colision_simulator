#ifndef PARTICLE
#include "particles.h"
#define PARTICLE
#endif

System* create () {
    System *l = (System *)malloc(sizeof(System));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void insert_particle(System *s, int screenWidth, int screenHeight){
    Particles *p = (Particles *)malloc(sizeof(Particles));
    p->next = NULL;
    p->p_pos.x = rand() % (screenWidth - 2*CIRCLE_RAD) + CIRCLE_RAD;
    p->p_pos.y = rand() % (screenHeight - 2*CIRCLE_RAD) + CIRCLE_RAD;
    p->p_vel.x = rand() % MAX_SPEED;
    p->p_vel.y = rand() % MAX_SPEED;
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
