#ifndef PARTICLE_H
#include "particles.h"
#define PARTICLE_H
#endif

double dt = (double)1/TARGET_FPS;
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

bool is_empty(System *s){
    if(s->head == NULL){
        return 1;
    }
    return 0;
}

void remove_particle(System *s){
    Particles *remove = s->head;
    s->head = s->head->next;
    free(remove);
    if(s->head == NULL){
        s->tail = NULL;
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

double kinect_energy(System *s){
    double total_energy = 0;
    for(Particles *p = s->head; p != NULL; p = p->next){
        double vel = sqrt(p->p_vel.x*p->p_vel.x + p->p_vel.y*p->p_vel.y);
        total_energy += vel*p->mass/2;
    }
    return total_energy;
}

void resolve_collision(Particles *p1, Particles *p2, double nx, double ny) {
    //Reference: https://physics.stackexchange.com/questions/681396/elastic-collision-3d-eqaution/681419#681419
    double dvx = p1->p_vel.x - p2->p_vel.x;
    double dvy = p1->p_vel.y - p2->p_vel.y;
    double dot_product = (nx * dvx) + (ny * dvy);
    if (dot_product <= 0) return;
    double mass_sum = p1->mass + p2->mass;
    double factor_p1 = (2.0 * p2->mass / mass_sum) * dot_product;
    p1->p_vel.x -= factor_p1 * nx;
    p1->p_vel.y -= factor_p1 * ny;
    double factor_p2 = (2.0 * p1->mass / mass_sum) * dot_product;
    p2->p_vel.x += factor_p2 * nx;
    p2->p_vel.y += factor_p2 * ny;
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
            double dist_x = p2->p_pos.x - p1->p_pos.x;
            double dist_y = p2->p_pos.y - p1->p_pos.y;
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
                p1->p_pos.x -= norm_x * (overlap * 0.5);
                p1->p_pos.y -= norm_y * (overlap * 0.5);
                p2->p_pos.x += norm_x * (overlap * 0.5);
                p2->p_pos.y += norm_y * (overlap * 0.5);

                // Update velocities
                resolve_collision(p1,p2, norm_x, norm_y);
            }
        }
    }
}
