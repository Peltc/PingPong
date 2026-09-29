#include "shader.h"
#include <glad/gl.h>
#include <stdlib.h>
#include <stdio.h>
// shader.c
shader* ini_shader() {
    shader* s = malloc(sizeof(shader));
    GLint success;
    char log[512];
    const char* fmshader = 
        "#version 120\n"
        "uniform vec4 uColor;\n"
        "void main(){\n"
        "   gl_FragColor = uColor;\n"
        "}\n";
    
    const char* vtshader = 
        "#version 120\n"
        "uniform mat3 mTRS;\n"
        "uniform mat3 mModel;\n"
        "attribute vec3 mainpos;\n"
        "void main(){\n"
        "   vec3 resultT = mTRS * mModel * vec3(mainpos.xy,1.0);\n"
        "   gl_Position = vec4(resultT.xy,mainpos.z,1.0);\n"
        "}\n";
    
    GLuint fmsource = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fmsource,1,&fmshader,0);
    glCompileShader(fmsource);
    GLuint vtsource = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtsource,1,&vtshader,0);
    glCompileShader(vtsource);
    s->program = glCreateProgram();
    glGetShaderiv(fmsource,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fmsource,512,NULL,log);
        printf("shader error:\n%s\n",log);
    }
    glAttachShader(s->program,fmsource);
    glAttachShader(s->program,vtsource);
    glBindAttribLocation(s->program,0,"mainpos");
    glLinkProgram(s->program);
    s->GetMat3 = glGetUniformLocation(s->program,"mTRS");
    s->GetModel = glGetUniformLocation(s->program,"mModel");
    s->GetColor = glGetUniformLocation(s->program,"uColor");
    return s;
}
void useprogram(shader* s){
    glUseProgram(s->program);
}
