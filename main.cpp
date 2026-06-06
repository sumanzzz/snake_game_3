#include <iostream>
#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 450;

const int ROWS = 20;
const int COLS = 20;

float foodRow, foodCol;
void generateFood() {
    foodRow = rand() % ROWS;
    foodCol = rand() % COLS;

}
void grid() {
    for (int i = 0; i <= 20; i++) {
        DrawLine3D(
            { (float)i, 0.01f, 0.0f },
            { (float)i, 0.01f, 20.0f },
            BLACK
        );

        DrawLine3D(
            { 0.0f, 0.01f, (float)i },
            { 20.0f, 0.01f, (float)i },
            BLACK
        );
    }
}
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
Direction dir;

int main()
{
    
       
    

    InitWindow(screenWidth, screenHeight, "snake game");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = { -10.0f, 25.0f, 10.0f };
    camera.target = { 10.0f, 0.0f, 10.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };// Camera up vector (rotation towards target)
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
    
    double lastMoveTime = GetTime();
    // Main game loop
    generateFood();
    bool gameOver = false;
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        /*UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };*/
        //----------------------------------------------------------------------------------
        
        if (IsKeyPressed(KEY_W) && dir != DOWN) dir = UP;
        if (IsKeyPressed(KEY_S) && dir != UP) dir = DOWN;
        if (IsKeyPressed(KEY_A) && dir != RIGHT) dir = LEFT;
        if (IsKeyPressed(KEY_D) && dir != LEFT) dir = RIGHT;
        
        if (GetTime() - lastMoveTime > 0.1) {
            float newRow = snake.back().first;
            float newCol = snake.back().second ;
           
            switch (dir)
            {
            case UP: newRow++;
                break;
            case DOWN:newRow--;
                break;
            case LEFT:newCol--;
                break;
            case RIGHT:newCol++;
                break;
            default:
                break;
            }
            if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
                gameOver = true;
                
            
            }
            for (auto segment : snake) {
                if (segment.first == newRow && segment.second == newCol) {
                    gameOver = true;
                }
            }
            if (!gameOver) {
                if (newRow == foodRow && newCol == foodCol) {
                    snake.push_back({ newRow , newCol });
                    generateFood();
                }
                else {
                    snake.push_back({ newRow , newCol });
                    snake.erase(snake.begin());
                }
            }
            
            
            lastMoveTime = GetTime();
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

        DrawPlane({ 10.0f , 0.0f , 10.0f }, { 20.f , 20.f }, DARKGREEN);
        grid();
        DrawCube({ foodRow + 0.5f , 0.5f , foodCol + 0.5f }, 1.0f, 0.3f, 1.0f, YELLOW);
        for (auto segment : snake) {
            DrawCube({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, RED);
        }
        for (auto segment : snake) {
            DrawCubeWires({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, WHITE);
        }
        

        EndMode3D();
        if (gameOver) {
            DrawText("GAME OVER", 300, 300, 40, RED);
        }

        

        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
