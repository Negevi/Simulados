#ifndef BALL_H
#define BALL_H

typedef struct {
    float x;
    float y;
} Coords;

typedef struct {
    float x;
    float y;
} Velocity;

typedef struct {
    Coords position;
    Velocity velocity;
    int mass;
    Color color;
} Ball;

Ball* CreateBallVector(int qnt, int max_mass, Rectangle border, int max_velocity_y, int max_velocity_x);
void DrawCircles(Ball* vector, int qnt);
void MoveBalls(Ball* vector, int qnt);
void CheckBorderCollision(Ball* ball_vector, Rectangle border, int qnt);
void CheckBallCollision(Ball* ball_vector, int qnt);

#endif 