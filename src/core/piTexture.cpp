#include <piTexture.h>
#include <piUtils.h>

#include <filesystem>

#include <GLES2/gl2.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//======================================================================================================

piTexture::piTexture() : filename_(""), textureW_(0), textureH_(0), textureID_(0)
{
}

piTexture::piTexture(piTexturePtr_t tex)
{
    this->textureID_ = tex->texID();
    this->textureW_  = tex->width();
    this->textureH_  = tex->height();

    this->filename_  = tex->name();
}

//======================================================================================================

piTexture::piTexture(const char *path) : piTexture()
{
    loadTexture(path);
}

//======================================================================================================
// Load PNG image using stb_image
void piTexture::loadTexture(const char* filename, GLuint* texID, int* width, int* height)
{
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically (OpenGL expects the top-left corner as the origin)

    unsigned char* image = stbi_load(filename, width, height, NULL, STBI_rgb_alpha);

    if(!image)
    {
        printf("\nERROR:   piTexture::loadTexture()  ...  Failed to load image");
        printf("\nERROR:   piTexture::loadTexture()  ...  Failed to load image");
        printf("\nERROR:   piTexture::loadTexture()  ...  Failed to load image");
        printf("\nERROR:   piTexture::loadTexture()  ...  Failed to load image");
        return;
    }

    std::filesystem::path filePath(filename);
    filename_ = filePath.filename().string();

    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

   checkGLError("Texture... 123");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

   checkGLError("Texture... 456");


    // Print the extracted filename
    std::cout <<  std::endl << "DEBUG: piTexture::loadTexture(path) ... texID: " << *texID << " ... WxH: " << *width << " x " << *height << std::endl << std::endl;

    stbi_image_free(image);
}

//======================================================================================================

void piTexture::loadTexture(const char *path)
{
    printf("\nDEBUG: piTexture::loadTexture(path) ... path: %s", path);

    // Load texture
    loadTexture(path, &textureID_, &textureW_, &textureH_);

    fprintf(stderr, "DEBUG: piTexture::loadTexture(%s) - WxH: %d x %d \n",
        path, textureW_, textureH_);
}

//======================================================================================================

piTexture::~piTexture()
{
    // printf("\nDEBUG: piTexture::~piTexture() ... ENTER >> textureID_: %d", textureID_);
    // printf("\nDEBUG: piTexture::~piTexture() ... ENTER >> textureID_: %d", textureID_);
    // printf("\nDEBUG: piTexture::~piTexture() ... ENTER >> textureID_: %d", textureID_);

    // Cleanup
    glDeleteTextures(1, &textureID_);
}

//======================================================================================================
//======================================================================================================
