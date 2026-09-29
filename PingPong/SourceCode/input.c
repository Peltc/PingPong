#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include <GLFW/glfw3.h>
#include "ball.h"
#include <stdbool.h>

void runthisshit(input* p_input,GLFWwindow* window){
    p_input->MoveDown = false;
    p_input->MoveUp = false;
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
        p_input->MoveUp = true;
        p_input->MoveDown = false;
    }
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
        p_input->MoveUp = false;
        p_input->MoveDown = true;
    }
    

}


