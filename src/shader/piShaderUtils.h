#pragma once

#include <map>
#include <string>
#include <vector>

#include <piCore.h>
#include <piValue.h>

class piValue;       //fwd
class shaderProgram; //fwd

//=====================================================================================================================================

struct uniformLoc; //fwd

typedef  piError (*setFunc_t)(const uniformLoc &);

typedef enum UniformType
{
  UniformType_Unknown,
  UniformType_Bool,
  UniformType_Int,
  UniformType_UInt,
  UniformType_Float,
  UniformType_Double,

  // TODO: Possibly not in GL ES 2.0
  //
  //  // Vectors:  bool
  //  UniformType_bVec2,
  //  UniformType_bVec3,
  //  UniformType_bVec4,

  // Vectors:  int
  UniformType_iVec2,
  UniformType_iVec3,
  UniformType_iVec4,

  // TODO: Possibly not in GL ES 2.0
  //
  //  // Vectors:  uint
  //  UniformType_uVec2,
  //  UniformType_uVec3,
  //  UniformType_uVec4,

  // Vectors:  float
  UniformType_Vec2,
  UniformType_Vec3,
  UniformType_Vec4,

  // TODO: Possibly not in GL ES 2.0
  //
  //  // Vectors:  double
  //  UniformType_dVec2,
  //  UniformType_dVec3,
  //  UniformType_dVec4,

  UniformType_Sampler2D,
  UniformType_Struct

} UniformType_t;


//=====================================================================================================================================

struct uniformLoc
{
  std::string    name;         // uniform NAME
  UniformType_t  type;         // GLSL type indicated by enum
  GLint          loc;          // uniform location in Shader

  piValue        value;        // piValue to be applied per draw.

  bool           needsUpdate;  // flag to set in shader only whence changed.

  setFunc_t      setFunc;      // static pointer to *setter* function to unwrap and apply piValue into Shader
};

//=====================================================================================================================================

typedef uniformLoc uniformLoc_t;

typedef std::map<std::string, uniformLoc_t>            UniformMap_t;
typedef std::map<std::string, uniformLoc_t>::iterator  UniformMapIter_t;

typedef std::vector<uniformLoc_t>                      UniformList_t;
typedef std::vector<uniformLoc_t>::iterator            UniformListIter_t;

typedef struct glShaderProgDetails
{
  GLuint program;
  GLuint fragShader;
  GLuint vertShader;

  bool     didError;
  std::string compilation;

} glShaderProgDetails_t;

//=====================================================================================================================================

glShaderProgDetails_t  createShaderProgram(const char* vShaderTxt, const char* fShaderTxt); //fwd
void linkShaderProgram(GLuint program); //fwd

//=====================================================================================================================================

class shaderProgram
{
public:

  shaderProgram();

  virtual ~shaderProgram();

  piError initShader(const char* v, const char* f);
  int getUniformLocation(const char* name);

  void use();

  void saveUniforms();        // save uniforms
  void restoreUniforms();     // restore uniforms

  piError draw( int resW, int resH, float* matrix, float alpha,
                piTextureRef t,
                GLenum mode,
                const void* pos,
                const void* uv,
                int count);

  GLuint program()  { return mProgram; }

protected:

  // Override to do uniform lookups
  virtual void prelink()  {}
  virtual void postlink() {}

  void copyUniforms(UniformMap_t &from,
                    UniformMap_t &to,
                    bool needsUpdate = false);

  GLuint mProgram, mFragShader,    mVertShader;
  GLuint mTimeLoc, mResolutionLoc, mMatrixLoc;
  GLuint mPosLoc,  mUVLoc,         mAlphaLoc,   mColorLoc;

  std::string mCompilation;

  UniformMap_t  mUniform_map;  // live
  UniformMap_t  mUniform_map2; // save/restore

}; // CLASS

//=====================================================================================================================================
