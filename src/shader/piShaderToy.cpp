#include <iostream>

#include <piShaderToy.h>
#include <piUtils.h>

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

static const char *vertexShaderFile   =   "vertexShader.glsl";
// static const char *fragmentShaderFile = "fragmentShader.glsl";

static const char *fragmentShaderFile = "fragStarField_1.glsl";

//======================================================================================================

piShaderToy::piShaderToy()
{
    // Vertex data for a rectangle
    GLfloat vertices[] =
    {
            // Vertex
        -0.5f, -0.5f, 0.0f,   // bottom-left
         0.5f, -0.5f, 0.0f,   // bottom-right
        -0.5f,  0.5f, 0.0f,   // top-left
         0.5f,  0.5f, 0.0f,   // top-right
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    shaderProgram_ = piUtils::createProgramFromFiles( vertexShaderFile, fragmentShaderFile );

    glUseProgram(shaderProgram_);
    checkGLError("piShaderToy::piShaderToy() >> shaderProgram_ ... ");

    // Set up vertex data and buffers
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGLError("piShaderToy::piShaderToy() >> glBufferData ... VBO");

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    checkGLError("piShaderToy::piShaderToy() >> glBufferData ... EBO");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UNIFORMS
    //

    iTime          = GET_UNIFORM_LOCATION(shaderProgram_, "iTime");
    iResolutionLoc = GET_UNIFORM_LOCATION(shaderProgram_, "iResolution");
    u_mvpMatrix    = GET_UNIFORM_LOCATION(shaderProgram_, "u_mvpMatrix");

    fprintf(stderr, "DEBUG:               iTime: %d\n", iTime);
    fprintf(stderr, "DEBUG:         iResolution: %d\n", iResolutionLoc);
    fprintf(stderr, "DEBUG:         u_mvpMatrix: %d\n", u_mvpMatrix);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ATTRIBUTES
    //

    posAttrib = glGetAttribLocation(shaderProgram_, "aPosition");
    checkGLError("piShaderToy::piShaderToy() >> Create - aPosition .... INIT ");

    fprintf(stderr, "DEBUG:         posAttrib: %d\n", posAttrib);

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    checkGLError("piShaderToy::piShaderToy() >> Create - posAttrib");

    // Set the resolution (width and height) as a vec2
    GLfloat resolution[2] = {this->getSizeW(), this->getSizeW()};
    glUniform2fv(iResolutionLoc, 1, resolution);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glUseProgram(0);                           // Unbind Program
    glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Unbind EBO

    checkGLError("piShaderToy::piShaderToy() >> Clean up state 11 ");

    // Clean up state
    glDisableVertexAttribArray(posAttrib);

    // checkGLError("piShaderToy::piShaderToy() >> Clean up state 22 ");
}

//======================================================================================================

piShaderToy::piShaderToy(const char *path) : piShaderToy()
{
}

piShaderToy::piShaderToy(const char *path, float px, float py) : piShaderToy(path)
{
    setPosX(px);
    setPosY(py);
}

piShaderToy::piShaderToy(const char *path, float px, float py, float ww, float hh) : piShaderToy(path)
{
    setPosX(px);
    setPosY(py);
    setSizeW(ww);
    setSizeH(hh);
}

//======================================================================================================

piShaderToy::~piShaderToy()
{
    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram_);
}

//======================================================================================================

static int count = 2;

void piShaderToy::update( glm::mat4 &projection, float time_secs)
{
    time = time_secs;

    // if(count-- > 0)
    piObject::update( projection, time_secs );
}

//======================================================================================================

void piShaderToy::draw()
{
    glUseProgram(shaderProgram_);               // Bind Program
    glBindBuffer(GL_ARRAY_BUFFER, VBO);         // Bind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    // Update UNIFORM values ...
    //

    GLfloat resolution[2] = {this->getSizeW(), this->getSizeW()};
    glUniform2fv(iResolutionLoc, 1, resolution);

    glUniformMatrix4fv(u_mvpMatrix, 1, GL_FALSE, glm::value_ptr( mvpMatrix ));

    // Set the TIME uniform in the shader
    glUniform1f(iTime, time);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(posAttrib);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Unbind EBO
    glUseProgram(0);                           // Unbind Program
}

//======================================================================================================
//======================================================================================================
