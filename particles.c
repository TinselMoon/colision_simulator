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

void ResolveCollision(Particles *p1, Particles *p2) {
    // 1. Vetor r_ij (Vetor Normal: aponta do centro de p1 para p2)
    // r_ij = pos_j - pos_i
    double dx = p2->p_pos.x - p1->p_pos.x;
    double dy = p2->p_pos.y - p1->p_pos.y;
    
    double distSq = dx*dx + dy*dy;
    double dist = sqrt(distSq);

    // Segurança contra divisão por zero
    if (dist == 0) return;

    // 2. Vetor Unitário r_hat_ij (Normalizado)
    double nx = dx / dist;
    double ny = dy / dist;

    // 3. Vetor v_ij (Velocidade Relativa: v_i - v_j)
    double dvx = p1->p_vel.x - p2->p_vel.x;
    double dvy = p1->p_vel.y - p2->p_vel.y;

    // 4. Produto Escalar (r_hat_ij . v_ij)
    // Projeta a velocidade relativa na direção normal
    double dot_product = (nx * dvx) + (ny * dvy);

    // IMPORTANTE: Se o produto escalar for negativo ou zero, as partículas
    // já estão se afastando. Não devemos resolver a colisão novamente (evita "grudar").
    // Na nossa definição (p1->p2), se dot > 0 elas estão se aproximando.
    if (dot_product <= 0) return;

    // 5. Massas (Usando raio^2 como aproximação para massa em 2D)
    double mass_sum = p1->mass + p2->mass;

    // 6. Implementando a Equação para v'_i (p1)
    // v'_i = v_i - [2*mj / (mi + mj)] * (dot_product) * r_hat
    double factor_p1 = (2.0 * p2->mass / mass_sum) * dot_product;
    
    p1->p_vel.x -= factor_p1 * nx;
    p1->p_vel.y -= factor_p1 * ny;

    // 7. Implementando a Equação Simétrica para v'_j (p2)
    // Pela 3ª Lei de Newton, a força é igual e oposta, mas escalada pela massa de p1
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
                ResolveCollision(p1,p2);
            }
        }
    }
}
