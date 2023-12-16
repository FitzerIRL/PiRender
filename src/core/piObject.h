#include <GLES2/gl2.h>
#pragma once

#include <string>
#include <vector>
#include <memory>

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>


class piObject
{
public:

     piObject();
    ~piObject();

    bool operator==(const piObject& other) const {
        // Compare all relevant members for equality
        return u_mvpMatrix == other.u_mvpMatrix &&
               iTime == other.iTime &&
               u_alpha == other.u_alpha &&
               alpha_ == other.alpha_ &&
               textureWidth == other.textureWidth &&
               textureHeight == other.textureHeight &&
               obj_w == other.obj_w &&
               obj_h == other.obj_h &&
               scale_x == other.scale_x &&
               scale_y == other.scale_y &&
               pos_x == other.pos_x &&
               pos_y == other.pos_y &&
               vel_x == other.vel_x &&
               vel_y == other.vel_y &&
               acc_x == other.acc_x &&
               acc_y == other.acc_y &&
               vel_r == other.vel_r &&
               acc_r == other.acc_r &&
               angle == other.angle &&
               angle_deg == other.angle_deg &&
               time == other.time &&
               last_secs == other.last_secs &&
               name == other.name &&
               vertices == other.vertices &&
               indices == other.indices &&
               mvpMatrix == other.mvpMatrix;
    }

    void reset();

    virtual void draw() {}; // pure virtual
    virtual void update(glm::mat4 &projection, float time_secs = 0.0f);

    inline void setSize( float ww, float hh) { dirty = true; obj_w = ww; obj_h = hh; };
    inline void setSizeW(float sw)           { dirty = true; obj_w = sw; };
    inline void setSizeH(float sh)           { dirty = true; obj_h = sh; };

    inline float getSizeW()                  { return obj_w; }
    inline float getSizeH()                  { return obj_h; }

    inline void setPos( float px, float py) { dirty = true; pos_x = px; pos_y = py; };
    inline void setPosX(float px)           { dirty = true; pos_x = px; };
    inline void setPosY(float py)           { dirty = true; pos_y = py; };

    inline float getPosX()                  { return pos_x; }
    inline float getPosY()                  { return pos_y; }

    inline void setScale( float ss)         { dirty = true; scale_x = ss; scale_y = ss; };
    inline void setScaleX(float sx)         { dirty = true; scale_x = sx; };
    inline void setScaleY(float sy)         { dirty = true; scale_y = sy; };

    void setAngleDegrees(float aa)          { dirty = true; angle_deg = aa; } //angle = aa * M_PI / 180.0f; };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    // PHYSICS
    //
    inline void setVelX(float vx)           { dirty = true; vel_x = vx; };
    inline void setVelY(float vy)           { dirty = true; vel_y = vy; };

    inline void setAccX(float ax)           { dirty = true; acc_x = ax; };
    inline void setAccY(float ay)           { dirty = true; acc_y = ay; };

    inline void setVelR(float vr)           { dirty = true; vel_r = vr; };
    inline void setAccR(float ar)           { dirty = true; acc_r = ar; };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    float alpha()                           { return alpha_;            }
    void  setAlpha(float a)                 { dirty = true; alpha_ = a; }

    float visible()                         { return alpha_ > 0.0f;     }
    void  setVisible(bool v)                { dirty = true; alpha_ = (v ? 1.0f : 0.0f); }

    glm::mat4& getMVPmatrix(void)           { return mvpMatrix; }

    void updateUVCoordinates(const int x, int y, int w, int h, int sw, int sh);
    void updateColor(const glm::vec4 clr );

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
    GLuint u_mvpMatrix;
    GLuint iTime;

    GLuint u_alpha;
    float alpha_;

    int textureWidth, textureHeight;

    float obj_w, obj_h;
    float scale_x, scale_y;

    float pos_x, pos_y;
    float vel_x, vel_y;
    float acc_x, acc_y;

    float vel_r, acc_r;
    float angle, angle_deg;

    float time;

    float last_secs;

    std::string name;
    std::vector<GLfloat> vertices;
    std::vector<GLuint>  indices;

    glm::mat4 mvpMatrix;

protected:
    bool dirty;
    bool dirty_uv;

}; // CLASS - piObject


typedef std::shared_ptr<piObject> piObjectPtr_t;
