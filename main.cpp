#include <iostream>
#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 450;

const int ROWS = 25;
const int COLS = 25;

int main()
{
    
       
    

    InitWindow(screenWidth, screenHeight, "snake game");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { 0.0f,20.0f, 0.1f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 0.0f, -1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    std::vector<std::vector<int>> Board(ROWS, std::vector<int>(COLS, 0));
    std::vector<std::pair<float, float>> snake
    {
        {5,3},
        {5,4},
        {5,5}
    };
    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

        DrawPlane({ 0.0f , 0.0f , 0.0f }, { 20.f , 20.f }, DARKGREEN);
        DrawGrid(20, 1.0f);
        for (auto segment : snake) {
            DrawCube({ segment.first + 0.5f , 0.15f , segment.second + 0.5f}, 1.0f, 0.3f, 1.0f, RED);
        }
        

        EndMode3D();

        

        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
