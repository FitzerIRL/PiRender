#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <piUtils.h>

// Vertex shader source code
const char* sourceVertex =

R"(
    attribute vec4 aPosition;
    attribute vec2 aTexCoord;

    uniform mat4 u_mvpMatrix;
    varying vec2  v_TexCoord;

    void main()
    {
        gl_Position = u_mvpMatrix * vec4(aPosition.xyz, 1.0);
        v_TexCoord  = aTexCoord;

        // v_TexCoord  = (aPosition * 0.5 + 0.5) * vec2(1.0, -1.0); // Flip y-axis
    }
)";

// Fragment shader source code
const char* sourceFragment = R"(

    varying vec2 v_TexCoord;

    uniform sampler2D textureSampler;

    void main()
    {
        gl_FragColor = texture2D(textureSampler, v_TexCoord);
        // vec4 pixel = texture2D(textureSampler, v_TexCoord);
        // gl_FragColor = vec4(0.0, 0.0, 1.0, pixel.a); //BLUE
    }
)";

// Window dimensions
#define RES_W  1280.0f / 2.0f
#define RES_H   720.0f / 2.0f

// Vertex data for a rectangle
GLfloat vertices[] = {
    // Vertex           // Texture
   -0.5f, -0.5f, 0.0f,       0.0f, 0.0f,  // bottom-left
    0.5f, -0.5f, 0.0f,       1.0f, 0.0f,  // bottom-right
   -0.5f,  0.5f, 0.0f,       0.0f, 1.0f,  // top-left
    0.5f,  0.5f, 0.0f,       1.0f, 1.0f   // top-right
};

GLuint indices[] = {
    0, 1, 2,
    1, 3, 2
};

// Vertex attribute locations
// GLuint posAttrib  = 0;
// GLuint texCoordAttrib = 1;

// Model matrix uniform location
GLint u_mvpMatrix;
GLint u_sampler2D;

// Model matrix for transformations
glm::mat4 mvpMatrix;
glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D

// GLFW callback for handling window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    fprintf(stderr, "DEBUG: framebuffer_size_callback() - ENTER\n ");
    glViewport(0, 0, width, height);
}


int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // Set GLFW to create an OpenGL ES 2.0 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(RES_W, RES_H, "OpenGL ES 2.0", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the window position (replace these values with the desired position)
    glfwSetWindowPos(window, 2500, 750);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    // Set up OpenGL ES 2.0
    GLuint shaderProgram = piUtils::createProgramFromSource( sourceVertex, sourceFragment );
    glUseProgram(shaderProgram);
    checkGLError("createProgramFromSource() ... ");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    GLuint VBO, EBO;

    // Set up vertex data and buffers
    glGenBuffers(1, &VBO);
    checkGLError("piImage::piImage() >> glGenBuffers ... VBO");

    glGenBuffers(1, &EBO);
    checkGLError("piImage::piImage() >> glGenBuffers ... EBO");

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGLError("piImage::piImage() >> glBufferData ... VBO");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    checkGLError("piImage::piImage() >> glBufferData ... EBO");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UNIFORMS
    //

    u_sampler2D = GET_UNIFORM_LOCATION(shaderProgram, "textureSampler");
    u_mvpMatrix = GET_UNIFORM_LOCATION(shaderProgram,  "u_mvpMatrix");

    fprintf(stderr, "DEBUG:         u_sampler2D: %d\n", u_sampler2D);
    fprintf(stderr, "DEBUG:         u_mvpMatrix: %d\n", u_mvpMatrix);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ATTRIBUTES
    //

    GLuint posAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    checkGLError("piImage::piImage() >> Create - aPosition .... INIT ");

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    checkGLError("piImage::piImage() >> Create - posAttrib");

    GLuint texCoordAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
    checkGLError("piImage::piImage() >> Create - aTexCoord .... INIT ");

    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Create - texCoordAttrib");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    // Load texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height;
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically (OpenGL expects the top-left corner as the origin)

    unsigned char* data = stbi_load("ball.png", &width, &height, NULL, STBI_rgb_alpha);

    if (data)
    {
        checkGLError("glGenTextures ... ");

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "DEBUG: stbi_load() - FAILED\n ");
        exit(-1);
    }
    stbi_image_free(data);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // | Sx   0   0   Tx |
    // | 0    Sy  0   Ty |
    // | 0    0   Sz  Tz |
    // | 0    0   0    1 |

    float pos_x   = RES_W * 0.3f, pos_y  = RES_H * 0.3f;
    float scale_x = 1.0, scale_y = 1.0;
    float angle = 0.0;

    float obj_w = 160.0f;
    float obj_h = 160.0f;

    glm::vec3 about_y_axis = glm::vec3(0.0f, 1.0f, 0.0f);

    // Apply transformations
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y,     0.0f)) *
                      glm::rotate(   glm::mat4(1.0f), glm::radians(angle), about_y_axis) *
                      glm::scale(    glm::mat4(1.0f), glm::vec3(scale_x * obj_w, scale_y * obj_h, 1.0f));

    mvpMatrix = projection * model;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.15f, 0.0f, 1.0f);  // Black background

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);                // Bind
        glBindBuffer(GL_ARRAY_BUFFER, VBO);         // Bind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind

        glBindTexture(GL_TEXTURE_2D, texture);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // Update UNIFORM values ...
        //
        glUniform1i(u_sampler2D, 0);

        glUniformMatrix4fv(u_mvpMatrix, 1, GL_FALSE, glm::value_ptr( mvpMatrix ));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Disable the vertex attribute arrays
    glDisableVertexAttribArray(posAttrib );
    glDisableVertexAttribArray(texCoordAttrib);

    // Terminate GLFW
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


/*

    // | Sx   0   0   Tx |
    // | 0    Sy  0   Ty |
    // | 0    0   Sz  Tz |
    // | 0    0   0    1 |

*/
