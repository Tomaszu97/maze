#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GLwrapper.h"
#include "cglm/cglm.h"
#include "cglm/struct.h"
#include "cglm/call.h"
#include "primitives.h"
#include "scene.h"
#include "map.h"

#define LABYRINTH_SIZE 61 // odd numbers only
#define MAP_W LABYRINTH_SIZE
#define MAP_H LABYRINTH_SIZE

float deltaT = 0.0f;
float lastT = 0.0f;

void processInput(GLFWwindow *window){
    const float camSpeed = 5.0f * deltaT;
    const float fovChangeSpeed = 75.0f * deltaT;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
    // if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
    //     fov += fovChangeSpeed;
    //     if (fov > 135.0f) fov = 135.0f;
    // }
    // if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    //     fov -= fovChangeSpeed;
    //     if (fov < 10.0f) fov = 10.0f;
    // }
    // if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
    //     vec3 tmp;
    //     glm_vec3_scale(camFront, camSpeed, tmp);
    //     glm_vec3_sub(camPos, tmp, camPos);
    // }
    // if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    //     vec3 tmp;
    //     glm_vec3_scale(camFront, camSpeed, tmp);
    //     glm_vec3_add(camPos, tmp, camPos);
    // }
    // if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
    //     vec3 tmp;
    //     glm_vec3_scale(camUp, camSpeed, tmp);
    //     glm_vec3_add(camPos, tmp, camPos);
    // }
    // if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
    //     vec3 tmp;
    //     glm_vec3_scale(camUp, camSpeed, tmp);
    //     glm_vec3_sub(camPos, tmp, camPos);
    // }
    // if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
    //     vec3 tmpCamRight;
    //     glm_vec3_cross(camUp, camFront, tmpCamRight);
    //     glm_normalize(tmpCamRight);
    //     glm_vec3_scale(tmpCamRight, camSpeed, tmpCamRight);
    //     glm_vec3_add(camPos, tmpCamRight, camPos);
    // }
    // if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
    //     vec3 tmpCamRight;
    //     glm_vec3_cross(camUp, camFront, tmpCamRight);
    //     glm_normalize(tmpCamRight);
    //     glm_vec3_scale(tmpCamRight, camSpeed, tmpCamRight);
    //     glm_vec3_sub(camPos, tmpCamRight, camPos);
    // }
}

int main(){
    struct map *ma = newMap(MAP_W, MAP_H);
    prepareMaze(ma);
    int mazeFinished = 0;
    float mazeTickTimer = glfwGetTime();

    setupGraphicsWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    initScene();
                                    
    unsigned int arrX = MAP_W;
    unsigned int arrY = MAP_H;
    unsigned int cubeObjCount = arrX*arrY;

    unsigned int cubeObj = createObject(cubeVertices, 
                                    36, 
                                    "block/spruce_planks.png",
                                    (vec3){1.0f, 1.0f, 1.0f},
                                    (vec3){0.0f, 0.0f, 0.0f},
                                    0,
                                    (vec3){0.0f, -0.5001f, 0.0f});

    unsigned int spelunkerCubeObj = createObject(cubeVertices, 
                                36, 
                                "block/ice.png",
                                (vec3){1.0f, 1.0f, 1.0f},
                                (vec3){0.0f, 0.0f, 0.0f},
                                0,
                                (vec3){0.0f, -0.5001f, 0.0f});


    // do stuff to properly scale texture so it repeats nicely
    planeVertices[13] = (1.0f*((float)arrX+4.0f));
    planeVertices[23] = (1.0f*((float)arrX+4.0f));
    planeVertices[28] = (1.0f*((float)arrX+4.0f));
    planeVertices[9] = (1.0f*((float)arrY+4.0f));
    planeVertices[19] = (1.0f*((float)arrY+4.0f));
    planeVertices[24] = (1.0f*((float)arrY+4.0f));

    unsigned int planeObj = createObject(planeVertices, 
                                            6, 
                                            "block/jungle_log.png",
                                            (vec3){(float)arrX+2.0f, (float)arrY+2.0f, 1.0f},
                                            (vec3){1.0f, 0.0f, 0.0f},
                                            90,
                                            (vec3){0.0f, 0.0f, 0.0f});

    while(!glfwWindowShouldClose(window)){
        // timing
        float currentT = glfwGetTime();
        deltaT = currentT - lastT;
        lastT = currentT;
        
        // input handling
        processInput(window);

        // calculation
        float sineVal = sin(M_PI_2*glfwGetTime()*0.125);
        float cosineVal = cos(M_PI_2*glfwGetTime()*0.125);
        fov = 55;
        setCameraPosition((vec3){0.7*sineVal*(arrX), 0.6*arrX, 0.7*cosineVal*(arrY)});
        setCameraLookAt((vec3){0,arrX/4,0});
        
        if(mazeFinished == 0){
            mazeTickTimer += deltaT;
            if (mazeTickTimer > 0.02){
                mazeTickTimer = 0;
                if(stepMaze(ma) == 0 ) mazeFinished = 1;
            }
        }

        // drawing
        clearSceneColor((vec3){0.93f, 0.6f, 0.26f});
        for(int i=0; i<cubeObjCount; i++){
            vec3 tempPos = {(float)(i%arrX)-(float)(arrX/2), -0.5001f, (float)floor(i/arrY)-(float)(arrY/2)};
            
            if (i == current_x+(MAP_W*current_y)){
                memcpy(positionBuf[spelunkerCubeObj], tempPos, 3*sizeof(float));
                drawObject(spelunkerCubeObj);
            }
            // inverted so it looks better and renders quicker
            else if (ma->data[i] == VISITED){
                memcpy(positionBuf[cubeObj], tempPos, 3*sizeof(float));
                drawObject(cubeObj);
            }
        }
        drawObject(planeObj);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
    glfwTerminate();
    return 0;
}

