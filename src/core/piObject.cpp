#include <piObject.h>
#include <piUtils.h>
#include <piAnimation.h>

//======================================================================================================

piObject::piObject() :
 obj_size(1.0f, 1.0f),
    scale(1.0f, 1.0f),
      pos(0.0f, 0.0f),
      vel(0.0f, 0.0f),
      acc(0.0f, 0.0f),
   anchor(0.5f, 0.5f),
      vel_r(0.0f),     acc_r(0.0f),
      angle(0.0f), angle_deg(0.0f),
       time(0.0f), last_secs(0.0f),
     alpha_(1.0f),     dirty(true),
     dirty_uv(true), dirty_color(true)
{
    // ctor
}

//======================================================================================================

piObject::~piObject() {}

//======================================================================================================

void piObject::reset()
{
                 //   vec3                        vec4                    vec2            9 * float
    vertices = {
                 // Vertex                       // Color              // Texture
                -0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f, 0.0f,       0.0f, 0.0f,   // bottom-left
                 0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f, 0.0f,       1.0f, 0.0f,   // bottom-right
                -0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f, 0.0f,       0.0f, 1.0f,   // top-left
                 0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f, 0.0f,       1.0f, 1.0f    // top-right
               };

    indices = {0, 1, 2, 1, 3, 2};
}

//======================================================================================================

void piObject::updateUVCoordinates(int x, int y, int w, int h, int sw, int sh)
{
    const float left    = (float) (x )        / ( (float) sw );
    const float right   = (float) (x + w - 1) / ( (float) sw );
    const float top0    = (float) (y + h - 1) / ( (float) sh );
    const float bottom0 = (float) (y )        / ( (float) sh );

    // printf("\n DEBUG: updateUVCoordinates() ... x: %d, y: %d, w: %d, h: %d,   sw: %d, sh: %d", x,y,  w,h,  sw,sh);
    // printf("\n DEBUG: updateUVCoordinates() ... L: %f, R: %f, T: %f, B: %f", left, right, top0, bottom0);

    const float top     = 1.0 - top0;
    const float bottom  = 1.0 - bottom0;
piSpritePtr_t
    vertices[9*0 + 7 + 0] = left;   vertices[9*0 + 7 + 1] = top;    // bottom-left
    vertices[9*1 + 7 + 0] = right;  vertices[9*1 + 7 + 1] = top;    // bottom-right
    vertices[9*2 + 7 + 0] = left;   vertices[9*2 + 7 + 1] = bottom; // top-left
    vertices[9*3 + 7 + 0] = right;  vertices[9*3 + 7 + 1] = bottom; // top-right

    dirty_uv = true;
}

//======================================================================================================

void piObject::updateColor(const glm::vec4 clr )
{
// std::cout << std::endl << " updateColor() - ENTER clr: " << clr << std::endl;

    #define vv  vertices

// std::cout << std::endl << " updateColor() - BEFORE > " << std::endl << vertices << std::endl;

    vv[9*0 + 3 + 0] = clr.r;  vv[9*0 + 3 + 1] = clr.g;  vv[9*0 + 3 + 2] = clr.b;  vv[9*0 + 3 + 3] = clr.a;
    vv[9*1 + 3 + 0] = clr.r;  vv[9*1 + 3 + 1] = clr.g;  vv[9*1 + 3 + 2] = clr.b;  vv[9*1 + 3 + 3] = clr.a;
    vv[9*2 + 3 + 0] = clr.r;  vv[9*2 + 3 + 1] = clr.g;  vv[9*2 + 3 + 2] = clr.b;  vv[9*2 + 3 + 3] = clr.a;
    vv[9*3 + 3 + 0] = clr.r;  vv[9*3 + 3 + 1] = clr.g;  vv[9*3 + 3 + 2] = clr.b;  vv[9*3 + 3 + 3] = clr.a;

    dirty_color = true;

std::cout << std::endl << " updateColor() - AFTER > " << std::endl << vertices << std::endl;
}

//======================================================================================================

//   hasMotion() - return TRUE ... if non-zero Acceleration or Velocity
//
#define hasMotion()  (fabs(vel.x) > 0.0f || fabs(vel.y) > 0.0f || \
                      fabs(acc.x) > 0.0f || fabs(acc.y) > 0.0f || \
                      fabs(vel_r) > 0.0f || fabs(acc_r) > 0.0f )

