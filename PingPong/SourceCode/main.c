#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// code files .h
// main.c
#include "shader.h"
#include "ball.h"
#include "box.h"
#include "matrix.h"
#include "input.h"
#include "savescore.h"
#define CONSTSPEEDVALUE 0.001
clock_t pauseTimerLast;
int x = 800;
int y = 600;
bool running = true;
bool paused = false;
GLFWwindow* window;
GLFWmonitor* monitor;
const GLFWvidmode* mode;
// fps cfg
clock_t LastFPSTime;
int frame = 0.0f;
char title[64];
int score = 0;

void savescoreFile(){
    if (score>1){
        savescore(score);
    }
}
void printHighScore(){
    int highscore = loadscore();
    printf("HighScore: %d\n",highscore);
}
void Restart(box* box1,box* box2,ball* mainball){
    if (paused){
        clock_t pausedTimerNow = clock();
        double ResultTime = (double)(pausedTimerNow - pauseTimerLast)/CLOCKS_PER_SEC;
        printf("Restarting... %f\n",(float)ResultTime);
        if (ResultTime >= 3.0f){
            //wall1: 0.990000
            //wall2: -0.990000
            //ballX: -0.500000
            //ballY: -0.900000
            box1->ModelX = 0.990000f;
            box2->ModelX = -0.990000f;
            mainball->ModelX = -0.500000f;
            mainball->ModelY = -0.900000f;
            mainball->speed = 0.007f;
            mainball->vx = mainball->speed;
            mainball->vy = mainball->speed;
            paused = false;
            score = 0.0f;
            printf("Pingpong alpha 0.0.0.1\n");
            printf("press W and S to Move the box\n");
            printf("Beat the game!\n");
            printf("free 2 points for you!\n");
            printf("Dont try to lose your paddle :)\n");
            printf("Press ESC to Quit\n");
            printHighScore();
        }
    }
}
void Death(ball* mainball){
    printf("You lose!\n");
    pauseTimerLast = clock();
    paused = true;
}
// Collision Between Ball VS Box Test!!!
float min(float a,float b){
    if(a < b){
        return a;
    }
    return b;
}
float max(float a,float b){
    if(a > b){
        return a;
    }
    return b;
}
void increasespeed(ball* mainball){
    // chatgpt teach me
    // rescaling math and Vector Magnitude/Length
    mainball->speed += CONSTSPEEDVALUE;
    float length = sqrt(mainball->vx*mainball->vx+mainball->vy*mainball->vy); // this is velocity vector
    if (length == 0.0f){
        return;
    }
    float nvx = mainball->vx/length; // normalized velocity X
    float nvy = mainball->vy/length; // normalized velocity Y
    mainball->vx = nvx*mainball->speed;
    mainball->vy = nvy*mainball->speed;
    //printf("speed of ball: %f\n",mainball->speed);
    // I should remember the: normalized velocity and pythagorean velocity vector direction
}
void collisionWallVSBall(ball* mainball,box* box1,box* box2){
    //chatgpt teach me here
    //the math is: Pythagorean theorem and normalized math and overlapping math amd geometry math!
    float nx = 0.0f;
    float ny = -1.0f;
    float dot = mainball->vx*nx+mainball->vy*ny;
    if (mainball->ModelX+mainball->r>1.0f){
        Death(mainball);
        //running = false;
    }
    if (mainball->ModelX-mainball->r<-1.0f){
        Death(mainball);
        //running = false;
    }
    if (mainball->ModelY+mainball->r>1.0f){
        mainball->vy = -mainball->vy;
        //mainball->vy -= 2.0f*dot*ny;
    }
    if (mainball->ModelY-mainball->r<-1.0f){
        mainball->vy = -mainball->vy;
        //mainball->vy -= 2.0f*dot*ny;
    }
    box* getBothBox[2];
    getBothBox[0] = box1;
    getBothBox[1] = box2;
    for(int i=0;i<2;i++){
        box* getBothFinal = getBothBox[i];
        float top = getBothFinal->ModelY + getBothFinal->ModelSy;
        float bottom = getBothFinal->ModelY - getBothFinal->ModelSy;
        float right = getBothFinal->ModelX + getBothFinal->ModelSx;
        float left = getBothFinal->ModelX - getBothFinal->ModelSx;
        //Test
        float overlapx = min(right,max(mainball->ModelX,left));
        float overlapy = min(top,max(mainball->ModelY,bottom));
        float dx = mainball->ModelX - overlapx;
        float dy = mainball->ModelY - overlapy;
        float finaldist = sqrt(dx*dx+dy*dy);
        bool check_dist = (finaldist < mainball->r);
        if(check_dist&&finaldist!=0.0f){
            float nx = dx/finaldist;
            float ny = dy/finaldist;
            float dot = mainball->vx*nx+mainball->vy*ny;
            mainball->vx -= 2.0f*nx*dot;
            mainball->vy -= 2.0f*ny*dot;
            score++;
            increasespeed(mainball);
            printf("Score: %d\n",score);
            savescoreFile();
        }

    }

}
void moveballonstart(ball* mainball){
    mainball->ModelX += mainball->vx;
    mainball->ModelY += mainball->vy;
}
//wall movements
void ControllWalls(input* maininput,box* box1,box* box2){
    if(maininput->MoveUp){
        box1->ModelY += box1->speed;
        box2->ModelY += box2->speed;
    }
    if(maininput->MoveDown){
        box1->ModelY -= box1->speed;
        box2->ModelY -= box1->speed;
    }
}
void Camera2D(matrix* m,shader* s){ // my camera matrix caller here!
    matrix* T = translation(0.0f,0.0f);
    matrix* R = rotation(0.0f);
    matrix* S = scale(1.0f,1.0f);
    matrix* result = multiplication(T,multiplication(R,S));
    glUniformMatrix3fv(s->GetMat3,1,GL_TRUE,result->m);
    free(T); free(R); free(S); free(result);    
}
void FPS(){   
   clock_t CurrentFPSTime = clock();
   frame++;
   double finaltime = (double)(CurrentFPSTime-LastFPSTime)/(double)CLOCKS_PER_SEC;
   if(finaltime>=(float)1.0f){
       //printf("FPS: %d\n",frame);
       sprintf(title,"PingPong || FPS: %d\n",frame);
       frame = 0.0f;
       LastFPSTime = CurrentFPSTime;
   }
}
void resize(GLFWwindow* paramwindow,int w,int h){
    glViewport(0,0,w,h);
}
void reposwin(){
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
    float newx = (mode->width-x)/2.0f;
    float newy = (mode->height-y)/2.0f;
    glfwSetWindowPos(window,newx,newy);
    glfwSetFramebufferSizeCallback(window,resize);
}
void object(matrix* matparam,shader* mainshader,ball* ball,box* box1,box* box2){
    useprogram(mainshader);
    //ball
    glUniform4f(mainshader->GetColor,1.0,0.0,0.0,1.0);
    draw(ball,matparam,mainshader);
    // walls
    glUniform4f(mainshader->GetColor,1.0,1.0,1.0,1.0);
    drawbox(box1,matparam,mainshader);
    drawbox(box2,matparam,mainshader);
}
void ReposWalls(box* box1,box* box2){
   box1->ModelX = 0.99f;
   box1->ModelY = 0.0f;
   box2->ModelX = -0.99f;
   box2->ModelY = 0.0f;
}
// call all of things here
void spawnwindow(){
    //printHighScore();
    printf("Pingpong alpha 0.0.0.1\n");
    printf("press W and S to Move the box\n");
    printf("Beat the game!\n");
    printf("free 2 points for you!\n");
    printf("Dont try to lose your paddle :)\n");
    printf("Press ESC to Quit\n");
    printHighScore();
    LastFPSTime = clock();
    glfwInit();
    window = glfwCreateWindow(x,y,"PingPong",monitor,NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    reposwin();
    shader* mainshaderobj = ini_shader();
    ball* mainball = spawnball();
    box* mainbox1 = ini_box();
    box* mainbox2 = ini_box();
    matrix* mainmatrix = identity();
    input* maininput;
    ReposWalls(mainbox1,mainbox2);
    //debug
    //printf("wall1: %f\n",mainbox1->ModelX);
    //printf("wall2: %f\n",mainbox2->ModelX);

    //printf("ballX: %f\n",mainball->ModelX);
    //printf("ballY: %f\n",mainball->ModelY);



    


    
    while(running){
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //printf("ball speed: %f\n",mainball->speed);
        Restart(mainbox1,mainbox2,mainball);
        object(mainmatrix,mainshaderobj,mainball,mainbox1,mainbox2);
        Camera2D(mainmatrix,mainshaderobj);
        if (!paused){
            moveballonstart(mainball);
            collisionWallVSBall(mainball,mainbox1,mainbox2);
            runthisshit(maininput,window);
            ControllWalls(maininput,mainbox1,mainbox2);
            FPS();
            glfwSetWindowTitle(window,title);
        }
        glfwPollEvents();
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
            savescoreFile();
            printf("Quit!\n");
            running = false;
        }
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}
int main(int argv,char* args[]){
    spawnwindow();
    return 0;
}
