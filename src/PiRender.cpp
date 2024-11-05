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
#include <piAnimation.h>

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

    // Specify GLFW options, including double buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

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

    piShaderToyPtr_t background = piShaderToy::create();
    background->setAnchor( 0.5, 0.5 );
    background->setSize( RES_W, RES_H );
    background->setPos(RES_W/2, RES_H/2);

    scene.addObject( background );
    // background.setScale(0.5);

#endif

#define INCLUDE_SCENE
#ifdef INCLUDE_SCENE

#if 0
    piSpritesheetPtr_t sheet = piSpritesheet::create("spritesheet.json");
    piTexturePtr_t  sheetTex = sheet->getTexture();

    // float sw = sprite->frame.w;
    // float sh = sprite->frame.h;

    // piImagePtr_t sparkLogo = piImage::create(sheetTex, sprite, RES_W/2, RES_H/2, sw, sh);
    piSpritePtr_t spr1 = sheet->getSprite("5of5stars.png");
    piImagePtr_t  obj1 = piImage::create(sheetTex, spr1, 0,0);

    obj1->setAnchor(0.0, 0.0);
    scene.addObject( obj1 );

    piSpritePtr_t  spr2 = sheet->getSprite("4of5stars.png");
    piImagePtr_t   obj2 = piImage::create(sheetTex, spr2, RES_W, RES_H);

    obj2->setAnchor(1.0, 1.0);
    scene.addObject( obj2 );


// std::cout << std::endl << " ##### pSparkLogo: " << sprite;
// std::cout << std::endl << " #####        WxH: " << sw << " x " << sh;
// std::cout << std::endl << " #####   sheetTex: " << sheetTex;

#endif

    // piTexturePtr_t   tex = piTexture::create("ball.png");
    // piObjectPtr_t  ball0 = piImage::create(tex, RES_W * 0.25f, RES_H * 0.25f);
    // scene.addObject( ball0 );

#if 0
    piObjectPtr_t ball = piImage::create("ball.png", 10, RES_H * 0.85);
    ball->setAnchor(0.0, 0.0);

    piAnimatorPtr_t anim = ball->animateProperty(ball->pos, glm::vec2(ball->pos.x, 0), 8 );


    anim->setCallback([ball](piObjectPtr_t obj)
    {
        // std::cout << std::endl << "Callback  >>  name: \"" << obj->getName() << "\" ..." << std::endl;

        obj->setAccX(10.01);
        obj->setAccR(10.01);
    });

    scene.addObject( ball );
#endif

    piObjectPtr_t face = piImage::create("Smiling_Face.png",RES_W/2, RES_H/2);
    scene.addObject( face );
    // face->setAnchor(1.0, 0.0);
    // face->animateProperty( face->pos, glm::vec2(RES_W, face->pos.y), 85 /2 );
    face->setAccX(10.01);
    face->setAccR(100.01);
    face->setAngleDegrees(45.0);

    piObjectPtr_t face1 = piImage::create("Smiling_Face.png",0, 0);
    scene.addObject( face1 );
    face1->setAnchor(0.0, 0.0);

    piObjectPtr_t face2 = piImage::create("Smiling_Face.png",RES_W, RES_H);
    scene.addObject( face2 );
    face2->setAnchor(1.0, 1.0);
#endif

#if 1
    piSpritesheet rgb_sheet("squares.json");

    piSpritePtr_t pRed = rgb_sheet.getSprite("red.png");

    if(pRed)
    {
        float sw = pRed->frame.w;
        float sh = pRed->frame.h;

        piTexturePtr_t rgbTex = rgb_sheet.getTexture();
        piImagePtr_t  redRed = piImage::create(rgbTex, pRed, 200, 200, sw, sh);
        redRed->setAnchor(0.5, 0.5);
        redRed->setPos(RES_W/2, RES_H/2);

        scene.addObject( redRed );
    }
    else
    {
        std::cout << "ERROR: pRed is NULL" << std::endl;
    }


    piSpritePtr_t pBlue = rgb_sheet.getSprite("blue.png");

    if(pBlue)
    {
        float sw = pBlue->frame.w;
        float sh = pBlue->frame.h;

        piTexturePtr_t  rgbTex = rgb_sheet.getTexture();
        piImagePtr_t  blueBlue = piImage::create(rgbTex, pBlue, 200, 200, sw, sh);
        blueBlue->setAnchor(0.5, 0.5);
        blueBlue->setPos(RES_W/6, RES_H/2);
        blueBlue->setAngleDegrees(45.0);

        scene.addObject( blueBlue );
    }
    else
    {
        std::cout << "ERROR: pRed is NULL" << std::endl;
    }
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
