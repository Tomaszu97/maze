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

#define MAP_W 41 //longer
#define MAP_H 41 //shorter

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

int randint(int min, int max)
{
    int tmp;
    if (max>=min)
        max-= min;
    else
    {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    return max ? (rand() % max + min) : min;
}

int main(){
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
                                            (vec3){(float)arrX+4.0f, (float)arrY+4.0f, 1.0f},
                                            (vec3){1.0f, 0.0f, 0.0f},
                                            90,
                                            (vec3){0.0f, 0.0f, 0.0f});

    while(!glfwWindowShouldClose(window)){
        float currentT = glfwGetTime();
        deltaT = currentT - lastT;
        lastT = currentT;
        processInput(window);

        float sineVal = sin(M_PI_2*glfwGetTime()*0.125);
        float cosineVal = cos(M_PI_2*glfwGetTime()*0.125);
        fov = 65;
        setCameraPosition((vec3){sineVal*arrX, 0.8f*arrX, cosineVal*arrX});
        setCameraLookAt((vec3){0,0,0});

        clearSceneColor((vec3){0.5f, 0.3f, 0.7f});
        
        for(int i=0; i<cubeObjCount; i++){
            vec3 tempPos = {(float)(i%arrX)-((float)(arrX/2)-0.5f), -0.5001f, (float)floor(i/arrY)-((float)(arrY/2)-0.5f)};
            memcpy(positionBuf[cubeObj], tempPos, 3*sizeof(float));
            if(i%2)drawObject(cubeObj);
        }
        drawObject(planeObj);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
    glfwTerminate();
    return 0;
}





// int main(int argc, char *argv[]){
//     struct map *ma = newMap(MAP_W, MAP_H);     
    
//     generateMaze(ma);
//     delMap(ma);

//     return 0;
// }
