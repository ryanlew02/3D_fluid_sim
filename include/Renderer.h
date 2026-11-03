#pragma once
#include <utility>
#include <vector>

#include "Camera.h"
#include "glad/gl.h"

class Renderer {

public:

    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void drawSphere(const Particle& particle) const;
    static std::vector<float> createSphere(float radius = 1.0f, int stacks = 20, int sectors = 30);
    void setupMeshBuffers(const float* vertices, size_t vertexCount);
    void setCamera(const Camera& camera, float aspectRatio) const;


    static void clear();


private:

    GLuint VAO = 0, VBO = 0;
    GLuint shaderProgram = 0;
    GLsizei vertexCount_ = 0;

    GLint modelLocation = -1;
    GLint viewLocation = -1;
    GLint projectionLocation = -1;


    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);
    static std::pair<const char*, const char*> getShaderSources();

};

