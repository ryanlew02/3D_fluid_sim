//
// Created by ryanl on 10/20/2026.
//
#include "../include/App.h"


App::App() : window(800, 800, "Fluid Simulator"), particle(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.5f) {

}

void App::run() {

    glClearColor(0.5f, 0.1f, 0.1f, 1.0f);

    int width, height;

    while (!window.shouldClose()) {

        renderer.clear();

        //set camera and draw sphere
        renderer.setCamera(camera, 1.0f);
        renderer.drawSphere(particle);

        //swap buffers and check for event updates
        window.swapBuffers();
        window.pollEvents();
    }

}