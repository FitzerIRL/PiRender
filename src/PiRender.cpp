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

#include <piShaderManager.h>
#include <piSpritesheet.h>

#include <iostream>
#include <thread>
#include <chrono>

const char *vertexShaderFile   =   "vertexShader.glsl";
const char *fragmentShaderFile = "fragmentShader.glsl";

// #define RES_W  480.0f
// #define RES_H  320.0f

#define RES_W  1280.0f
#define RES_H   720.0f
#define FPS      60.0f

static glm::mat4 gMatrix((1.0f));

#define MS_PER_FRAME ((int)(1.0f / FPS * 1000.0f))

//======================================================================================================
//

// Vertex data for the rectangle
GLfloat vertices[] = {

    -1.0f,  1.0f,  // Vertex 1 (top left)
    -1.0f, -1.0f,  // Vertex 2 (bottom left)
     1.0f, -1.0f,  // Vertex 3 (bottom right)
     1.0f,  1.0f,  // Vertex 4 (top right)
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
};

GLFWwindow *window;

void errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

//======================================================================================================

int main()
{
    piScene scene;
    piSpritesheet sheet;

    sheet.read("./spritesheet.json");

   // piSprite *pStars0 = sheet.getSprite("0of5stars.png");
    piSprite *pStars1 = sheet.getSprite("1of5stars.png");
    // piSprite *pStars2 = sheet.getSprite("2of5stars.png");
    // piSprite *pStars3 = sheet.getSprite("3of5stars.png");
    // piSprite *pStars4 = sheet.getSprite("4of5stars.png");

    // std::pair<bool, Sprite> sp = sheet.getSprite("1of5stars.png");

    // const Sprite& sprite = sp.second;

    // std::cout << " JSON: " << std::endl << sprite << std::endl;

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(RES_W, RES_H, "OpenGL ES 2.0 Triangle", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set the window position (replace these values with the desired position)
    glfwSetWindowPos(window, 2500, 750);
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 1 for vsync, 0 for no vsync

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // SCENE
    //
    // Set up orthographic projection matrix
    glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D


    piImage padd("STpadd_fg_1280_720.png",          RES_W / 2.0f, RES_H / 2.0f);
    padd.setScale(0.95f);

    scene.addObject( &padd );

    scene.addObject( new piImage("ball.png",          RES_W * 0.525f, RES_H * 0.25f)) ;
    scene.addObject( new piImage("Smiling_Face.png",  RES_W * 0.525f, RES_H * 0.5f)) ;
    // scene.addObject( new piImage("Robin_240_160.png", 240/2, 160/2, 200, 200)) ;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    GLuint backgroundProgram;

    // Set up OpenGL ES 2.0
    backgroundProgram = piUtils::createProgramFromFiles(vertexShaderFile, fragmentShaderFile);
    glUseProgram(backgroundProgram);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color with alpha

    glViewport(0, 0, RES_W, RES_H);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    GLuint backgroundVbo, backgroundEbo;

    // Generate and bind the VBO
    glGenBuffers(1, &backgroundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO
    glGenBuffers(1, &backgroundEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UNIFORMS
    //
    // Get the location of the uniform variable iResolution
    GLuint iResolutionLoc = GET_UNIFORM_LOCATION(backgroundProgram, "iResolution");
    GLuint iTimeLoc       = GET_UNIFORM_LOCATION(backgroundProgram, "iTime");

    // Set the resolution (width and height) as a vec2
    GLfloat resolution[2] = {RES_W, RES_H};
    glUniform2fv(iResolutionLoc, 1, resolution);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ATTRIBUTES
    //
    // Get the attribute location from the shader program

    GLint posAttrib = glGetAttribLocation(backgroundProgram, "vPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    checkGLError("Background - posAttrib ... ");

    // Unbind the VAO to store the EBO binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT );

        checkGLError("Background - RENDER ... ");

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // BACKGROUND
        //

        glUseProgram(backgroundProgram);

        // Set the time uniform in the shader
        glUniform1f(iTimeLoc, currentTime);

        // Bind the VBO + EBO for the background
        glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEbo);

        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Draw background using the EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(posAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);           // Unbind
        glUseProgram(0);                           // Unbind

        checkGLError("Background - Done ... ");
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // SCENE
        //
        scene.update(projection, currentTime);
        scene.draw();

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Sleep for 2 milseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
    } // WHILE

    glDeleteProgram(backgroundProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
//======================================================================================================
//======================================================================================================


