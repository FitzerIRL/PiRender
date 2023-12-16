
#include <piImage.h>
#include <piUtils.h>

#include <piSpritesheet.h>

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//======================================================================================================

// Vertex shader source code
static const char* sourceVertex =

R"(
    attribute vec3 aPosition;
    attribute vec4 aColor;
    attribute vec2 aTexCoord;

    uniform mat4  u_mvpMatrix;
    varying vec4  v_Color;
    varying vec2  v_TexCoord;

    void main()
    {
        gl_Position = u_mvpMatrix * vec4(aPosition.xyz, 1.0);

        v_Color    = aColor;
        v_TexCoord = aTexCoord;

        // v_TexCoord  = (aPosition * 0.5 + 0.5) * vec2(1.0, -1.0); // Flip y-axis
    }
)";

// Fragment shader source code
static const char* sourceFragment = R"(

    varying vec4 v_Color;
    varying vec2 v_TexCoord;

    uniform sampler2D textureSampler;
    uniform float alpha;

    void main()
    {
        // Apply the tint only if the alpha value of v_Color is greater than zero
   //     vec4 color   = texture2D(textureSampler, v_TexCoord);
    //    gl_FragColor = mix(color, color + v_Color, v_Color.a * color.a); //step(0.0, v_Color.a * color.a));



    vec4 originalColor = texture2D(textureSampler, v_TexCoord);
    vec4 tintedColor = vec4(v_Color.rgb, 1.0);

    // Blend between the original color and the tinted color based on v_Color.a
    gl_FragColor = mix(originalColor, tintedColor, originalColor.a > 0.0 ? v_Color.a : 0.0);

// gl_FragColor = vec4(1.0, 0.0, 0.0,  1.0);
        // vec4 color   = texture2D(textureSampler, v_TexCoord);
        // gl_FragColor = vec4(color.rgb, color.a * alpha) * vec4(1.0,0.0,0.0, v_Color.a);
    }
)";

//======================================================================================================

piImage::piImage()
{
    reset();

    shaderProgram_ = piUtils::createProgramFromSource( sourceVertex, sourceFragment );

// shader =
    glUseProgram(shaderProgram_);
    checkGLError("piImage::piImage() >> shaderProgram_ ... ");

    // Set up vertex data and buffers
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    checkGLError("piImage::piImage() >> glBufferData ... VBO");

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    checkGLError("piImage::piImage() >> glBufferData ... EBO");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UNIFORMS
    //

    iTime       = GET_UNIFORM_LOCATION(shaderProgram_, "iTime");
    u_alpha     = GET_UNIFORM_LOCATION(shaderProgram_, "alpha");
    u_sampler2D = GET_UNIFORM_LOCATION(shaderProgram_, "textureSampler");
    u_mvpMatrix = GET_UNIFORM_LOCATION(shaderProgram_, "u_mvpMatrix");

    // fprintf(stderr, "DEBUG:               alpha: %d\n", u_alpha);
    // fprintf(stderr, "DEBUG:               iTime: %d\n", iTime);
    // fprintf(stderr, "DEBUG:         u_sampler2D: %d\n", u_sampler2D);
    // fprintf(stderr, "DEBUG:         u_mvpMatrix: %d\n", u_mvpMatrix);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ATTRIBUTES
    //

    // Attribute:  POSITION
    posAttrib = glGetAttribLocation(shaderProgram_, "aPosition");

    glEnableVertexAttribArray(posAttrib);    // fprintf(stderr, "DEBUG:   posAttrib: %d\n", posAttrib);
    // fprintf(stderr, "DEBUG:      aColor: %d\n", colorAttrib);
    // fprintf(stderr, "DEBUG:   aTexCoord: %d\n", texCoordAttrib);
    // Attribute:  COLOR
    colorAttrib = glGetAttribLocation(shaderProgram_, "aColor");

    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Create - texCoordAttrib");

    // Attribute:  TEX COORDS
    texCoordAttrib = glGetAttribLocation(shaderProgram_, "aTexCoord");

    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Create - texCoordAttrib");

    // fprintf(stderr, "DEBUG:   posAttrib: %d\n", posAttrib);
    // fprintf(stderr, "DEBUG:      aColor: %d\n", colorAttrib);
    // fprintf(stderr, "DEBUG:   aTexCoord: %d\n", texCoordAttrib);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glUseProgram(0);                           // Unbind Program
    glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Unbind EBO

    checkGLError("piImage::piImage() >> Clean up state 11 ");

    // // Clean up state
    // glDisableVertexAttribArray(posAttrib);
    // glDisableVertexAttribArray(colorAttrib);
    // glDisableVertexAttribArray(texCoordAttrib);

    // checkGLError("piImage::piImage() >> Clean up state 22 ");
}

