#pragma once
#include "shader.h"
#include "matrix.h"
#include <glad/gl.h>
//ball.h
typedef struct {
    GLuint VBO;
    GLuint VAO;
    float r;
    float speed;
    float vx;
    float vy;
    float ModelX;
    float ModelY;
} ball;
ball* spawnball();
void draw(ball* b,matrix* mat,shader* mshader);


