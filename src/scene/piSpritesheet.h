#pragma once

#include <map>
#include <string>

#include <fstream>
#include <iostream>

#include <memory>
#include <piTexture.h>

//======================================================================================================
//======================================================================================================

class Frame {
public:
    int x, y, w, h;

    friend std::ostream& operator<<(std::ostream& os, const Frame& frame) {
        os << "{x: " << frame.x << ", y: " << frame.y << ", w: " << frame.w << ", h: " << frame.h << "}";
        return os;
    }
};


class piSprite; // fwd
typedef std::shared_ptr<piSprite> piSpritePtr_t;

class piSprite {
public:

    static piSpritePtr_t create() { return std::make_shared<piSprite>(); }

    std::string name;

    Frame frame;
    bool rotated;
    bool trimmed;

    Frame sheetSize;
    Frame spriteSourceSize;
    Frame sourceSize;

    friend std::ostream& operator<<(std::ostream& os, const piSprite& sprite) {
        os << "name: " << sprite.name << "\n"
           << "frame: " << sprite.frame << "\n"
           << "rotated: " << std::boolalpha << sprite.rotated << "\n"
           << "trimmed: " << std::boolalpha << sprite.trimmed << "\n"
           << "spriteSourceSize: " << sprite.spriteSourceSize << "\n"
           << "sheetSize: " << sprite.sheetSize << "\n"
           << "sourceSize: " << sprite.sourceSize;
        return os;
    }
}; // CLASS - piSprite


//======================================================================================================
//======================================================================================================

typedef std::map<std::string, piSpritePtr_t> SpriteMap_t;

class piSpritesheet
{
public:

    piSpritesheet();
    piSpritesheet(const std::string& path);

    bool read(const std::string& path);

    const SpriteMap_t& getSprites() const;

    //piSprite* getSprite(const std::string& name);

    piSpritePtr_t getSprite(const std::string& name);

    int getW()  const  { return texture->width();  }
    int getH()  const  { return texture->height(); }

    //piTexture&     getTexture()  { return texture; }
    piTexturePtr_t getTexture()  { return texture; }

private:

  //  piTexture   texture;
    piTexturePtr_t texture;

    std::string filename;
    SpriteMap_t sprites;

};//class




