//
// Created by ryanl on 10/20/2026.
//
#pragma once
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"


class App {

public:

    App();
    void run();

private:
    Window window;
    Renderer renderer;
    Camera camera;

    Particle particle;
};

