#ifndef PARTICLE
#include "particles.h"
#define PARTICLE
#endif

double dt = (double)1/60;
Color values[] = {DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE,
                    DARKBROWN, GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN,
                    LIGHTGRAY, PINK, YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE};

int num_colors = sizeof(values)/sizeof(values[0]);

System* create () {
    System *l = (System *)malloc(sizeof(System));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void update_pos(System *s, int screenWidth, int screenHeight){
    for(Particles *p = s->head; p != NULL; p = p->next){
        p->p_pos.x += p->p_vel.x*dt;
        p->p_pos.y += p->p_vel.y*dt;
    }
}

void insert_particle(System *s, int screenWidth, int screenHeight){
    Particles *p = (Particles *)malloc(sizeof(Particles));
    p->next = NULL;
    p->radius = (rand() % (CIRCLE_RAD_MAX - CIRCLE_RAD_MIN))+CIRCLE_RAD_MIN;
    p->p_pos.x = rand() % (screenWidth - 2*p->radius) + p->radius;
    p->p_pos.y = rand() % (screenHeight - 2*p->radius) + p->radius;
    p->p_vel.x = (-MAX_SPEED/2) + rand() % MAX_SPEED;
    p->p_vel.y = (-MAX_SPEED/2) + rand() % MAX_SPEED;
    p->color = values[rand()%num_colors];
    p->mass = M_PI*p->radius*p->radius; // Multiply by a mass constant
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
        while (h != NULL) {
            Particles *t = h->next; 
            free (h); 
            h = t;
        }
    }
    free(l);
}

void draw_particles(System *s){
    for(Particles *l = s->head; l != NULL; l=l->next){
        DrawCircleV(l->p_pos, l->radius, l->color);
    }
}

void fix_contacts(System *s, int screenWidth, int screenHeight){
    for(Particles *p1 = s->head; p1 != NULL; p1 = p1->next){
        if((p1->p_pos.x+p1->radius) > screenWidth){
            p1->p_pos.x = screenWidth - p1->radius;
            p1->p_vel.x = -(p1->p_vel.x);
        }
        else if((p1->p_pos.x-p1->radius) < 0){
            p1->p_pos.x = 0 + p1->radius;
            p1->p_vel.x = -(p1->p_vel.x);
        }
        if((p1->p_pos.y+p1->radius) > screenHeight){
            p1->p_pos.y = screenHeight - p1->radius;
            p1->p_vel.y = -(p1->p_vel.y);
        }
        else if((p1->p_pos.y-p1->radius) < 0){
            p1->p_pos.y = 0 + p1->radius;
            p1->p_vel.y = -(p1->p_vel.y);
        }
        for(Particles *p2 = p1->next; p2 != NULL; p2 = p2->next){
            double dist_x = p1->p_pos.x - p2->p_pos.x;
            double dist_y = p1->p_pos.y - p2->p_pos.y;
            double sqr_dist = dist_x*dist_x + dist_y*dist_y;
            double sum_radius = p1->radius + p2->radius;
            if(sqr_dist < sum_radius*sum_radius){
                double dist = sqrt(sqr_dist);
                if(dist < 0.0001){
                    dist_x = 1.0;
                    dist_y = 0.0;
                    dist = 1.0;
                }
                double overlap = sum_radius - dist;
                double norm_x = dist_x/dist;
                double norm_y = dist_y/dist;
                p1->p_pos.x += norm_x * (overlap * 0.5);
                p1->p_pos.y += norm_y * (overlap * 0.5);
                p2->p_pos.x -= norm_x * (overlap * 0.5);
                p2->p_pos.y -= norm_y * (overlap * 0.5);

                // Update velocities
            }
        }
    }
}
