#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

class piTexture; // fwd
typedef std::shared_ptr<piTexture> piTexturePtr_t;

class piTexture
{
public:
     piTexture();
     piTexture(piTexturePtr_t tex);
     piTexture(const char *path);
    ~piTexture();

    // Factory function
    static auto create()                     { return std::make_shared<piTexture>();             }
    static auto create(piTexturePtr_t tex)   { return std::make_shared<piTexture>(tex);          }
    static auto create(const char *path)     { return std::make_shared<piTexture>(path);         }
    static auto create(std::string path)     { return std::make_shared<piTexture>(path.c_str()); }

    // Load PNG image using stb_image
    void loadTexture(const std::string path) { loadTexture(path.c_str() ); };
    void loadTexture(const char *path);
    void loadTexture(const char* filename, GLuint* texID, int* width, int* height);

    std::string name()   const  { return filename_;  }
    GLuint      texID()  const  { return textureID_; }
    GLsizei     width()  const  { return textureW_;  }
    GLsizei     height() const  { return textureH_;  }

    void setTexID(GLuint id)    { textureID_ = id; }
    void setWidth(GLsizei w)    { textureW_  =  w; }
    void setHeight(GLsizei h)   { textureH_  =  h; }


friend std::ostream& operator<<(std::ostream& os, const piTexture& tex) {
    os << "name: " << tex.filename_  << "\n"
        << "id: "  << tex.textureID_ << "\n"
        << "W: "   << tex.textureW_  << "\n"
        << "H: "   << tex.textureH_  << "\n";
    return os;
}

private:
    std::string filename_;

    GLsizei textureW_, textureH_;
    GLuint textureID_;

}; // class - piTexture

// Operator<< for piSpritePtr_t
inline std::ostream& operator<<(std::ostream& os, const piTexturePtr_t& ptr) {
    if (ptr) {
        os << *ptr; // Dereference the pointer and use the existing operator<< for piSprite
    } else {
        os << "nullptr"; // Or any other representation for a null pointer
    }
    return os;
}