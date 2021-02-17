#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
}

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}\0";


int main(){
    //init glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required on macos    
    GLFWwindow *window = glfwCreateWindow(640, 480, "xD!", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    //setup window size and size callback
    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  
    //setup vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("error: vertex shader compilation failed!\n%s", infoLog);
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
    }

    //setup shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check shader program compile errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("error: shader program - linking failed!\n%s", infoLog);
    }
    //activate shader program
    glUseProgram(shaderProgram);
    //we don't need these no more
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    //create vertices of an object
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        -0.5f, 0.5f, 0.0f,  //top left
         0.5f, -0.5f, 0.0f, //bottom right
         0.5f, 0.5f, 0.0f,  //top right
         0.7f, 0.7f, 0.0f,
         0.9f, 0.7f, 0.0f,
         0.7f, 0.9f, 0.0f
    };
    //specify how to draw vertices
    unsigned int indices[] = {
        0,1,2,
        1,2,3
    };

    // VBO (Vertex Buffer Object)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //move vertices data to VBO

    // VAO (Vertex Array Object)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  

    // set vertex attributes pointers to match vertex data format
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // EBO (Element (vertices) Buffer Object)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //EBO reference is stored in VAO and bound when VAO is bound
    //to define EBO to use with VAO just bind it after binving VAO. it keeps an eye on last-bound EBO
    // ERGO - there is only need to bind VAO, EBO is bound automatically

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);  
        glDrawArrays(GL_TRIANGLES, 4, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);  
        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
    glfwTerminate();
    return 0;
}
