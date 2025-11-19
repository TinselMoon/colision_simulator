#include "particles.h"

int main(int argc, char **argv){
    if(argc < 2){
        printf("Error, no enough information about screen width and height\n");
        exit(0);
    }
    srand(time(NULL));
    const int screenWidth = atoi(argv[1]);
    const int screenHeight = atoi(argv[2]);
    System *s = create();
    for(int i = 0; i < 10; i++)
        insert_particle(s, screenWidth, screenHeight);
    
    InitWindow(screenWidth, screenHeight, "colisions simulator");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            //DrawCircleV(teste->p_pos, circleRad, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    destroy(s);
    return 0;
}
