#include <stdlib.h>
#include <stdio.h>
#include "savescore.h"
//savescore.c
//Save score here so user can track their own shit.
void savescore(int score){
    //saving the file with directory for all PC
    char* path = getenv("LOCALAPPDATA"); // this thing has pointing to a LOCALDATA on your PC 
    char filepath[512]; // this is the array of char its 512
    snprintf(filepath,sizeof(filepath),"%s/Score.txt",path);// string builder and put it on pathfile 
    FILE* file = fopen(filepath,"w"); // this file opener that point to a current PC directory path.
    fprintf(file,"%d",score );  // the formatted text will write on the .txt file (depends on what u build) 
    fclose(file); // close the file
}
int loadscore(){
    //load the saved file from your PC!
    char* path = getenv("LOCALAPPDATA");
    char filepath[512];
    snprintf(filepath,sizeof(filepath),"%s/Score.txt",path);
    FILE* file = fopen(filepath,"r");
    int score;  
    fscanf(file,"%d",&score);
    fclose(file);
    return score;
}
