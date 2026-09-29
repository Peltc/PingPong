#pragma once 
#include <glad/gl.h>
typedef struct {
    float m[9];    

} matrix;
matrix* identity();
matrix* translation(float x,float y);
matrix* rotation(float angle);
matrix* scale(float sx,float sy);
matrix* multiplication(matrix* A,matrix* B);



