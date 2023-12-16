#include <piObject.h>
#include <piUtils.h>

//======================================================================================================

piObject::piObject() :
      obj_w(0.0f),     obj_h(0.0f),
    scale_x(1.0f),   scale_y(1.0f),
      pos_x(0.0f),     pos_y(0.0f),
      vel_x(0.0f),     vel_y(0.0f),
      acc_x(0.0f),     acc_y(0.0f),
      vel_r(0.0f),     acc_r(0.0f),
      angle(0.0f), angle_deg(0.0f),
       time(0.0f), last_secs(0.0f),
     alpha_(1.0f),     dirty(true)
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
    const float left    = (x    )        / ( sw );
    const float right   = (x + w - 1)    / ( sw );
    const float top0    = (y  + h - 1  ) / ( sh );
    const float bottom0 = (y )           / ( sh );

   // printf("x: %d, y: %d, w: %d, h: %d,   sw: %d, sh: %d", x,y,  w,h,  sw,sh);

    const float top     = 1.0 - top0;
    const float bottom  = 1.0 - bottom0;

    // Only update UV coordinates, leaving vertex coordinates unchanged

    vertices[9*0 + 7 + 0] = left;   vertices[9*0 + 7 + 1] = top;    // bottom-left
    vertices[9*1 + 7 + 0] = right;  vertices[9*1 + 7 + 1] = top;    // bottom-right
    vertices[9*2 + 7 + 0] = left;   vertices[9*2 + 7 + 1] = bottom; // top-left
    vertices[9*3 + 7 + 0] = right;  vertices[9*3 + 7 + 1] = bottom; // top-right

    dirty_uv = true;

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);        // Bind VBO
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);          // Unbind VBO

    // checkGLError("piImage::updateUVCoordinates() >> piSprite ! ");
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

std::cout << std::endl << " updateColor() - AFTER > " << std::endl << vertices << std::endl;
}

//======================================================================================================

//   hasMotion() - return TRUE ... if non-zero Acceleration or Velocity
//
#define hasMotion()  (fabs(vel_x) > 0.0f || fabs(vel_y) > 0.0f || \
                      fabs(acc_x) > 0.0f || fabs(acc_y) > 0.0f || \
                      fabs(vel_r) > 0.0f || fabs(acc_r) > 0.0f )

void piObject::update( glm::mat4 &projection, float time_secs /* = 0.0 */ )
{
    time = time_secs;

// std::cout << std::endl << "DEBUG:   hasMotion() " << hasMotion() << "  time_secs: " << time_secs  << "  last_secs: " << last_secs<< std::endl;

    if( hasMotion() && last_secs > 0 && time_secs != 0.0f )
    {
        float dt = (time_secs - last_secs);

        // Compute VELOCITY
        //
        vel_x += (acc_x * dt);
        vel_y += (acc_y * dt);
        vel_r += (acc_r * dt);

        // Compute POSITION
        //
        pos_x     += (vel_x * dt);
        pos_y     += (vel_y * dt);
        angle_deg += (vel_r * dt);

        dirty = true;
    }

    last_secs = time_secs;

    if(!dirty)
    {
    //  return;
    }

    dirty = false;

    const glm::vec3 about_y_axis = glm::vec3(0.0f, 0.0f, 1.0f); //  = glm::vec3(0.0f, 1.0f, 0.0f);

// Verify that the angle is correctly calculated
// std::cout << "Angle (degrees): " << angle_deg << std::endl;
// std::cout << "Angle (radians): " << angle     << std::endl;
// std::cout << "Scale (scale_x): " << scale_x   << std::endl;
// std::cout << "Scale (scale_y): " << scale_y   << std::endl;
// std::cout << "Dims    (obj_w): " << obj_w     << std::endl;
// std::cout << "Dims    (obj_h): " << obj_h     << std::endl;

    float angleInDegrees = angle_deg;//90.0f;
    float angleInRadians = glm::radians(angleInDegrees);

    // std::cout << "Angle in Degrees: " << angleInDegrees << std::endl;
    // std::cout << "Angle in Radians: " << angleInRadians << std::endl;

    // M = T * R * S * T

    // glm::mat4 model =    glm::translate(glm::mat4(1.0f), glm::vec3(-pos_x, -pos_y,     0.0f)) *
    //                      glm::scale(    glm::mat4(1.0f), glm::vec3(scale_x * obj_w, scale_y * obj_h, 1.0f)) *
    //                      glm::rotate(   glm::mat4(1.0f), glm::radians(angle_deg), about_y_axis) *
    //                      glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y,     0.0f));


    // M = T * R * S

    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y,     0.0f)) *
    //                   glm::rotate(   glm::mat4(1.0f), glm::radians(angle_deg), about_y_axis) *
    //                   glm::scale(    glm::mat4(1.0f), glm::vec3(scale_x * obj_w, scale_y * obj_h, 1.0f));


    glm::mat4 model = //glm::translate(glm::mat4(1.0f), glm::vec3(-pos_x, -pos_y, 0.0)) *
                      glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, 0.0f)) *
                      glm::rotate(   glm::mat4(1.0f), glm::radians(-angle_deg), about_y_axis) *
                      glm::scale(    glm::mat4(1.0f), glm::vec3(scale_x * obj_w, scale_y * obj_h, 1.0f));


    // M = T * R * S * T

    // glm::mat4 t(1);
    // t = glm::translate(t, glm::vec3(-pos_x, -pos_y,0));
    // t = glm::rotate(   t, glm::radians(angle_deg), about_y_axis);
    // t = glm::scale(    t, glm::vec3(scale_x * obj_w, scale_y * obj_h, 1.0f));
    // t = glm::translate(t, glm::vec3(pos_x, pos_y,0.0f));

    // glm::mat4 model = t;

    // analyzeTransformation(model, "model"); // DEBUG
    // analyzeTransformation(projection, "projection"); // DEBUG

    mvpMatrix = projection * model;

    // analyzeTransformation(mvpMatrix, "mvpMatrix"); // DEBUG

    // printf("\nDEBUG  piObject::update() ... time_secs: %f    #### ", time_secs);
}

//======================================================================================================
//======================================================================================================
