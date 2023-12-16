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


static glm::mat4 gMatrix((1.0f));

#define FPS      60.0f
#define MS_PER_FRAME ((int)(1.0f / FPS * 1000.0f))

#if 0

#define RES_W  1280.0f
#define RES_H   720.0f

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

    std::pair<bool, Sprite> sp = sheet.getSprite("1of5stars.png");

    const Sprite& sprite = sp.second;

    std::cout << " JSON: " << std::endl << sprite << std::endl;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color with alpha

    glViewport(0, 0, RES_W, RES_H);

    piShader background;

    background.addUniform("iResolution", "vec2");
    background.addUniform("iTime", "float");

    background.initShader(vertexShaderFile, fragmentShaderFile);
    background.use();

    // background.setUniformVal("iResolution", glm::vec2(RES_W, RES_H));
    background.setUniformVal("u_resolution", glm::vec2(RES_W, RES_H));

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ATTRIBUTES
    //
    // Get the attribute location from the shader program

    // GLint posAttrib = glGetAttribLocation(background.program(), "vPosition");
    // fprintf(stderr,"INFO: main() - posAttrib: %d \n", posAttrib);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int count = 5;

    while (!glfwWindowShouldClose(window) && count-- >0)
    {
        float currentTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT );

        checkGLError("Background - RENDER ... ");

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // BACKGROUND
        //

        float* matrixData = glm::value_ptr(gMatrix);

        float x = 0, y = 0;
        float w = RES_W, h = RES_H;


        const float verts[4][2] =
        {
            { x  , y   },
            { x+w, y   },
            { x  , y+h },
            { x+w, y+h }
        };

        background.use();
        background.setUniformVal("a_color", glm::vec4(1.0, 0.0, 0.0, 1.0 )); // RED
        background.setUniformVal("u_alpha", (float) 1.0f);

        background.setUniformVal("iTime", currentTime);
        // background.setUniformVal("u_time", currentTime);

        // std::cout << std::endl << " gMatrix: " << gMatrix << std::endl;

        background.draw(RES_W, RES_H, matrixData, 1.0,
                            0,
                            GL_TRIANGLES, // mode
                            vertices, //verts,
                            NULL,
                            6);


// piError shaderProgram::draw(int resW, int resH, float* matrix, float alpha,
//                             piTextureRef t,
//                             GLenum mode,
//                             const void* pos,
//                             const void* uv,
//                             int count)
// {



// fprintf(stderr,"INFO: while() - DONE \n");

        checkGLError("Background - Done ... ");
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // SCENE
        //
        // scene.update(projection, currentTime);
        // scene.draw();

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Sleep for 2 milseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
    } // WHILE

    // glDeleteProgram(backgroundProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
//======================================================================================================
//======================================================================================================





#else
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


#define RES_W  1280.0f
#define RES_H   720.0f


const char* vertexShaderSource = R"(
    attribute vec2 position;
    uniform mat4 modelViewProjection;
    void main() {
        gl_Position = vec4(position, 0.0, 1.0) * modelViewProjection;
    }
)";

const char* fragmentShaderSource = R"(
    // precision mediump float;
    void main() {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

void compileShader(GLuint shader, const char* source)
{
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for shader compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
    }
}

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
    glfwSwapInterval(1); // 1 for vsync, 0 for no vsync

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertexShader, vertexShaderSource);

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragmentShader, fragmentShaderSource);

    // Create and link the shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for program linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking error:\n" << infoLog << std::endl;

        return EXIT_FAILURE;
    }

    checkGLError("Program... ");

    // Use the shader program
    glUseProgram(shaderProgram);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color with alpha

    glViewport(0, 0, RES_W, RES_H);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    float x = 0.0f, y = 0.0f;
    float w = 2.0f, h = 2.0f;

    const float verts[4][2] =
    {
        { x  , y   },
        { x+w, y   },
        { x  , y+h },
        { x+w, y+h }
    };

    // Create a vertex buffer object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // Specify the vertex attribute pointers
    GLint positionAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Set up a simple orthographic projection matrix
    glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D

    GLint mvpLoc = glGetUniformLocation(shaderProgram, "modelViewProjection");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(projection));

    checkGLError("Setup... ");

// #define INCLUDE_SCENE
#ifdef INCLUDE_SCENE
    piScene scene;
    scene.addObject( new piImage("ball.png",          RES_W * 0.525f, RES_H * 0.25f)) ;
#endif

int count = 125;

    // Main rendering loop
    while (!glfwWindowShouldClose(window) && count-- )
    {
        float currentTime = glfwGetTime();

        // Poll for and process events
        glfwPollEvents();

        // Clear the color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // SCENE
        //
#ifdef INCLUDE_SCENE
        scene.update(projection, currentTime);
        scene.draw();
#endif
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // // Draw the rectangle
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Swap buffers
        glfwSwapBuffers(window);


        checkGLError("Render... ");

        // Sleep for a bit ...
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
    }

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &vbo);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}


#endif