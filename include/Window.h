#pragma once
#include <string>

#include "GLFW/glfw3.h"

class Window {

private:
    GLFWwindow* window = nullptr;

public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;
    GLFWwindow* getWindow() const;
    void setTitle(const std::string& title) const;

};
