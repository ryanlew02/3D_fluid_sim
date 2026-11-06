#include "../include/Renderer.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include <numbers>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"

Renderer::Renderer() {

    //enable depth in OpenGL
    glEnable(GL_DEPTH_TEST);

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

    const MeshData sphere = createSphere();

    indexCount_ = static_cast<GLsizei>(sphere.indices.size());

    setupMeshBuffers(sphere);

    createBox();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

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

void Renderer::setupMeshBuffers(const MeshData& mesh) {

    //generate the vao and vbo with 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //make the VAO the current vertex array by binding it
    glBindVertexArray(VAO);

    //bind the VBO specifying array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    //upload triangle vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    //configure the vertex attribute so OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    //enable the vertex attribute so OpenGL knows how to use it
    glEnableVertexAttribArray(0);

    //bind both the VBO and VAO to 0 so we don't accidentally modify them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

MeshData Renderer::createSphere(const float radius, const int stacks, const int sectors) {

    MeshData mesh;

    std::vector<float> vertices;

    constexpr float PI = std::numbers::pi_v<float>;

    //go through the stacks and sectors and get the points for each square to create the triangles
    for (int i = 0; i <= stacks; i++) {
        const float theta = i * PI / stacks;

        for (int j = 0; j <= sectors; j++) {
            const float phi = 2.0f * PI * j / sectors;

            const float x = radius * sin(theta) * cos(phi);
            const float y = radius * cos(theta);
            const float z = radius * sin(theta) * sin(phi);

            mesh.vertices.push_back(x);
            mesh.vertices.push_back(y);
            mesh.vertices.push_back(z);

        }
    }

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < sectors; j++) {
            const unsigned int a = i * (sectors + 1) + j;
            const unsigned int b = (i + 1) * (sectors + 1) + j;
            const unsigned int c = b + 1;
            const unsigned int d = a + 1;

            //first triangle
            mesh.indices.push_back(a);
            mesh.indices.push_back(b);
            mesh.indices.push_back(c);

            //second triangle
            mesh.indices.push_back(a);
            mesh.indices.push_back(c);
            mesh.indices.push_back(d);

        }
    }


    return mesh;
}

void Renderer::drawSphere(const Particle& particle) const {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //create identity matrix
    glm::mat4 model(1.0f);

    //move sphere to particle's position
    model = glm::translate(model, particle.position);

    //scale sphere to particles radius
    model = glm::scale(model, glm::vec3(particle.radius));

    //send model to vertex shader
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    //draw spheres
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

}

void Renderer::setCamera(const Camera& camera, const float aspectRatio) const{
    glUseProgram(shaderProgram);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}

void Renderer::createBox() {





}



