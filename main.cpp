#include <iostream>
#include "raylib.h"
#include <vector>
#include <queue>
#include <algorithm>

//const int screenWidth = 800;
//const int screenHeight = 450;

const int screenWidth = 1200;
const int screenHeight = 1000;

const int ROWS = 20;
const int COLS = 20;

float foodRow, foodCol;

std::vector<std::vector<int>> Board(ROWS, std::vector<int>(COLS, 0));
std::vector<std::pair<float, float>> snake
{
    {5,3},
    {5,4},
    {5,5}
};
std::vector<std::pair<float, float>> reaperAI
{
    {10,3},
    {10,4},
    {10,5}
};
#pragma region AI
std::pair<int , int> reaperNextmove() {
    std::vector<std::vector<int>> Blocked(ROWS, std::vector<int>(COLS, 0));
    for (auto segment : snake) {
        Blocked[segment.first][segment.second] = 1;
    }
    for (int i = 0; i < reaperAI.size() - 1; i++) {
        Blocked[reaperAI[i].first][reaperAI[i].second] = 1;
    }

    // BFS
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<int>> visited(ROWS, std::vector<int>(COLS, 0));
    std::vector<std::vector<std::pair<int, int>>> parent(ROWS, std::vector <std::pair<int, int>>(COLS, { -1 , -1 }));

    auto head = reaperAI.back();
    q.push(head);
    visited[head.first][head.second] = 1;

    std::vector<int> neighborRow = { -1 , 1 ,0 , 0 };
    std::vector<int> neighborCol = { 0 , 0 ,-1 , 1 };

    while (!q.empty()) {
        auto [row, col] = q.front();
        q.pop();

        if (row == foodRow && col == foodCol) break;
        
        for (int i = 0; i < 4; i++) {
            int nr = row + neighborRow[i];
            int nc = col + neighborCol[i];

            if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) continue;

            if (Blocked[nr][nc] == 1) continue;

            if (visited[nr][nc] == 1) continue;

            visited[nr][nc] = 1;

            parent[nr][nc] = { row , col };

            q.push({ nr , nc });
        }
        
    }
    if (!visited[foodRow][foodCol]) {
        return reaperAI.back();
    }
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> current = { foodRow , foodCol };

    while (current != reaperAI.back()) {
        path.push_back(current);
        current = parent[current.first][current.second];
    }
    path.push_back(reaperAI.back());
    std::reverse(path.begin(), path.end());

    if (path.size() > 1) {
        return path[1];
    }

    return reaperAI.back();
}


#pragma endregion
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
Direction dir;




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

#pragma region MAIN
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
    
    
    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    double lastMoveTime = GetTime();
    // Main game loop
    generateFood();
    bool gameOver = false;
    int playerScore = 0, reaperScore = 0;
    std::string winner;
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
            float newCol = snake.back().second;
            
            auto reaperMove = reaperNextmove();
            int reaperRow = reaperMove.first;
            int reaperCol = reaperMove.second;
            if (!gameOver) {
                if (reaperRow == foodRow && reaperCol == foodCol) {
                    reaperAI.push_back({ reaperRow ,reaperCol });
                    reaperScore++;
                    generateFood();
                }
                else {
                    reaperAI.push_back({ reaperRow , reaperCol });
                    reaperAI.erase(reaperAI.begin());
                }
            }
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
            if (newRow < 0 || newRow >= ROWS || newCol <0 || newCol >= COLS) {
                gameOver = true;
                
            
            }
            for (auto segment : snake) {
                if (segment.first == newRow && segment.second == newCol) {
                    gameOver = true;
                }
            }
            for (auto segment : reaperAI) {
                if (segment.first == newRow && segment.second == newCol) {
                    gameOver = true;
                }
            }
            if (reaperScore == 20 || playerScore == 20) {
                gameOver = true;
            }
            if (!gameOver) {
                if (newRow == foodRow && newCol == foodCol) {
                    snake.push_back({ newRow , newCol });
                    playerScore++;
                    generateFood();
                }
                else {
                    snake.push_back({ newRow , newCol });
                    snake.erase(snake.begin());
                }
            }
            if (gameOver) {
                if (reaperScore > playerScore) {
                    winner = "Reaper";
                }
                else winner = "Player";
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
        DrawSphere({ foodRow + 0.5f , 0.5f , foodCol + 0.5f }, 0.5f, YELLOW);
        for (auto segment : snake) {
            DrawCube({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, RED);
        }
        for (auto segment : snake) {
            DrawCubeWires({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, WHITE);
        }
        for (auto segment : reaperAI) {
            DrawCube({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, BLUE);
        }
        for (auto segment : reaperAI) {
            DrawCubeWires({ segment.first + 0.5f , 0.15f , segment.second + 0.5f }, 1.0f, 0.3f, 1.0f, WHITE);
        }
        

        EndMode3D();
        DrawText(TextFormat("Reaper : %d", reaperScore), screenWidth/2, 5, 20, RED);
        DrawText(TextFormat("Player : %d", playerScore), screenWidth/2, 25, 20, RED);

        if (gameOver) {
            DrawText("GAME OVER", screenWidth/2, screenHeight/2, 60, RED);
            DrawText(TextFormat("Winner : %s", winner.c_str()), screenWidth /2 ,  50,35, MAROON);
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
#pragma endregion
