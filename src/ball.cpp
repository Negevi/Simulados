#include <raylib.h>
#include <random>
#include <ctime>
#include "ball.h"

Coords GetRandomCoords(Rectangle border, float radius) {
    int radius_int = (int) radius + 0.5;
    Coords coords;

    coords.x = (float)border.x + radius_int + (rand() % (int)(border.width - 2 * radius_int));
    coords.y = (float)border.y + radius_int + (rand() % (int)(border.height - 2 * radius_int));

    return coords;
}

Velocity GetRandomVelocites(int range_X, int range_Y) {
    Velocity vel;
    vel.x = (float) ((rand() % (range_X) * 2) - range_X + 1);
    vel.y = (float) ((rand() % (range_Y) * 2) - range_Y + 1);
    return vel;
}

float mass_to_radius(int mass) {
    return (float) mass * 0.75;
}

Ball CreateRandomBall(int max_mass, Rectangle border, int max_velocity_x, int max_velocity_y) {
    Ball ball;

    Color color = {static_cast<unsigned char>(rand() % 255 + 1),
                   static_cast<unsigned char>(rand() % 255 + 1),
                   static_cast<unsigned char>(rand() % 255 + 1),
                   255};
    ball.color = color; // wtf?? gemini que fez

    ball.mass = (rand() % max_mass) + 1;
    ball.position = GetRandomCoords(border, mass_to_radius(ball.mass));
    ball.velocity = GetRandomVelocites(max_velocity_x, max_velocity_y);

    return ball;
}

Ball* CreateBallVector(int qnt, int max_mass, Rectangle border, int max_velocity_y, int max_velocity_x) {
    Ball* vector = (Ball*) malloc((qnt) * sizeof(Ball));

    for(int i = 0; i < qnt; i++) {
        vector[i] = CreateRandomBall(max_mass, border, max_velocity_y, max_velocity_x);
    }
    
    return vector;
}

void DrawCircles(Ball* vector, int qnt) {
    for(int i = 0; i < qnt; i++) {
        DrawCircle((int)vector[i].position.x, (int)vector[i].position.y, mass_to_radius(vector[i].mass), vector[i].color);
    }
}

void Move(float* pos_x, float* pos_y, float velocity_x, float velocity_y) {
    float dt = GetFrameTime();
    *pos_x += velocity_x * dt;
    *pos_y += velocity_y * dt;
}

void MoveBalls(Ball* vector, int qnt) {
    for(int i = 0; i < qnt; i++) {
        Move(&vector[i].position.x, &vector[i].position.y, vector[i].velocity.x, vector[i].velocity.y);
    }
}

void CheckBorderCollision(Ball* vector, Rectangle border, int qnt) {
    for(int i = 0; i < qnt; i++) {
        float radius = mass_to_radius(vector[i].mass);
        // left - right
        if(vector[i].position.x - radius <= border.x + 3 || 
           vector[i].position.x + radius >= border.x + border.width - 3) {
            vector[i].velocity.x *= -1;
        }
        // top - bottom
        if(vector[i].position.y - radius <= border.y + 3 || 
           vector[i].position.y + radius >= border.y + border.height - 3) {
                vector[i].velocity.y *= -1;
        }
    }
}

void Collide(Ball* ball_1, Ball* ball_2) {
    // formulas encontradas em "2-Dimensional Elastic Collisions without Trigonometry", vobarian.com

    /* Essencialmente, a formula encontra dois vetores; o normal e o tangencial. O primeiro e um vetor definido 
    pela distancia entre ambos os centros. O segundo e o vetor perpendicula ao primeiro no ponto de colisao,
    ambos com tamanho 1 (versores). A formula visa transformar as velocidades em relacao ao ponto de colisao e depois tratar como 
    duas colisoes em uma dimensao para ambas. */

    float x = ball_2->position.x - ball_1->position.x;
    float y = ball_2->position.y - ball_1->position.y; 
    float mod = sqrt(x * x + y * y);

    Vector2 unit_normal = {x / mod, y / mod};
    Vector2 unit_tangent = {-unit_normal.y, unit_normal.x};

    // Aqui, juntaria Vx e Vy em vetores para cada bola (de acordo com o pdf). Pulei esse passo para nao fazer contas descecessarias.

    // Aqui, e feito o produto escalar entre as velocidades e os versores Tg e N para encontrar as respectivas velocidades em ambas direcoes
    float v1_relativa_n = unit_normal.x * ball_1->velocity.x + unit_normal.y * ball_1->velocity.y;
    float v2_relativa_n = unit_normal.x * ball_2->velocity.x + unit_normal.y * ball_2->velocity.y;

    float v1_relativa_tg = unit_tangent.x * ball_1->velocity.x + unit_tangent.y * ball_1->velocity.y;
    float v2_relativa_tg = unit_tangent.x * ball_2->velocity.x + unit_tangent.y * ball_2->velocity.y;

    // Computando utilizando formulas da colisao 1D
    float v1_final_n = (v1_relativa_n * (ball_1->mass - ball_2->mass) + 2 * ball_2->mass * v2_relativa_n) / (ball_1->mass + ball_2->mass);
    float v2_final_n = (v2_relativa_n * (ball_2->mass - ball_1->mass) + 2 * ball_1->mass * v1_relativa_n) / (ball_1->mass + ball_2->mass);

    // Re-transformando em vetores
    Vector2 v1_vector_n_final = {v1_final_n * unit_normal.x, v1_final_n * unit_normal.y};
    Vector2 v2_vector_n_final = {v2_final_n * unit_normal.x, v2_final_n * unit_normal.y};

    // Velocidades tangencias permanecem iguais antes e depois da colisao pois vetor tangencial e tangencial ao ponto de colisao. 
    // vx_final_tg = vx_relativa_tg <-- na pratica, significa isso
    Vector2 v1_vector_tg_final = {v1_relativa_tg * unit_tangent.x, v1_relativa_tg * unit_tangent.y};
    Vector2 v2_vector_tg_final = {v2_relativa_tg * unit_tangent.x, v2_relativa_tg * unit_tangent.y};

    // Resolvendo a colisao somando ambos componentes para a velocidade final.
    ball_1->velocity.x = v1_vector_n_final.x + v1_vector_tg_final.x;
    ball_1->velocity.y = v1_vector_n_final.y + v1_vector_tg_final.y;

    ball_2->velocity.x = v2_vector_n_final.x + v2_vector_tg_final.x;
    ball_2->velocity.y = v2_vector_n_final.y + v2_vector_tg_final.y;

    //FUNCIONOU?? (e de primeira ainda)
}

void CheckBallCollision(Ball* ball_vector, int qnt) {
    for (int i = 0; i < qnt; i++) {
        float radius = mass_to_radius(ball_vector[i].mass);

        for (int j = i + 1; j < qnt; j++) {
            float radius_compared = mass_to_radius(ball_vector[j].mass);

            float x = ball_vector[i].position.x - ball_vector[j].position.x;
            float y = ball_vector[i].position.y - ball_vector[j].position.y;

            // calculando o quadrado da distancia. vi em um video, mais eficiente por nao utilizar o sqrt()
            float distance_sq = x * x + y * y; 

            if (distance_sq <= (radius + radius_compared) * (radius + radius_compared)) {
                Collide(&ball_vector[i], &ball_vector[j]);
            }
        }
    }
}
