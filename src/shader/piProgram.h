// #include <GL/glew.h>
#include <GLES2/gl2.h>
#pragma once


class piProgram
{
public:
    static GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    static GLuint createProgramFromFiles(const char *vertexShaderPath, const char *fragmentShaderPath);
    static GLuint createProgramFromSource(const char *vertexShaderSource, const char *fragmentShaderSource);

    static void checkShaderCompilation(GLuint shader);
    static void checkProgramLinking(GLuint program);

    static GLuint compileShader(GLenum type, const char *source);
    static GLuint loadAndCompileShader(const GLuint shaderType, const char *path);

    // New functions for handling VBOs and EBOs
    static GLuint createVBO(const void *data, GLsizei dataSize);
    static GLuint createEBO(const GLuint *indices, GLsizei indexCount);

    static void bindVBO(GLuint vbo);
    static void bindEBO(GLuint ebo);

private:
    // Internal helper function to track and restore OpenGL state
    static void saveAndSetGLState();
    static void restoreGLState();

    // Internal state variables
    static GLint savedProgram;
    static GLint savedVertexArray;
    static GLint savedElementArrayBuffer;
    static GLint savedArrayBuffer;

    // Add more state variables as needed

}; // class piProgram
