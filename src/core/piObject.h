#include <GLES2/gl2.h>
#pragma once

#include <piCore.h>
// #include <piAnimation.h>

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <iostream>

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

class piAnimator;

class piObject : public std::enable_shared_from_this<piObject>
{

     // Use piObjectPtr_t
    typedef std::shared_ptr<piAnimator> piAnimatorPtr_t;

public:

     piObject();
    ~piObject();

    bool operator==(const piObject& other) const {

        if(obj_size != other.obj_size) return false; // OBJ SIZE
        if(tex_size != other.tex_size) return false; // TEX SIZE
        if(pos      != other.pos)      return false; // POSITION
        if(scale    != other.scale)    return false; // SCALE
        if(vel      != other.vel)      return false; // VELOCITY
        if(acc      != other.acc)      return false; // ACCELERATION

        // Compare all relevant members for equality
        return u_mvpMatrix == other.u_mvpMatrix &&
               iTime == other.iTime &&
               u_alpha == other.u_alpha &&
               alpha_  == other.alpha_ &&

            //    obj_size.x == other.obj_size.x &&
            //    obj_size.y == other.obj_size.y &&
            //    scale.x == other.scale.x &&
            //    scale.y == other.scale.y &&
            //    pos.x == other.pos.x &&
            //    pos.y == other.pos.y &&
            //    vel.x == other.vel.x &&
            //    vel.y == other.vel.y &&
            //    acc.x == other.acc.x &&
            //    acc.y == other.acc.y &&
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

    // SIZE
    inline void setSize(float sx, float sy)    { dirty = true; obj_size = glm::vec2(sx, sy); };
    inline void setSize(const glm::vec2& size) { dirty = true; obj_size = size; }
    inline void setSizeW(float sw)             { dirty = true; obj_size.x = sw; };
    inline void setSizeH(float sh)             { dirty = true; obj_size.y = sh; };

    inline glm::vec2 getSize()  const { return obj_size;   };
    inline float     getSizeW() const { return obj_size.x; };
    inline float     getSizeH() const { return obj_size.y; };

    // POSITION
    inline void setPos(float px, float py)  { dirty = true; pos   = glm::vec2(px, py); };
    inline void setPos(const glm::vec2& pp) { dirty = true; pos   = pp; };
    inline void setPosX(float px)           { dirty = true; pos.x = px; };
    inline void setPosY(float py)           { dirty = true; pos.y = py; };

    inline glm::vec2 getPos()  const { return pos;   };
    inline float     getPosX() const { return pos.x; };
    inline float     getPosY() const { return pos.y; };

    // ANCHOR
    inline void setAnchor(float ax, float ay)  { dirty = true; anchor   = glm::clamp(glm::vec2(ax, ay), 0.0f, 1.0f); };
    inline void setAnchor(const glm::vec2& aa) { dirty = true; anchor   = glm::clamp(aa, 0.0f, 1.0f); };
    inline void setAnchorX(float ax)           { dirty = true; anchor.x = glm::clamp(ax, 0.0f, 1.0f); };
    inline void setAnchorY(float ay)           { dirty = true; anchor.y = glm::clamp(ay, 0.0f, 1.0f); };

    inline glm::vec2 getAnchor()  const { return anchor;   };
    inline float     getAnchorX() const { return anchor.x; };
    inline float     getAnchorY() const { return anchor.y; };

    // SCALE
    inline void setScale(float sx, float sy)  { dirty = true; scale   = glm::clamp(glm::vec2(sx, sy), 0.0f, 1.0f); };
    inline void setScale(const glm::vec2& ss) { dirty = true; scale   = glm::clamp(ss, 0.0f, 1.0f); };
    inline void setScaleX(float sx)           { dirty = true; scale.x = glm::clamp(sx, 0.0f, 1.0f); };
    inline void setScaleY(float sy)           { dirty = true; scale.y = glm::clamp(sy, 0.0f, 1.0f); };

    inline glm::vec2 getScale()  const { return scale;   };
    inline float     getScaleX() const { return scale.x; };
    inline float     getScaleY() const { return scale.y; };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    // PHYSICS
    //

    // VELOCITY
    inline void setVel(const glm::vec2& vv) { dirty = true; vel   = vv; };
    inline void setVelX(float vx)           { dirty = true; vel.x = vx; };
    inline void setVelY(float vy)           { dirty = true; vel.y = vy; };

    inline glm::vec2 getVel() const { return vel; };

    // ACCLERATION
    inline void setAcc(const glm::vec2& aa) { dirty = true; acc = aa; };
    inline void setAccX(float ax)           { dirty = true; acc.x = ax; };
    inline void setAccY(float ay)           { dirty = true; acc.y = ay; };

    inline glm::vec2 getAcc() const { return acc; };

    // ANGULAR
    inline void setVelR(float vr)           { dirty = true; vel_r = vr; };
    inline void setAccR(float ar)           { dirty = true; acc_r = ar; };

    inline void setAngleDegrees(float deg)  { dirty = true; angle_deg = deg; };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void setName(std::string nn)            { name = nn;                }
    std::string getName()                   { return name;              }

    float alpha()                           { return alpha_;            }
    void  setAlpha(float a)                 { dirty = true; alpha_ = CLAMP_01(a); }

    float visible()                         { return alpha_ > 0.0f;     }
    void  setVisible(bool v)                { dirty = true; alpha_ = (v ? 1.0f : 0.0f); }

    glm::mat4& getMVPmatrix(void)           { return mvpMatrix; }

    void updateUVCoordinates(const int x, int y, int w, int h, int sw, int sh);
    void updateColor(const glm::vec4 clr );

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    std::list<piAnimatorPtr_t> animators;

    piAnimatorPtr_t animateProperty(glm::vec2 &prop, glm::vec2 end_val, float duration = 50);

    void addAnimator(piAnimatorPtr_t animator);
    void updateAll(float deltaTime);

public:
    GLuint u_mvpMatrix;
    GLuint iTime;

    GLuint u_alpha;
    float alpha_;

    glm::ivec2 tex_size;
    glm::vec2  obj_size;
    glm::vec2  scale;

    glm::vec2 pos;    // position
    glm::vec2 vel;    // velocity
    glm::vec2 acc;    // acceleration
    glm::vec2 anchor; // anchor

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
    bool dirty_color;

}; // CLASS - piObject

//class piObject; //fwd

typedef std::shared_ptr<piObject> piObjectPtr_t;

//======================================================================================================
//======================================================================================================
