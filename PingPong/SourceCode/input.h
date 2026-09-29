#pragma once 
#include <GLFW/glfw3.h>
#include <stdbool.h>
typedef struct {
    bool MoveUp;
    bool MoveDown;
    
} input;
void runthisshit(input* p_input,GLFWwindow* window);
