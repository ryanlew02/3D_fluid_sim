#pragma once
#include <utility>
#include <vector>

#include "Camera.h"
#include "glad/gl.h"
#include "Particle.h"

struct MeshData;

class Renderer {

public:

    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void drawSphere(const Particle& particle) const;
    static MeshData createSphere(float radius = 1.0f, int stacks = 20, int sectors = 30);
    void setupMeshBuffers(const MeshData& meshData);
    void setCamera(const Camera& camera, float aspectRatio) const;
    void createBox();


    static void clear();


private:

    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint shaderProgram = 0;
    GLsizei vertexCount_ = 0;
    GLsizei indexCount_ = 0;

    GLint modelLocation = -1;
    GLint viewLocation = -1;
    GLint projectionLocation = -1;


    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);
    static std::pair<const char*, const char*> getShaderSources();

};


struct MeshData {

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

};