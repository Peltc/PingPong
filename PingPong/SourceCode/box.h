#pragma once 
#include <glad/gl.h>
#include "shader.h"
#include "matrix.h"
//box.h
typedef struct {
    float ModelSx;
    float ModelSy;
    GLuint VBO;
    GLuint VAO;
    float r;
    float ModelX;
    float ModelY;
    float speed;
} box;

box* ini_box();
void drawbox(box* b,matrix* mat,shader* mshader);
