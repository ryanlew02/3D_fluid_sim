

#include "../include/Renderer.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include <numbers>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

    const std::vector<float> vertices = createSphere();

    vertexCount_ = static_cast<GLsizei>(vertices.size() / 3);

    Renderer::setupMeshBuffers(vertices.data(), vertexCount_);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
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

void Renderer::setupMeshBuffers(const float* vertices, const size_t vertexCount) {

    //generate the vao and vbo with 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //make the VAO the current vertex array by binding it
    glBindVertexArray(VAO);

    //bind the VBO specifying array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

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

std::vector<float> Renderer::createSphere(float radius, int stacks, int sectors) {
    std::vector<float> vertices;

    constexpr float PI = std::numbers::pi_v<float>;

    for (int i = 0; i < stacks; i++) {
        const float theta1 = PI * i / stacks;
        const float theta2 = PI * (i + 1) / stacks;

        for (int j = 0; j < sectors; j++) {
            const float phi1 = 2.0f * PI * j / sectors;
            const float phi2 = 2.0f * PI * (j + 1) / sectors;

            auto point = [&](const float theta, const float phi) {
                const float x = radius * sin(theta) * cos(phi);
                const float y = radius * sin(theta) * sin(phi);
                const float z = radius * cos(theta);

                return std::vector<float>{x, y, z};
            };

            auto a = point(theta1, phi1);
            auto b = point(theta2, phi1);
            auto c = point(theta2, phi2);
            auto d = point(theta1, phi2);

            //first triangle
            vertices.insert(vertices.end(), a.begin(), a.end());
            vertices.insert(vertices.end(), b.begin(), b.end());
            vertices.insert(vertices.end(), c.begin(), c.end());

            //second triangle
            vertices.insert(vertices.end(), a.begin(), a.end());
            vertices.insert(vertices.end(), c.begin(), c.end());
            vertices.insert(vertices.end(), d.begin(), d.end());
        }
    }
    return vertices;
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

    //draw sphere
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);

    glBindVertexArray(0);

}