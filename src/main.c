#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GLwrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "cglm/cglm.h"
#include "cglm/struct.h"
#include "cglm/call.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

float mixVal = 0.5;

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        mixVal += 0.01f;
        if (mixVal > 1.0f) mixVal = 1.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        mixVal -= 0.01f;
        if (mixVal < 0.0f) mixVal = 0.0f;
    }
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

unsigned int loadTexture(char *fileName){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //mipmap has no effect whatsoever
    
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
        printf("error: failed to load texture from file");
    }
    stbi_image_free(data);
    return texture;
}

int main(){
    setupGraphicsWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    float vertices1[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    };
    unsigned int indices[] = {
        0,1,2,
        1,2,3
    };

    unsigned int VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    
    unsigned int VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int EBO[1];
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    unsigned int catTexture = loadTexture("res/garfield.png");
    unsigned int dogTexture = loadTexture("res/hourglassdog.jpg");

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);

    
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       
        float sineVal = (sin(M_PI_2*glfwGetTime()*2)+1)/2;
        int sineUnif = glGetUniformLocation(shaderProgram, "sine");
        glUseProgram(shaderProgram);
        glUniform1f(sineUnif, sineVal);

        mat4 transform;
        glm_mat4_identity(transform);
        glm_translate(transform, (vec4){0.50f, 0.50f, 0.0f, 1.0f});
        glm_rotate(transform, glm_rad(mixVal*360), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(transform, (vec3){(sineVal-0.5)*2, (sineVal-0.5)*2, 0.5f});
        //^ goes up, lowest transform is first
        unsigned int transformUnif = glGetUniformLocation(shaderProgram, "transform");
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformUnif, 1, GL_FALSE, transform);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, catTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dogTexture);

        glBindVertexArray(VAO[0]);
        //glDrawArrays(GL_TRIANGLES, 0, 6); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);  
                
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
    glfwTerminate();
    return 0;
}
