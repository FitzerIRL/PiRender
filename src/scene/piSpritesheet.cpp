#include <piSpritesheet.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

//======================================================================================================

piSpritesheet::piSpritesheet()
{
}

//======================================================================================================

piSpritesheet::piSpritesheet(const std::string& filename)
{
    read(filename);
}

//======================================================================================================

bool piSpritesheet::read(const std::string& path)
{
    // Read JSON file
    std::ifstream file(path);
    json jsonData;
    file >> jsonData;

    // Access the "meta" section
    nlohmann::json meta = jsonData["meta"];

    // Access values within the "meta" section
    std::string app     = meta["app"];
    std::string version = meta["version"];
    std::string image   = meta["image"];
    std::string format  = meta["format"];
    int         sizeW   = meta["size"]["w"];
    int         sizeH   = meta["size"]["h"];
    std::string scale   = meta["scale"];

    // // Print the values
    // std::cout << "App: " << app << std::endl;
    // std::cout << "Version: " << version << std::endl;
    // std::cout << "Image: " << image << std::endl;
    // std::cout << "Format: " << format << std::endl;
    // std::cout << "Size: " << sizeW << "x" << sizeH << std::endl;
    // std::cout << "Scale: " << scale << std::endl;

    filename = meta["image"];

    texture = piTexture::create(filename);

    std::cout << "Sprite Sheet >> filename: " << filename << std::endl;
    std::cout << "Sprite Sheet >> texture: " << texture->texID() << std::endl;

    // std::cout << "====================================== START ..." << std::endl;

    // Parse frames
    json frames = jsonData["frames"];
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
        std::string spriteName = it.key();
        piSpritePtr_t sprite = piSprite::create();

        sprite->sheetSize.x = 0;
        sprite->sheetSize.y = 0;
        sprite->sheetSize.w = sizeW;
        sprite->sheetSize.h = sizeH;

        sprite->name = spriteName;

        sprite->frame.x = it.value()["frame"]["x"];
        sprite->frame.y = it.value()["frame"]["y"];
        sprite->frame.w = it.value()["frame"]["w"];
        sprite->frame.h = it.value()["frame"]["h"];

        sprite->rotated = it.value()["rotated"];
        sprite->trimmed = it.value()["trimmed"];

        sprite->spriteSourceSize.x = it.value()["spriteSourceSize"]["x"];
        sprite->spriteSourceSize.y = it.value()["spriteSourceSize"]["y"];
        sprite->spriteSourceSize.w = it.value()["spriteSourceSize"]["w"];
        sprite->spriteSourceSize.h = it.value()["spriteSourceSize"]["h"];

        sprite->sourceSize.x = 0;
        sprite->sourceSize.y = 0;

        sprite->sourceSize.w = it.value()["sourceSize"]["w"];
        sprite->sourceSize.h = it.value()["sourceSize"]["h"];

        sprites[spriteName] = sprite;

// std::cout << std::endl << " Sprite:  " << sprite.name << " frame: " << sprite.sourceSize << std::endl;
        // std::cout << sprite << ":\n";
    } // FOR

  //  std::cout << std::endl << "====================================== END ..." << std::endl;

    return true;
}

//======================================================================================================

const SpriteMap_t& piSpritesheet::getSprites() const
{
    return sprites;
}

//======================================================================================================

// Function to get a sprite by name
piSpritePtr_t piSpritesheet::getSprite(const std::string& name)
{
    auto it = sprites.find(name);

    if (it != sprites.end())
    {
        // Sprite found, return it
        return it->second;
    }

    // Sprite not found, return a pair indicating failure
    return nullptr;
}

//======================================================================================================
//======================================================================================================

