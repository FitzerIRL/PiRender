#include <GLES2/gl2.h>
#pragma once

#include <piObject.h>

class piShaderToy : public piObject
{
public:
     piShaderToy();
     piShaderToy(const char *path);
     piShaderToy(const char *path, float px, float py);
     piShaderToy(const char *path, float px, float py, float ww, float hh);
    ~piShaderToy();

     void draw();
     void update( glm::mat4 &projection, float time_secs);

private:

    GLuint VBO, EBO;

    GLuint shaderProgram_;
    GLuint posAttrib;

    GLuint iResolutionLoc;

}; // class