#include <GLES2/gl2.h>
#pragma once

#define PI_OK    true
#define PI_FAIL  false

typedef GLint piError;

typedef GLint piTextureRef;


// Macro to clamp a value between 0.0 and 1.0
#ifndef CLAMP_01
  #define CLAMP_01(x) ((x) < 0.0 ? 0.0 : ((x) > 1.0 ? 1.0 : (x)))
#endif