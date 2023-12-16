
#include <GLES2/gl2.h>
#pragma once

#include <piCore.h>
#include <piValue.h>
#include <piShaderUtils.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <initializer_list>
#include <unordered_map>
#include <memory>


#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

class piValue; // fwd

piValue copyUniform(UniformType_t type, piValue &val);

//=====================================================================================================================================

class piShader: public shaderProgram
{
public:
  piShader();
  piShader(const char* vShaderTxt, const char* fShaderTxt);

  virtual ~piShader();

//   virtual unsigned long Release();

  bool isRealTime() { return mIsRealTime; }
  bool needsFbo()   { return mNeedsFbo;   }
  int32_t passes()  { return mPasses;     }

  virtual void setupResource();
  virtual void init();
  virtual void onInit();

  piError addUniform(const std::string &name, const std::string &type);

  void addUniforms(std::initializer_list<std::pair<std::string, std::string>> uniforms)
  {
    for (const auto& uniform : uniforms)
    {
        std::cout << "Name: " << uniform.first << ", Type: " << uniform.second << std::endl;

        addUniform(uniform.first, uniform.second);
    }
  }


//   virtual piError Set(const char* name, const piValue* value);

  UniformType_t getUniformType(std::string &name);

  piError setUniformVal(const std::string& name, const piValue& v);

  // Loading
  //
  //   virtual void     loadResource(rtObjectRef archive = NULL, bool reloading=false);
  //   virtual void     loadResourceFromFile();
  //   virtual void     loadResourceFromArchive(rtObjectRef /*archiveRef*/);

protected:
  //
  // GL Setters
  //
  //   static piError fillInt32Vec(int32_t *vec, const rtArrayObject* vals); // generic code
  //   static piError fillFloatVec(float   *vec, const rtArrayObject* vals); // generic code

  // Samplers
  static piError bindTexture3(const uniformLoc_t &p)   { return bindTextureN(3, p); };
  static piError bindTexture4(const uniformLoc_t &p)   { return bindTextureN(4, p); };
  static piError bindTexture5(const uniformLoc_t &p)   { return bindTextureN(5, p); };

  static piError bindTextureN(GLenum n, const uniformLoc_t &p); // generic setter

  // Scalars
  static piError setUniform1i(const uniformLoc_t &p)   { glUniform1i(p.loc, p.value.toInt32() ); return PI_OK; }
  static piError setUniform1f(const uniformLoc_t &p)   { glUniform1f(p.loc, p.value.toFloat() ); return PI_OK; }

  // INT vectors
  static piError setUniform2iv(const uniformLoc_t &p)  { return setUniformNiv(2, p); }
  static piError setUniform3iv(const uniformLoc_t &p)  { return setUniformNiv(3, p); }
  static piError setUniform4iv(const uniformLoc_t &p)  { return setUniformNiv(4, p); }

  static piError setUniformNiv(int N, const uniformLoc_t &p); // generic setter

  // FLOAT vectors
  static piError setUniform2fv(const uniformLoc_t &p)  { return setUniformNfv(2, p); }
  static piError setUniform3fv(const uniformLoc_t &p)  { return setUniformNfv(3, p); }
  static piError setUniform4fv(const uniformLoc_t &p)  { return setUniformNfv(4, p); }

  static piError setUniformNfv(int N, const uniformLoc_t &p); // generic setter

  // Override to do uniform lookups
  virtual void prelink();
  virtual void postlink();

private:
  bool        mInitialized;
  bool        mIsRealTime;
  bool        mNeedsFbo;

  // Property ivars
  //
  int32_t     mPasses;
  int32_t     mSamplerCount;

}; // CLASS - piShader

typedef std::shared_ptr<piShader> piShaderPtr_t;

//=====================================================================================================================================

typedef std::map<uint32_t, piShader*> ShaderMap_t;
typedef std::shared_ptr<piShader> piShaderPtr_t;

class piShaderManager
{
public:

    void addShader(const std::string& name, piShader* program);
    void removeShader(const std::string& name);

    std::shared_ptr<piShader> getProgram(const std::string& name);

    static void removeShader(uint32_t shaderId);
//   static void clearAllShaders();

protected:

  std::unordered_map<std::string, piShaderPtr_t> shaders;

  static ShaderMap_t   mShaderMap;
  static bool init;
};

//=====================================================================================================================================
