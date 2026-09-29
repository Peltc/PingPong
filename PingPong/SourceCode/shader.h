#pragma once
#include <glad/gl.h>
//shader.h
typedef struct {
    GLuint program;
    GLuint GetMat3;
    GLuint GetModel;
    GLuint GetColor;

} shader;
shader* ini_shader();
void useprogram(shader* s);


