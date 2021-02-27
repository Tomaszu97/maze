#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad.h"
#include "GLFW/glfw3.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

GLFWwindow *window;
unsigned int shaderProgram;

void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

char *readFileIntoStr(char *fileName, unsigned int strBufSize){
    FILE *f = fopen(fileName, "r");
    if(!f){
        printf("error: couldn't open file %s\n", fileName);
        exit(1);
    }
    
    char *str = (char*)calloc(sizeof(char), strBufSize);
    unsigned int pos = 0;
    while(pos < (strBufSize-1)){
        int c  = fgetc(f);
        if(feof(f)) break;
        str[pos] = (char)c;    
        pos++;
    }
    str[pos] = '\0';

    fclose(f); 
    return str;
}

void setupGraphicsWindow(int windowWidth, int windowHeight){
    const char *vertexShaderSource = readFileIntoStr("src/vertexShader.vert", 1024);
    const char *fragmentShaderSource = readFileIntoStr("src/fragmentShader.frag", 1024);
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required on macos    
    window = glfwCreateWindow(windowWidth, windowHeight, "xD!", NULL, NULL);
    if (window == NULL){
        printf("error: failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    
    //init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("error: failed to initialize GLAD\n");
        exit(1);
    }

    //setup window size and size callback
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    //these will come in handy 
    int success;
    char infoLog[512];

    //setup vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check vertex shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("error: vertex shader compilation failed! Log below:\n%s", infoLog);
        exit(1);
    }

    //setup fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //check fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("error: fragment shader compilation failed! Log below:\n%s", infoLog);
        exit(1);
    }

    //setup shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check shader program compile errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("error: shader program - linking failed!\n%s", infoLog);
        exit(1);
    } 
    
    //we don't need these anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glEnable(GL_DEPTH_TEST);
}

#endif