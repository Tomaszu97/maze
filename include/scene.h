#ifndef SCENE_H
#define SCENE_H

#include "GLwrapper.h"
#include "cglm/cglm.h"
#include "cglm/struct.h"
#include "cglm/call.h"
#include "primitives.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_OBJECTS 512

int lastObject = -1;
unsigned int VBO[MAX_OBJECTS];
unsigned int VAO[MAX_OBJECTS];
unsigned int vertexCountBuf[MAX_OBJECTS];
unsigned int textureBuf[MAX_OBJECTS];
vec3 scaleBuf[MAX_OBJECTS];
vec3 rotationVectorBuf[MAX_OBJECTS];
float rotationValueBuf[MAX_OBJECTS];
vec3 positionBuf[MAX_OBJECTS];

float fov = 90.0f;
vec3 camPos = {0.0f, 0.0f, 3.0f};
vec3 camFront = {0.0f, 0.0f, -1.0f};
vec3 camUp = {0.0f, 1.0f, 0.0f};


/* initializes object buffers and scene in general */
void initScene(){
    glGenBuffers(MAX_OBJECTS, VBO);
    glGenVertexArrays(MAX_OBJECTS, VAO);
}

/* loads texture from png/jpg image file (other formats not tested) */
unsigned int loadTexture(char *fileName){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //mipmap here has no effect whatsoever
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);

    if(data){
        unsigned int format = GL_RGBA;
        switch(nrChannels){
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        printf("error: failed to load texture from file\n");
    }
    stbi_image_free(data);
    return texture;
}


/* returns objects VAO/VBO/vertexCountBuf index */
unsigned int createObject(float *vertices, 
                            unsigned int vertexCount, 
                            char *textureFilename, 
                            vec3 scale, 
                            vec3 rotationVector, 
                            float rotationValue, 
                            vec3 position){
    lastObject++;
    if (lastObject >= MAX_OBJECTS){
        printf("error: tried to create too many objects in the scene\n");
        exit(1);
    }

    vertexCountBuf[lastObject] = vertexCount;
    rotationValueBuf[lastObject] = rotationValue;
    for (int i=0; i<3; i++){
        scaleBuf[lastObject][i] = scale[i];
        rotationVectorBuf[lastObject][i] = rotationVector[i];
        positionBuf[lastObject][i] = position[i];
    }
    
    unsigned int vertexByteSize = 5*sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[lastObject]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexByteSize, vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO[lastObject]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexByteSize, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexByteSize, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    char *tmpFilename = calloc(128, sizeof(char));
    sprintf(tmpFilename, "res/%s", textureFilename);
    textureBuf[lastObject] = loadTexture(tmpFilename);
    free(tmpFilename);

    // TODO support -1 value 
    return lastObject;
}

/* paints the whole scene to color */
void clearSceneColor(vec3 color){
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* draws textured object on the screen */
void drawObject(unsigned int objectIndex){
    // transformations //
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, positionBuf[objectIndex]);
    glm_rotate(model, glm_rad(rotationValueBuf[objectIndex]), rotationVectorBuf[objectIndex]);
    glm_scale(model, scaleBuf[objectIndex]);
    
    mat4 view;
    glm_mat4_identity(view);
    vec3 tempDest;
    glm_vec3_add(camPos, camFront, tempDest);
    glm_lookat(camPos, tempDest, camUp, view);

    mat4 projection;
    glm_mat4_identity(projection);
    glm_perspective(glm_rad(fov), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 10000.0f, projection);

    glUseProgram(shaderProgram);
    unsigned int modelUnif = glGetUniformLocation(shaderProgram, "modelMat");
    unsigned int viewUnif = glGetUniformLocation(shaderProgram, "viewMat");
    unsigned int projUnif = glGetUniformLocation(shaderProgram, "projMat");
    glUniformMatrix4fv(modelUnif, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewUnif, 1, GL_FALSE, view);
    glUniformMatrix4fv(projUnif, 1, GL_FALSE, projection); 

    // drawing //
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBuf[objectIndex]);
    glBindVertexArray(VAO[objectIndex]);
    glDrawArrays(GL_TRIANGLES, 0, vertexCountBuf[objectIndex]);
    glBindVertexArray(0);
}

void moveCamera(vec3 cameraPosD){
    //dirty
    camPos[0] += -cameraPosD[0];
    camPos[1] += -cameraPosD[1];
    camPos[2] += cameraPosD[2];
}

void setCameraPosition(vec3 cameraPos){
    //dirty
    camPos[0] = -cameraPos[0];
    camPos[1] = -cameraPos[1];
    camPos[2] = cameraPos[2];
}

void getCameraPosition(vec3 *posToRet){
    memcpy(posToRet, camPos, 3*sizeof(float));
}

void setCameraDirection(vec3 camDir){
    memcpy(camFront, camDir, 3*sizeof(float));
    //dirty
    camFront[0] *= -1;
    camFront[1] *= -1;

    normalizeCameraPos();
}

void setCameraLookAt(vec3 pointToLookAt){
    glm_vec3_sub(pointToLookAt, camPos, camFront);
    normalizeCameraPos();
}

void normalizeCameraPos(){
    vec3 tmpCamRight;
    glm_cross(camFront, (vec3){0.0f, 1.0f, 0.0f}, tmpCamRight);
    glm_cross(camFront, tmpCamRight, camUp);
    glm_normalize(camFront);
    glm_normalize(camUp);
}



//unsigned int EBO[MAX_OBJECTS];
// unsigned int indices[] = {
//     0,1,2,
//     1,2,3
// };
// unsigned int EBO[1];
// glGenBuffers(1, EBO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

#endif
