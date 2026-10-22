#pragma once
#include <utility>
#include <vector>

#include "glad/gl.h"

class Renderer {

public:

    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void drawSphere();
    static std::vector<float> createSphere(const float centerX, const float centerY, const float centerZ, const float radius);

    void initialize();
    void clear() const;
    void render() const;


private:

    GLuint VAO = 0, VBO = 0;
    GLuint shaderProgram = 0;

    GLint modelLocation = -1;
    GLint viewLocation = -1;
    GLint projectionLocation = -1;

    int vertexCount = 0;

    void setupBuffers();
    void setupShaders();

    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);
    static std::pair<const char*, const char*> getShaderSources();

};

