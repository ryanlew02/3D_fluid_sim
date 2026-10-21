//
// Created by ryanl on 10/20/2026.
//
#pragma once
#include "Window.h"


class App {
private:
    Window window;
    //Renderer renderer;

    void update();
    void render();

public:

    App();
    void run();
};

