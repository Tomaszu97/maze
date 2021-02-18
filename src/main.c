#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
}

const char *vertexShaderSource = "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
layout (location = 1) in vec3 aColor;\n\
out vec3 vertexColor;\n\
void main(){\n\
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
   vertexColor = aColor;\n\
}\0";

const char *fragmentShaderSource = "\
#version 330 core\n\
in vec3 vertexColor;\n\
out vec4 FragColor;\n\
uniform vec4 ourColor;\n\
void main(){\n\
   FragColor = vec4(vertexColor.rg, (vertexColor.b/3)+ourColor.b, 1.0);\n\
}\0";

void setupGraphicsWindow(int windowWidth, int windowHeight, GLFWwindow **window, unsigned int *shaderProgram){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required on macos    
    *window = glfwCreateWindow(windowWidth, windowHeight, "xD!", NULL, NULL);
    if (*window == NULL){
        printf("error: failed to create GLFW window\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(*window);
    
    //init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("error: failed to initialize GLAD\n");
        exit(-1);
    }

    //setup window size and size callback
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
  
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
        printf("error: vertex shader compilation failed!\n%s", infoLog);
        exit(-1);
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
        printf("error: fragment shader compilation failed!\n%s", infoLog);
        exit(-1);
    }

    //setup shader program
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);
    //check shader program compile errors
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        printf("error: shader program - linking failed!\n%s", infoLog);
        exit(-1);
    } 
    
    //we don't need these anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}


int main(){
    GLFWwindow *window;
    unsigned int shaderProgram;
    setupGraphicsWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &window, &shaderProgram);
    
    float vertices1[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    unsigned int VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    
    unsigned int VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //specify how to draw vertices
    //unsigned int indices[] = {
    //    0,1,2,
    //    1,2,3
    //};
    //
    //unsigned int EBO[1];
    //glGenBuffers(1, EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       
        float timeVal = glfwGetTime();
        float blueVal = (sin(M_PI+timeVal*6)+1.0f)/2.0f/3.0f;
        int vertexColorLoc = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLoc, 0.0f, 0.0f, blueVal, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); 
        glBindVertexArray(0);  
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
    glfwTerminate();
    return 0;
}
