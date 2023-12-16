#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <vector>

// #include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <piUtils.h>

// Vertex shader source code
const char* vertexShaderSource =

R"(
    attribute vec4 aPosition;
    attribute vec2 aTexCoord;

    uniform mat4 u_projection;   // Projection matrix uniform
    uniform mat4 u_model;        // Model matrix uniform

    varying vec2  v_TexCoord;

    void main()
    {
        gl_Position = u_projection * u_model * vec4(aPosition.xyz, 1.0);
        v_TexCoord  = aTexCoord;

        // v_TexCoord  = (aPosition * 0.5 + 0.5) * vec2(1.0, -1.0); // Flip y-axis
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(

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
GLuint posAttrib  = 0;
GLuint texCoordAttrib = 1;

// Model matrix uniform location
GLint u_model;
GLint u_projection;
GLint u_sampler2D;

// Model matrix for transformations
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::ortho(0.0f, RES_W, 0.0f, RES_H, -1.0f, 1.0f); // 2D

// GLFW callback for handling window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    fprintf(stderr, "DEBUG: framebuffer_size_callback() - ENTER\n ");
    glViewport(0, 0, width, height);
}


// Macro for getting uniform location and checking for errors
#define GET_UNIFORM_LOCATION(program, uniformName) \
    glGetUniformLocation(program, uniformName); \
    checkGLError("piImage::piImage() >> glGetUniformLocation ... " uniformName)


int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Set GLFW to create an OpenGL ES 2.0 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(RES_W, RES_H, "OpenGL ES 2.0", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }


    // Set the window position (replace these values with the desired position)
    glfwSetWindowPos(window, 2500, 750);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the callback for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set up OpenGL ES 2.0
    GLuint shaderProgram = piUtils::createProgramFromSource(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    checkGLError("createProgramFromSource() ... ");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UNIFORMS
    //

    u_sampler2D     = GET_UNIFORM_LOCATION(shaderProgram, "textureSampler");
    u_model         = GET_UNIFORM_LOCATION(shaderProgram, "u_model");
    u_projection    = GET_UNIFORM_LOCATION(shaderProgram, "u_projection");

    fprintf(stderr, "DEBUG:         u_sampler2D: %d\n", u_sampler2D);
    fprintf(stderr, "DEBUG:             u_model: %d\n", u_model);
    fprintf(stderr, "DEBUG:        u_projection: %d\n", u_projection);

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
    // ATTRIBUTES
    //
static float ang = 0.0f;
// ball.setAngleDegrees( ang++);SE, 5 * sizeof(GLfloat), (GLvoid*)0);
    checkGLError("piImage::piImage() >> Create - aPosition");

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

    glm::vec3 translation  = glm::vec3(pos_x, pos_y, 0.0f);
    glm::vec3 scale        = glm::vec3(scale_x, scale_y, 1.0);
    glm::vec3 about_y_axis = glm::vec3(0.0f, 1.0f, 0.0f);

    model = glm::mat4(1.0f); // identity

    // Apply transformations
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, 0.0f)) *
                      glm::rotate(   glm::mat4(1.0f), glm::radians(angle), about_y_axis) * // Rotate by 45 degrees
                      glm::scale(    glm::mat4(1.0f), glm::vec3(scale.x * obj_w, scale.y * obj_h, 1.0f));

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the clear color
    glClearColor(0.0f, 0.15f, 0.0f, 1.0f);  // Black background

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);         // Bind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind

        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        //
        // Update UNIFORM values ...
        //
        glUniform1i(u_sampler2D, 0);

        // Set the model matrix uniform in the shader
        glUniformMatrix4fv(u_model,      1, GL_FALSE, glm::value_ptr( model ));
        glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr( projection ));

        // Draw the quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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