#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct particle{
    Vector2 p_pos;
    Vector2 p_vel;
    Vector2 p_acc;
    struct particle *next;
} Particles;

int main(int argc, char **argv){
    if(argc < 2){
        printf("Error, no enough information about screen width and height\n");
        exit(0);
    }
    const int screenWidth = atoi(argv[1]);
    const int screenHeight = atoi(argv[2]);

    InitWindow(screenWidth, screenHeight, "colisions simulator");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    double g = 500.0f;
    double circleRad = 100.0f;
    double floorLimit = (double)screenHeight - circleRad;
    double dt = 1.0/20;
    Particles *teste = (Particles *)malloc(sizeof(Particles));
    teste->p_pos.x = (double)screenWidth/2;
    teste->p_pos.y = (double)screenHeight/2;
    teste->p_vel.x = 0.0f;
    teste->p_vel.y = 0.0f;
    teste->p_acc.y = 0.0f;
    teste->p_acc.x = 0.0f;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        /*teste->p_vel.y = teste->p_vel.y + g*dt;
        teste->p_pos.y = teste->p_pos.y + teste->p_vel.y*dt;
        if(teste->p_pos.y > floorLimit){
            double overshoot = teste->p_pos.y - floorLimit;
            teste->p_pos.y = screenHeight - overshoot;
            teste->p_vel.y = -(teste->p_vel.y);
        }*/
        double new_pos;
        double new_vel;
        double new_acc;
        new_pos = teste->p_pos.y + teste->p_vel.y *dt + (teste->p_acc.y)*(dt*dt*0.5);
        new_acc = g;
        new_vel = teste->p_vel.y + (new_acc+teste->p_acc.y)*(dt*0.5);
        teste->p_pos.y = new_pos;
        teste->p_vel.y = new_vel;
        teste->p_acc.y = new_acc;
        if(teste->p_pos.y >= screenHeight - circleRad){
            teste->p_vel.y = -(teste->p_vel.y);
            teste->p_pos.y = screenHeight - circleRad;
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            DrawCircleV(teste->p_pos, circleRad, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
