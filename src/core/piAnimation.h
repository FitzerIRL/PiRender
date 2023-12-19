#include <GLES2/gl2.h>
#pragma once

#include <list>
#include <memory>

#include <functional>
#include <iostream>

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include <piCore.h>
// #include <piObject.h>

class piEasingFunctions; //fwd
class piAnimate;         //fwd
class piAnimator;        //fwd

class piObject;


//======================================================================================================
//======================================================================================================

class piEasingFunctions
{
public:
    using EasingFunction = std::function<float(float)>;

    piEasingFunctions();

    static float Linear(float t)          { return t;           }

    static float QuadraticIn(float t)     { return t * t;       }
    static float QuadraticOut(float t)    { return t * (2 - t); }
    static float QuadraticInOut(float t)  { return (t < 0.5) ? 2 * t * t : -1 + (4 - 2 * t) * t; }

    static float CubicIn(float t)         { return t * t * t;   }
    static float CubicOut(float t)        { float f = t - 1; return f * f * f + 1; }
    static float CubicInOut(float t)      { return (t < 0.5) ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; }

    static float EaseOutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1)
        {
            return (n1 * t * t);
        }
        else if (t < 2.0f / d1)
        {
            t -= 1.5f / d1;
            return (n1 * t * t + 0.75f);
        }
        else if (t < 2.5f / d1)
        {
            t -= 2.25f / d1;
            return (n1 * t * t + 0.9375f);
        }
        else
        {
            t -= 2.625f / d1;
            return (n1 * t * t + 0.984375f);
        }
    }

    static float EaseInBounce(float t)
    {
        return 1.0f - EaseOutBounce(1.0f - t);
    }
};

//======================================================================================================

class piAnimator; // fwd
typedef std::shared_ptr<piAnimator> piAnimatorPtr_t;

class piAnimator
{
    // Use piObjectPtr_t
    typedef std::shared_ptr<piObject> piObjectPtr_t;

    using EasingFunction    = std::function<float(float)>;
    using AnimationCallback = std::function<void(piObjectPtr_t o)>;

public:
    static piAnimatorPtr_t create(  piObjectPtr_t obj,
                                    glm::vec2 &value,
                                    glm::vec2 end_val,
                                    float duration = 50,
                                    const AnimationCallback &callback = nullptr)
    {
        return std::make_shared<piAnimator>(obj, value, end_val, duration);
    }

    bool isComplete()   { return bComplete; }

    void setDuration(float dur)           { duration   = dur;   }
    void setStartValue(glm::vec2 start)   { startValue = start; }
    void setEndValue(glm::vec2 end)       {   endValue = end;   }

    void setCallback(const AnimationCallback &cb) { callback = cb; }

    void setEasingFunction(const EasingFunction &easing) { easingFunction = easing; }

    // void startAnimation()
    // {
    //     startTime = 0.0f;
    // }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    piAnimator(piObjectPtr_t o, glm::vec2& value, glm::vec2 end_val, float duration,
            const AnimationCallback &callback = nullptr) :

        animatedValue(value), obj(o), current_sec(0.0f), duration(duration), bComplete(false)
    {
        startValue = value;
        endValue   = value + end_val;

        glm::vec2 delta = (end_val - value );

        endValue   = (value + delta);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void update(float delta_secs)
    {
        // delta_secs = 0.1; // JUNK DEBUG

        current_sec += delta_secs;

        if(current_sec > duration)
        {
            if(callback && bComplete == false) // call once
            {
                callback(obj);
            }

            bComplete = true;
            return;
        }

        float normalizedTime = glm::clamp(current_sec / duration, 0.0f, 1.0f);

        // Compute with EASING ...
        //
        animatedValue = startValue + (endValue - startValue) * easingFunction(normalizedTime);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    glm::vec2& animatedValue;

    glm::vec2 startValue;
    glm::vec2 endValue;

    float current_sec;
    // float startTime;
    float duration;

    bool bComplete;
    bool bPaused;

    piObjectPtr_t obj;

    AnimationCallback callback;
    EasingFunction easingFunction = piEasingFunctions::EaseOutBounce; //Linear;
};

//======================================================================================================
