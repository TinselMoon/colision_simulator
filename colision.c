#include "particles.h"

int main(int argc, char **argv){
    if(argc < 4){
        printf("Error, not enough information\nRun: %s [width] [height] [num of particles]\n", argv[0]);
        exit(0);
    }
    srand(time(NULL));
    const int screenWidth = atoi(argv[1]);
    const int screenHeight = atoi(argv[2]);
    System *s = create();
    int num_particles = atoi(argv[3]);
    for(int i = 0; i < num_particles; i++)
        insert_particle(s, screenWidth, screenHeight);

    InitWindow(screenWidth, screenHeight, "colisions simulator");
    SetTargetFPS(TARGET_FPS);               // Set our game to run at 60 frames-per-second
    double fps;
    ClearBackground(RAYWHITE);
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (IsKeyPressed(KEY_I)){
            insert_particle(s, screenWidth, screenHeight);
            num_particles++;
        }
        BeginDrawing();
        if(!is_empty(s)){
            ClearBackground(RAYWHITE);
            if (IsKeyPressed(KEY_R)){
                remove_particle(s);
                num_particles--;
            }
            update_pos(s, screenWidth, screenHeight);
            for(int i = 0; i < 4; i++)fix_contacts(s, screenWidth, screenHeight);
            draw_particles(s);
        }
        fps = 1/GetFrameTime();
        DrawText(TextFormat("Number of Particles: %d\n"
                            "Total kinetic energy: %.0f\n"
                            "Press I to insert a particle\n"
                            "Press R to remove a particle\n"
                            "FPS: %.2f", num_particles, kinetic_energy(s), fps), 10, 10, 30, DARKGRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();        // Close window and OpenGL context
    destroy(s);
    return 0;
}
