//
// Created by ryanl on 10/20/2026.
//

#include "../include/Window.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <stdexcept>

Window::Window(int width, int height, const char* title) {

    if (!glfwInit()) {
        throw std::runtime_error("glfwInit failed");
    }

    // macOS OpenGL configuration
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Windows OpenGL configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        const char* description;
        const int error = glfwGetError(&description);
        std::cerr << "GLFW Error: " << error << std::endl;
        glfwTerminate();
        throw std::runtime_error("glfwTerminate failed");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("GLAD initialization failed");
    }

    glViewport(0, 0, width, height);

}

Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

GLFWwindow* Window::getWindow() const {
    return window;
}

void Window::setTitle(const std::string& title) const {
    glfwSetWindowTitle(window, title.c_str());
}



