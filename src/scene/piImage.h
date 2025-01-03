#include <GLES2/gl2.h>
#pragma once

#include <piObject.h>
#include <piTexture.h>
#include <piShaderManager.h>
#include <piSpritesheet.h>

#include <vector>

class piImage; // fwd
typedef std::shared_ptr<piImage> piImagePtr_t;


class piImage : public piObject
{
public:
     piImage();
     piImage(piTexturePtr_t tex);
     piImage(piTexturePtr_t tex, float px, float py);
     piImage(piTexturePtr_t tex, float px, float py, float ww, float hh);

     piImage(piTexturePtr_t tex, piSpritePtr_t sp);
     piImage(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py);
     piImage(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py, float ww, float hh);

     piImage(const char *path);
     piImage(const char *path, float px, float py);
     piImage(const char *path, float px, float py, float ww, float hh);


    // Factory function
    static auto create()                                                                              { return std::make_shared<piImage>();                        }
    static auto create(piTexturePtr_t tex)                                                            { return std::make_shared<piImage>(tex);                     }
    static auto create(piTexturePtr_t tex, float px, float py)                                        { return std::make_shared<piImage>(tex, px, py);         }
    static auto create(piTexturePtr_t tex, float px, float py, float ww, float hh)                    { return std::make_shared<piImage>(tex, px, py, ww, hh); }
    static auto create(piTexturePtr_t tex, piSpritePtr_t sp)                                          { return std::make_shared<piImage>(tex, sp);                 }
    static auto create(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py)                      { return std::make_shared<piImage>(tex, sp, px, py);         }
    static auto create(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py, float ww, float hh)  { return std::make_shared<piImage>(tex, sp, px, py, ww, hh); }

    static auto create(const char *path)                                                              { return std::make_shared<piImage>(path);                    }
    static auto create(const char *path, float px, float py)                                          { return std::make_shared<piImage>(path, px, py);            }
    static auto create(const char *path, float px, float py, float ww, float hh)                      { return std::make_shared<piImage>(path, px, py, ww, hh);    }

    ~piImage();

    void draw();

private:

    piShaderPtr_t shader;

    piTexturePtr_t texture;

    GLuint VBO, EBO;

    GLuint shaderProgram_;
    GLuint u_sampler2D;

    GLuint posAttrib;
    GLuint colorAttrib;
    GLuint texCoordAttrib;

    void commonSetup();

}; // CLASS - piImage

