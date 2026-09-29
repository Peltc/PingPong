#include <glad/gl.h>
#include <math.h>
#include "ball.h"
#include "shader.h"
#include "matrix.h"
#include <stdlib.h>

#define segments 24
#define realscale 0.03

// ball.c
//
static float vertex[segments * 3];
ball* spawnball(){
    ball* mball = malloc(sizeof(ball));
    mball->r = realscale;
    mball->speed = 0.007f;
    mball->vx = mball->speed;
    mball->vy = mball->speed;
    mball->ModelX = -0.5f;
    mball->ModelY = -0.9f;
    for(int i=0;i<segments;i++){
        float angle = (float)(i*360.0f/segments);
        float radConvert = (float)(angle*3.14159/180.0f);
        float x = cos(radConvert);
        float y = sin(radConvert);
        float z = 0.0f;
        vertex[i*3+0] = x;
        vertex[i*3+1] = y;
        vertex[i*3+2] = z;
    }
    glGenVertexArrays(1,&mball->VAO);
    glBindVertexArray(mball->VAO);
    glGenBuffers(1,&mball->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,mball->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    return mball;
}

void draw(ball* b,matrix* mat,shader* mshader){
    glBindVertexArray(b->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,b->VBO);
    matrix* T = translation(b->ModelX,b->ModelY);
    matrix* R = rotation(0.0f);
    matrix* S = scale(realscale,realscale);
    matrix* final = multiplication(T,multiplication(R,S));
    glUniformMatrix3fv(mshader->GetModel,1,GL_TRUE,final->m);
    free(T);free(R);free(S);free(final);
    glDrawArrays(GL_TRIANGLE_FAN,0,segments);
}
