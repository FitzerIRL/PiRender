#include <GLES2/gl2.h>
#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>


//======================================================================================================

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);  // Pretty print for glm::vec2
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);  // Pretty print for glm::vec3
std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);  // Pretty print for glm::vec4
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);  // Pretty print for glm::mat4
std::ostream& operator<<(std::ostream& os, const glm::quat& q);    // Pretty print for glm::quat

std::ostream& operator<<(std::ostream& os, const std::vector<GLfloat>& vertices);

void analyzeTransformation(const glm::mat4& model, const char* name = "");

//======================================================================================================

void checkGLError(const char* operation); // fwd

//======================================================================================================

class piUtils
{

public:
    static GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    static GLuint createProgramFromFiles(const char *vertexShaderPath, const char *fragmentShaderPath);
    static GLuint createProgramFromSource(const char *vertexShaderSource, const char *fragmentShaderSource );

    static GLuint createShader(GLenum type, const char *source);

    static char *loadShader(const char *path);

    static void checkShaderCompilation(GLuint shader);
    static void checkProgramLinking(GLuint program);

private:

}; //class


//======================================================================================================

#ifndef GET_UNIFORM_LOCATION

    // Macro for getting uniform location and checking for errors
    #define GET_UNIFORM_LOCATION(program, uniformName) \
        glGetUniformLocation(program, uniformName); \
        checkGLError("piImage::piImage() >> glGetUniformLocation ... " uniformName)
#endif

//======================================================================================================
//======================================================================================================
