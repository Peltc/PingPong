#include <glad/gl.h>
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// matrix.c
// camera 2d
matrix* identity(){
    matrix* mat = malloc(sizeof(matrix));
    mat->m[0] = 1.0f; mat->m[1] = 0.0f; mat->m[2] = 0.0f;
    mat->m[3] = 0.0f; mat->m[4] = 1.0f; mat->m[5] = 0.0f;
    mat->m[6] = 0.0f; mat->m[7] = 0.0f; mat->m[8] = 1.0f;
    return mat;
}
matrix* translation(float x,float y){
    matrix* mat = identity();
    mat->m[2] = x;
    mat->m[5] = y;
    return mat;
}

matrix* rotation(float angle){
    matrix* mat = identity();
    float r_con = (angle*3.14159f/180.0f);
    mat->m[0] = cos(r_con); mat->m[1] = -sin(r_con); 
    mat->m[3] = sin(r_con); mat->m[4] = cos(r_con);
    return mat;
}
matrix* scale(float sx,float sy){
    matrix* mat = identity();
    mat->m[0] = sx;
    mat->m[4] = sy;
    return mat;
}

matrix* multiplication(matrix* A,matrix* B){
    matrix* mat = identity();
    for (int x=0;x<3;x++){
        for (int y=0;y<3;y++){
            mat->m[y*3+x] = 
                A->m[y*3+0]*B->m[0*3+x]+
                A->m[y*3+1]*B->m[1*3+x]+
                A->m[y*3+2]*B->m[2*3+x];
        }
    }
    return mat;
}