void piObject::update( glm::mat4 &projection, float time_secs /* = 0.0 */ )
{
    float dt = (time_secs - last_secs);

    time = time_secs;

// std::cout << std::endl << "DEBUG:   hasMotion() " << hasMotion() << "  time_secs: " << time_secs  << "  last_secs: " << last_secs<< std::endl;

#define USE_PHYSICS
#ifdef USE_PHYSICS

    if( hasMotion() && last_secs > 0 && time_secs != 0.0f )
    {
        // Compute VELOCITY & POSITION
        //
        vel += (acc * dt);
        pos += (vel * dt);

        // Compute ROTATION VELOCITY & ANGLE
        //
        vel_r     += (acc_r * dt);
        angle_deg += (vel_r * dt);

        dirty = true;
    }
#endif // USE_PHYSICS

    last_secs = time_secs;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define USE_ANIMATORS
#ifdef USE_ANIMATORS

    if(animators.size())
    {
        for (auto& animator : animators)
        {
            animator->update(dt);

            if(animator->isComplete() == false)
            {
                dirty = true;
            }
        }

        // Find completed animators
        animators.erase(
            std::remove_if(
                animators.begin(),
                animators.end(),
                [](const auto& animator) { return animator->isComplete(); }
            ),
            animators.end()
        );
    }//ENDIF

#endif // USE_ANIMATORS

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if(!dirty)
    {
      return;
    }

    dirty = false;

    const glm::vec3 about_y_axis = glm::vec3(0.0f, 0.0f, 1.0f);

    float angleInDegrees = angle_deg;
    float angleInRadians = glm::radians(angleInDegrees);

    // std::cout << std::endl << "############### Angle in Degrees: " << angleInDegrees << std::endl;
    // std::cout << "Angle in Radians: " << angleInRadians << std::endl;

    // M = T * R * S * T

    // Anchor offset
    float mpx = (anchor.x - 0.5 );
    float mpy = (anchor.y - 0.5 );

    float dx = -(mpx * obj_size.x);
    float dy = -(mpy * obj_size.y);

    glm::mat4 model = glm::mat4(1.0f);
    // glm::mat4 II = glm::mat4(1.0f);

    // std::cout << "############### mpx: " << mpx << " mpy: " << mpy << std::endl;
    // std::cout << "############### dx: " <<   dx << " dy: "  <<  dy << std::endl;

    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x + dx, pos.y + dy, 0.0f)) *
    //                   glm::rotate(   glm::mat4(1.0f), glm::radians(-angle_deg), about_y_axis)  *
    //                   glm::scale(    glm::mat4(1.0f), glm::vec3(scale.x * obj_size.x,
    //                                                             scale.y * obj_size.y, 0.0f));

     // analyzeTransformation(model, "model", false); // DEBUG
    // analyzeTransformation(projection, "projection", false); // DEBUG

    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, -angle_deg);

    model = glm::translate(model, glm::vec3(pos.x + dx, pos.y + dy, 0.0f));

    model = glm::rotate(   model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //
    model = glm::rotate(   model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //
    model = glm::rotate(   model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //

    model = glm::scale(    model, glm::vec3(scale.x * obj_size.x, scale.y * obj_size.y, 1.0f));

    // std::cout << "############### pos.x: " << pos.x << " pos.y: " << pos.y << std::endl;
    // std::cout << "############### dx: " << dx << " dy: " << dy << std::endl;
    // std::cout << "############### scale.x: " << scale.x << " scale.y: " << scale.y << std::endl;
    // std::cout << "############### obj_size.x: " << obj_size.x << " obj_size.y: " << obj_size.y << std::endl;

    // analyzeTransformation(II,    "II",    false); // DEBUG
    // analyzeTransformation(model, "model", false); // DEBUG

    mvpMatrix = projection * model;

    // analyzeTransformation(mvpMatrix, "mvpMatrix", false); // DEBUG

    // analyzeTransformation(this->getMVPmatrix(), "getMVPmatrix", false); // DEBUG

    // printf("\nDEBUG  piObject::update() ... time_secs: %f    #### ", time_secs);
}

//======================================================================================================
//======================================================================================================


piAnimatorPtr_t piObject::animateProperty(glm::vec2 &prop, glm::vec2 end_val, float duration /* = 50 */)
{
    piObjectPtr_t obj =  shared_from_this();

    piAnimatorPtr_t anim = piAnimator::create( obj, prop, end_val, duration );

    addAnimator( anim );

    return anim;
}

void piObject::addAnimator(piAnimatorPtr_t animator)
{
    animators.push_back(animator);
}

void piObject::updateAll(float deltaTime)
{
    for (const auto& animator : animators)
    {
        animator->update(deltaTime);
    }
}