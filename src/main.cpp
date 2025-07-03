#include <raylib.h>
#include <random>
#include <ctime>
#include "ball.h"

/* Global Constants; All units considered in SI */
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200
#define FPS 240
#define QNT_CIRCLES 7
#define MAX_MASS 100
#define MAX_VELOCITY_X 500
#define MAX_VELOCITY_Y 500

int main() {

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Collision Simulator");
    SetTargetFPS(FPS);
    srand(time(NULL));

    Rectangle border;
    border.height = (int) WINDOW_HEIGHT * 0.90;
    border.width = (int) WINDOW_WIDTH * 0.90;
    border.x = (int) WINDOW_WIDTH * 0.05;
    border.y = (int) WINDOW_HEIGHT * 0.05;

    Ball* ball_vector = CreateBallVector(QNT_CIRCLES, MAX_MASS, border, MAX_VELOCITY_X, MAX_VELOCITY_Y);

    // Inicio do loop
    while(WindowShouldClose() == false) {
        CheckBorderCollision(ball_vector, border, QNT_CIRCLES);
        CheckBallCollision(ball_vector, QNT_CIRCLES);

        MoveBalls(ball_vector, QNT_CIRCLES);

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleLinesEx(border, 4, RED); // Draws the border
        DrawCircles(ball_vector, QNT_CIRCLES);

        EndDrawing();
    }

    free(ball_vector);
    CloseWindow();
    return 0;
}