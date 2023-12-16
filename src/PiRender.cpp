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
#include <chrono>

using namespace std::chrono;

static glm::mat4 gMatrix((1.0f));

#define FPS      60.0f
#define MS_PER_FRAME ((int)(1.0f / FPS * 1000.0f))

#include <GLFW/glfw3.h>
#include <iostream>

#define RES_W  1280.0f
#define RES_H   720.0f

int main()
{
    // piShader shader;

    // shader.addUniforms( {
    //     { "pos1", "ivec2" },
    //     { "pos3", "vec2"  },
    //     { "pos2", "float" },
    //     { "pos1", "ivec2" }
    // });

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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color with alpha

    glViewport(0, 0, RES_W, RES_H);

    // Set up a simple orthographic projection matrix
    glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    piScene scene;

#define INCLUDE_BG
#ifdef INCLUDE_BG

    piShaderToy background;
    background.setSize( RES_W, RES_H );
    background.setPos(RES_W/2, RES_H/2);

    scene.addObject( background );
    // background.setScale(0.5);

#endif


#define INCLUDE_SCENE
#ifdef INCLUDE_SCENE

#if 0
    piSpritesheet sheet("spritesheet.json");

    piSpritePtr_t pSparkLogo = sheet.getSprite("Spark_logo.png");

    float sw = pSparkLogo->frame.w;
    float sh = pSparkLogo->frame.h;

    piTexturePtr_t sheetTex = sheet.getTexture();
    piImagePtr_t  sparkLogo = piImage::create(sheetTex, pSparkLogo, 200, 200, sw, sh);
    scene.addObject( sparkLogo );
#endif

    piTexturePtr_t   tex = piTexture::create("ball.png");
    piObjectPtr_t  ball0 = piImage::create(tex, RES_W * 0.25f, RES_H * 0.25f);
    scene.addObject( ball0 );

    // piObjectPtr_t ball = piImage::create("ball.png", RES_W * 0.25f, RES_H * 0.25f);
    // scene.addObject( ball );

    piObjectPtr_t face      = piImage::create("Smiling_Face.png",RES_W/4, RES_H/2);
    scene.addObject( face );

    face->setAccX(10.01);
    face->setAccR(10.01);
    face->setAngleDegrees(45.0);
#endif

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
        // SCENE
        //

        scene.update(projection, currentTime);
        scene.draw();

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // Swap buffers
        glfwSwapBuffers(window);

        // Sleep for a bit ...
        std::this_thread::sleep_for(16ms);
    }

    // Cleanup

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
