

#include "../include/Renderer.h"

#include <iostream>
#include <ostream>


Renderer::Renderer() {

    //get shader sources
    auto [vertexSource, fragmentSource] = Renderer::getShaderSources();

    //compile shaders and create program
    shaderProgram = Renderer::createProgram(vertexSource, fragmentSource);

    //get needed locations
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    viewLocation = glGetUniformLocation(shaderProgram, "view");
    projectionLocation = glGetUniformLocation(shaderProgram, "projection");

    if (modelLocation == -1 || viewLocation == -1 || projectionLocation == -1) {
        throw std::runtime_error("Cannot find model/view/projection location");
    }

    //tell OpenGL which shader program we want to use
    glUseProgram(shaderProgram);

    const std::vector<float> vertices = CreateCircle();

    vertexCount = vertices.size() / 2;


    Renderer::initVAOVBO(VAO, VBO, vertices.data(), vertexCount);


}











GLuint Renderer::createProgram(const char* vertexSource, const char* fragmentSource) {

    //create vertex shader and get reference
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attach vertex shader source to the vertex object
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    //compile vertex shader into machine code
    glCompileShader(vertexShader);

    //make sure vertex shader was created
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    //create fragment shader and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //attach fragment shader source to the vertex object
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    //compile fragment shader into machine code
    glCompileShader(fragmentShader);

    //make sure that fragment shader was created
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    //create the shader program and get reference
    const GLuint shaderProgram = glCreateProgram();

    //attach shaders to shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    //make sure shader program was generated successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    //delete shaders now that they are in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::pair<const char*, const char*> Renderer::getShaderSources() {
    auto vertexSource = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    auto fragmentSource = R"(
        #version 330 core

        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.2, 0.6, 1.0, 1.0);
        }
    )";

    return {vertexSource, fragmentSource};
}