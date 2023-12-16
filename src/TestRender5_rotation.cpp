#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <piShaderManager.h>
#include <piShaderUtils.h>

#include <piUtils.h>
#include <piScene.h>
#include <piImage.h>

#include <piShaderToy.h>
#include <piSpritesheet.h>

#include <iostream>
#include <thread>
#include <chrono>Sprite

using namespace std::chrono;

static glm::mat4 gMatrix((1.0f));

#define FPS      60.0f
#define MS_PER_FRAME ((int)(1.0f / FPS * 1000.0f))

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define RES_W  480.0f
#define RES_H   320.0f

// #define RES_W  1280.0f
// #define RES_H   720.0f

int main()
{
    std::cout << std::fixed; // prevent scientific notation ... "4.28315e-05"

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(RES_W, RES_H, "OpenGL ES 2.0 Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 2500, 750);
    glfwSwapInterval(1); // 1 for vsync, 0 for no vsync

    // Set minimum and maximum window size to the fixed size
    glfwSetWindowSizeLimits(window, RES_W, RES_H, RES_W, RES_H);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color with alpha

    glViewport(0, 0, RES_W, RES_H);

    // Set up a simple orthographic projection matrix
    glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D

    // analyzeTransformation(projection, "ORG projection"); // DEBUG

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // ################################################
    piImage face("Smiling_Face.png", RES_W/2, RES_H/2);
    face.setAngleDegrees(45.0);
    face.setAccR(100.81);
    face.setAccX(5.81);
    // ################################################

    // Main rendering loop
    while (!glfwWindowShouldClose(window) )
    {
        float currentTime = glfwGetTime();

        // Poll for and process events
        glfwPollEvents();

        // Clear the color buffer
        glClearColor(0.0f, 0.00f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // DRAW
        //
        face.update(projection, currentTime);
        face.draw();

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // Swap & Sleep for a bit ...
        glfwSwapBuffers(window);
        std::this_thread::sleep_for(16ms);

    } // WHILE

    // Cleanup

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