//======================================================================================================

piImage::piImage(piTexturePtr_t tex) : piImage()
{
    texture = tex;

    name = tex->name();
}

piImage::piImage(piTexturePtr_t tex, float px, float py) : piImage(tex)
{
    setPosX(px);
    setPosY(py);
}

piImage::piImage(piTexturePtr_t tex,  float px, float py, float ww, float hh) : piImage(tex, px, py)
{
    setSizeW(ww);
    setSizeH(hh);
}

piImage::piImage(piTexturePtr_t tex, piSpritePtr_t sp) : piImage()
{
// std::cout << " ##### sp: " << *sp << std::endl;
// std::cout << " ##### tex: " << tex.texID() << std::endl;

    texture = tex;

    name = tex->name();

    const float sw = sp->sheetSize.w;
    const float sh = sp->sheetSize.h;

    const float x = sp->frame.x;
    const float y = sp->frame.y;
    const float w = sp->frame.w;
    const float h = sp->frame.h;

    setSize( w, h );

    updateUVCoordinates( x,y, w,h, sw,sh);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);        // Bind VBO
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO
}

piImage::piImage(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py) : piImage(tex, sp)
{
    setPosX(px);
    setPosY(py);
}

piImage::piImage(piTexturePtr_t tex, piSpritePtr_t sp, float px, float py, float ww, float hh) : piImage(tex, sp, px, py)
{
    setSizeW(ww);
    setSizeH(hh);
}

piImage::piImage(const char *path) : piImage()
{
    printf("\nDEBUG: piImage::piImage() ...  ENTER");

    texture.loadTexture(path);

    int w = texture.width();
    int h = texture.height();

    setSize( w, h );

    printf("\nDEBUG: piImage::piImage() ...  EXIT");
    fflush(stdout);
}

piImage::piImage(const char *path, float px, float py) : piImage(path)
{
    setPosX(px);
    setPosY(py);
}

piImage::piImage(const char *path, float px, float py, float ww, float hh) : piImage(path)
{
    setPosX(px);
    setPosY(py);
    setSizeW(ww);
    setSizeH(hh);
}

//======================================================================================================

piImage::~piImage()
{
    GLuint texID = texture.texID();

    // Cleanup
    glDeleteBuffers(1,  &VBO);
    glDeleteBuffers(1,  &EBO);
    glDeleteTextures(1, &texID );
}

//======================================================================================================

void piImage::draw()
{
    GLuint texID = texture.texID();

   // printf("\nDEBUG: piImage::draw() ... textureID: %u ...", texID);

    glUseProgram(shaderProgram_);               // Bind Program
    glBindBuffer(GL_ARRAY_BUFFER, VBO);         // Bind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO
    glBindTexture(GL_TEXTURE_2D, texID);        // Bind Texture

    if(dirty_uv)
    {
        printf("\nDEBUG  piImage::draw() ... dirty_uv   #### ");

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        dirty_uv = false;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    // Update UNIFORM values ...
    //

    // Set the ALPHA uniform in the shader
    glUniform1f(u_alpha, alpha_);

    glUniformMatrix4fv(u_mvpMatrix, 1, GL_FALSE, glm::value_ptr( mvpMatrix ));
    checkGLError("piImage::draw() >> Draw - mvpMatrix");

    // POSITION
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Draw - posAttrib");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // COLOR
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Draw - colorAttrib");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // TEXTURE
    glUniform1i(u_sampler2D, 0);

    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
    checkGLError("piImage::piImage() >> Draw - texCoordAttrib");
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // TODO:  Only upload to GPU if changes in data
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    checkGLError("piImage::draw() >> glBufferData");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // DRAW
    //
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    checkGLError("piImage::draw() >> glDrawElements");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Unbind EBO
    glBindTexture(GL_TEXTURE_2D, 0);           // Unbind Texture
    glUseProgram(0);                           // Unbind Program

    checkGLError("piImage::piImage() >> Clean up state 11 ");

    // Clean up state
    glDisableVertexAttribArray(posAttrib);
    glDisableVertexAttribArray(colorAttrib);
    glDisableVertexAttribArray(texCoordAttrib);

    // checkGLError("piImage::piImage() >> Clean up state 22 ");
}

//======================================================================================================
//======================================================================================================
