#include <glad/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include "box.h"
#include "matrix.h"
#include "shader.h"
box* ini_box(){
    box* mbox = malloc(sizeof(box));
    mbox->r = 1.0f;
    mbox->speed = 0.05f;
    mbox->ModelSx = 0.01f;
    mbox->ModelSy = 0.15f;
    float vertex[18] = 
    {
        mbox->r,mbox->r,mbox->r-mbox->r,
        -mbox->r,-mbox->r,mbox->r-mbox->r,
        mbox->r,-mbox->r,mbox->r-mbox->r,

        mbox->r,mbox->r,mbox->r-mbox->r,
        -mbox->r,mbox->r,mbox->r-mbox->r,
        -mbox->r,-mbox->r,mbox->r-mbox->r,
    };
    glGenVertexArrays(1,&mbox->VAO);
    glBindVertexArray(mbox->VAO);
    glGenBuffers(1,&mbox->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,mbox->VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    return mbox;
}
void drawbox(box* b,matrix* mat,shader* mshader){
    glBindVertexArray(b->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,b->VBO);
    matrix* T = translation(b->ModelX,b->ModelY);
    matrix* R = rotation(0.0f);
    matrix* S = scale(b->ModelSx,b->ModelSy);
    matrix* final = multiplication(T,multiplication(R,S));
    glUniformMatrix3fv(mshader->GetModel,1,GL_TRUE,final->m);    
    glDrawArrays(GL_TRIANGLES,0,6);
}
