
#include <piShaderUtils.h>
#include <piShaderManager.h>

using namespace std;

extern const char* vShaderText;
GLuint currentGLProgram;

//=====================================================================================================================================

piShader::piShader()
{

}

piShader::piShader(const char* vShaderPath, const char* fShaderPath)
{
    fprintf(stderr,"INFO: piShader() - ENTER \n");

    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM

    // Load VERTEX shader source code from files
    FILE *fhVertex = fopen(vShaderPath, "r");
    if (!fhVertex) {
        fprintf(stderr, "Failed to open vertex shader file: %s\n", vShaderPath);
        return;
    }

    fseek(fhVertex, 0, SEEK_END);
    long fileSizeV = ftell(fhVertex);
    fseek(fhVertex, 0, SEEK_SET);

    char *sourceV = (char *)malloc(fileSizeV + 1);
    fread(sourceV, 1, fileSizeV, fhVertex);
    fclose(fhVertex);

    sourceV[fileSizeV] = '\0';


    // Load FRAGMENT shader source code from files
    FILE *fhFragment = fopen(fShaderPath, "r");
    if (!fhFragment) {
        fprintf(stderr, "Failed to open vertex shader file: %s\n", vShaderPath);
        return;
    }

    fseek(fhFragment, 0, SEEK_END);
    long fileSizeF = ftell(fhFragment);
    fseek(fhFragment, 0, SEEK_SET);

    char *sourceF = (char *)malloc(fileSizeF + 1);
    fread(sourceF, 1, fileSizeF, fhFragment);
    fclose(fhFragment);

    sourceF[fileSizeF] = '\0';


    if(shaderProgram::initShader( sourceV, sourceF ) != PI_OK)
    {
        fprintf(stderr,"FATAL: Shader error: %s \n", mCompilation.c_str() );
    }

    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM
}


piShader::~piShader()
{
  fprintf(stderr,"~piShader()");
}

void piShader::onInit()
{
  if(mProgram != -1)
  {
    postlink();
  }

  if( mInitialized)
    return;

  mInitialized = true;
}

void piShader::init()
{
  if( mInitialized)
    return;

  mInitialized = true;
}

void piShader::setupResource()
{
  piShader::init();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // CREATE SHADER PROGRAMS
/*
  if(mFragmentSrc.length() > 0)
  {
    // Setup shader
    double startDecodeTime = pxMilliseconds();

    const char* vtxCode = mVertexSrc.length() > 0 ? (const char*) mVertexSrc.data() : vShaderText; // or use "default" Vertex shader

    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM

    if(shaderProgram::initShader( vtxCode, (const char*) mFragmentSrc.data() ) != PI_OK)
    {
        fprintf(stderr,"FATAL: Shader error: %s \n", mCompilation.cString() );

        setLoadStatus("glError", mCompilation.cString() );
        setLoadStatus("statusCode",PX_RESOURCE_STATUS_DECODE_FAILURE);

        if (gUIThreadQueue)
        {
          AddRef(); // async
          gUIThreadQueue->addTask(onDownloadCanceledUI, this, (void*)"reject");
        }

        mReady.send("reject", this );
    }

    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM
    // COMPILE SHADER PROGRAM

    double stopDecodeTime = pxMilliseconds();
    setLoadStatus("decodeTimeMs", static_cast<int>(stopDecodeTime-startDecodeTime));
  }
  */
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

UniformType_t piShader::getUniformType(std::string &name)
{
  UniformMapIter_t it = mUniform_map.find(name.c_str());
  if(it != mUniform_map.end())
  {
    uniformLoc_t &p = (*it).second;

    return p.type;
  }

  return UniformType_Unknown;
}

//
//  Set() >> setUniformVal() ... called when setting Uniforms directly on a [shaderProgram].
//

piError piShader::addUniform(const std::string &name, const std::string &type)
{
  setFunc_t     setFunc = NULL;
  UniformType_t typeEnum = UniformType_Unknown;

  if(name.empty() || type.empty())
  {
    fprintf(stderr,"Bad args ... empty !");
    return PI_FAIL;
  }

  if(name == "u_time")
  {
    mIsRealTime = true;
  }
  else
  if(type == "int")
  {
    setFunc  = piShader::setUniform1i;
    typeEnum = UniformType_Int;
  }
  else
  if(type == "float")
  {
    setFunc  = piShader::setUniform1f;
    typeEnum = UniformType_Float;
  }
  else
  if(type == "ivec2")
  {
    setFunc  = piShader::setUniform2iv;
    typeEnum = UniformType_iVec2;
  }
  else
  if(type == "ivec3")
  {
    setFunc  = piShader::setUniform3iv;
    typeEnum = UniformType_iVec3;
  }
  else
  if(type == "ivec4")
  {
    setFunc  = piShader::setUniform4iv;
    typeEnum = UniformType_iVec4;
  }
  else
  if(type == "vec2")
  {
    setFunc  = piShader::setUniform2fv;
    typeEnum = UniformType_Vec2;
  }
  else
  if(type == "vec3")
  {
    setFunc  = piShader::setUniform3fv;
    typeEnum = UniformType_Vec3;
  }
  else
  if(type == "vec4")
  {
    setFunc  = piShader::setUniform4fv;
    typeEnum = UniformType_Vec4;
  }
  else
  if(type == "sampler2D")
  {
    if(name == "s_texture")
    {
      mNeedsFbo = true;
    }
    setFunc = (mSamplerCount == 3) ? piShader::bindTexture3 :
              (mSamplerCount == 4) ? piShader::bindTexture4 :
              (mSamplerCount == 5) ? piShader::bindTexture5 : NULL;

    typeEnum = UniformType_Sampler2D;

    mSamplerCount++;
  }
  else
  {
      return PI_FAIL; // unknown type
  }

  mUniform_map[name] = { name, typeEnum, (GLint) -1, piValue(), false, setFunc };  // ADD TO MAP

  return PI_OK;
}

void piShader::prelink()
{
  fprintf(stderr,"INFO: prelink() - ENTER \n");

  mPosLoc = 0;
  mUVLoc = 1;
//   glBindAttribLocation(mProgram, mPosLoc, "vPosition")
  glBindAttribLocation(mProgram, mPosLoc, "pos") ;
  glBindAttribLocation(mProgram, mUVLoc,  "uv");
}

void piShader::postlink()
{
  fprintf(stderr,"INFO: postlink() - ENTER \n");

//   mTimeLoc       = getUniformLocation("iTime");//u_time");
//   mResolutionLoc = getUniformLocation("iResolution");//u_resolution");
//   mMatrixLoc     = getUniformLocation("amymatrix");

  mColorLoc      = getUniformLocation("u_color");
  mAlphaLoc      = getUniformLocation("u_alpha");
  mTimeLoc       = getUniformLocation("u_time");
  mResolutionLoc = getUniformLocation("u_resolution");
  mMatrixLoc     = getUniformLocation("amymatrix");

  addUniform("a_color","vec4");
  addUniform("u_alpha", "float");


  if(mUniform_map.size() > 0)
  {
    for (UniformMapIter_t it  = mUniform_map.begin();
                          it != mUniform_map.end(); ++it)
    {
       fprintf(stderr,"INFO: postlink() - locating >> %s \n",  (*it).second.name.c_str());

      (*it).second.loc = getUniformLocation( (*it).second.name.c_str() );
    }
  }//ENDIF
}

//=====================================================================================================================================
//
// GL Setters
//
#if 0
/*static*/ piError piShaderManager::fillInt32Vec(int32_t *vec, const rtArrayObject* vals)
{
  if(vals && vec)
  {
    piValue count;                  // HACK - WORKAROUND
    vals->Get("length", &count);    // HACK - WORKAROUND

    int len = count.toUInt32();
    int  ii = 0;

    for (uint32_t i = 0, l = len; i < l; ++i)
    {
      piValue vv;
      if(vals->Get(i, &vv) == PI_OK && !vv.isEmpty())
      {
        vec[ii++] = vv.toInt32();
      }
    }//FOR

    return PI_OK;
  }

  return PI_FAIL;
}

/*static*/ piError piShaderManager::fillFloatVec(float *vec, const rtArrayObject* vals)
{
  if(vals && vec)
  {
    piValue count;                  // HACK - WORKAROUND
    vals->Get("length", &count);    // HACK - WORKAROUND

    int len = count.toUInt32();
    int ii  = 0;

    for (uint32_t i = 0, l = len; i < l; ++i)
    {
      piValue vv;
      if(vals->Get(i, &vv) == PI_OK && !vv.isEmpty())
      {
        vec[ii++] = vv.toFloat();
      }
    }//FOR

    return PI_OK;
  }

  return PI_FAIL;
}
#endif

piError piShader::setUniformVal(const std::string& name, const piValue& v)
{
  UniformMapIter_t it = mUniform_map.find(name);
  if(it != mUniform_map.end())
  {
    use();

    uniformLoc_t &p = (*it).second;

    p.value = v;        // update the VALUE
    p.needsUpdate = true;

    return PI_OK;
  }

  fprintf(stderr, "\n ERROR: setUniformVal() >>  %s - NOT FOUND", name.c_str() );
  return PI_FAIL; // not found
}

// sampler
/*static*/ piError piShader::bindTextureN(GLuint n, const uniformLoc_t &p)
{
/*
  pxImage         *img = dynamic_cast<         pxImage* >(p.value.toObject().getPtr());
  rtImageResource *res = dynamic_cast< rtImageResource* >(p.value.toObject().getPtr());

  pxTextureRef tex;

  if(res)
    tex = res->getTexture(); // passed an imageResource
  else
  if(img)
    tex = img->getTexture(); // passed an image

  if(tex)
  {
    GLuint tid = tex->getNativeId();

    GLenum texN;

    switch(n)
    {
      case 3: texN = GL_TEXTURE3; break;
      case 4: texN = GL_TEXTURE4; break;
      case 5: texN = GL_TEXTURE5; break;
      default:
        fprintf(stderr,"Invalid texture ID: %d  (Not 3,4 or 5) ", n);
        return PI_FAIL;
    }

    glActiveTexture( texN );

    glBindTexture(GL_TEXTURE_2D,     tid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glUniform1i(p.loc, n);


    return PI_OK;
  }
*/
  return PI_FAIL;
}

/*static*/ piError piShader::setUniformNiv(int N, const uniformLoc_t &p)
{
//   const rtArrayObject* vals = (const rtArrayObject*)  p.value.toObject().getPtr();

//   if(vals)
  {
    static int32_t vec[4] = {0}; // up to vec4

    // fillInt32Vec(&vec[0], vals);

    switch(N)
    {
      case 2: glUniform2iv(p.loc, 1, vec ); break;
      case 3: glUniform3iv(p.loc, 1, vec ); break;
      case 4: glUniform4iv(p.loc, 1, vec ); break;
    }

    return PI_OK;
  }

  return PI_FAIL;
}

/*static*/ piError piShader::setUniformNfv(int N, const uniformLoc_t &p)
{
//   const rtArrayObject* vals = (const rtArrayObject*)  p.value.toObject().getPtr();

//   if(vals)
  {
    static float vec[4] = {0.0}; // up to vec4

    // fillFloatVec(&vec[0], vals);

    switch(N)
    {
      case 2: glUniform2fv(p.loc, 1, vec ); break;
      case 3: glUniform3fv(p.loc, 1, vec ); break;
      case 4: glUniform4fv(p.loc, 1, vec ); break;
    }

    return PI_OK;
  }

  return PI_FAIL;
}


static int gShaderId = 0;

/**********************************************************************/
/**                    piShaderManager                                */
/**********************************************************************/
ShaderMap_t    piShaderManager::mShaderMap;     //  ID to SHADER

void piShaderManager::removeShader(uint32_t shaderId)
{
  ShaderMap_t::iterator it = mShaderMap.find(shaderId);
  if (it != mShaderMap.end())
  {
    mShaderMap.erase(it);
  }
  else
  {
    fprintf(stderr,"removeShader(id: %d) - NOT found ! \n",shaderId);
  }
}

//=====================================================================================================================================
//
// // helper to copy "variant" like UNIFORM types
//
piValue copyUniform(UniformType_t type, piValue &val)
{
  piValue ans;

  switch(type)
  {
      case UniformType_Unknown:
      ans.setEmpty();
      break;

      case UniformType_Bool:
      case UniformType_Int:
          // JS only has Doubles.  Cast to Int.
          ans.setValue((int) val.toDouble());
          return ans;
      break;

      case UniformType_UInt:
          // JS only has Doubles.  Cast to UInt.
          ans.setValue( (uint32_t) val.toDouble());
          return ans;
      break;

      case UniformType_Float:
      case UniformType_Double:
          // JS only has Doubles.  Cast to Float.
          ans.setValue((float) val.toDouble() );
          return ans;
      break;

      // Vectors:  int
      case UniformType_iVec2:
            ans.setValue( val.toVec2() );
            return ans;
      case UniformType_iVec3:
            ans.setValue( val.toVec2() );
            return ans;
      case UniformType_iVec4:
            ans.setValue( val.toVec2() );
            return ans;
    //   {
    //     rtObjectRef    array = new rtArrayObject();
    //     rtObjectRef   valObj = val.toObject();
    //     rtObjectRef     keys = valObj.get<rtObjectRef>("allKeys");

    //     if (keys)
    //     {
    //       uint32_t len = keys.get<uint32_t>("length");
    //       for (uint32_t i = 0; i < len; i++)
    //       {
    //         piValue  val = valObj.get<piValue>(i);

    //         // JS only has Doubles.  Cast to Int.
    //         val.setInt32( (int32_t) val.toDouble() );

    //         ((rtArrayObject*) array.getPtr())->pushBack(val);
    //       }
    //     }

    //     ans.setObject(array);

    //     return ans;
    //   }
      break;
      // TODO: Possibly not in ES 2.0
      //
      //      // Vectors:  bool
      //    case UniformType_bVec2:
      //    case UniformType_bVec3:
      //    case UniformType_bVec4:
      //
      //      // Vectors:  uint
      //    case UniformType_uVec2:
      //    case UniformType_uVec3:
      //    case UniformType_uVec4:
      //
      //
      //      // Vectors:  double
      //    case UniformType_dVec2:
      //    case UniformType_dVec3:
      //    case UniformType_dVec4:

      // Vectors:  float
    //   case UniformType_Vec2:
    //   case UniformType_Vec3:
    //   case UniformType_Vec4:
    //   {
    //     rtObjectRef valObj = val.toObject();

    //     if(valObj)
    //     {
    //       rtObjectRef keys = valObj.get<rtObjectRef>("allKeys");

    //       if (keys)
    //       {
    //         rtObjectRef array = new rtArrayObject();

    //         uint32_t len = keys.get<uint32_t>("length");
    //         for (uint32_t i = 0; i < len; i++)
    //         {
    //           piValue val = valObj.get<piValue>(i);

    //           // JS only has Doubles.  Cast to Float.
    //           val.setFloat( (float) val.toDouble() );

    //           ((rtArrayObject*) array.getPtr())->pushBack(val);

    //         }//FOR

    //         ans.setObject(array);
    //       }
    //     }

    //     return ans;
    //   }
    //   break;

      case UniformType_Sampler2D:
        ans.setValue(val);
        return ans;
      break;
      case UniformType_Struct:
      break;
  }//SWITCH

  return ans;
}
//=====================================================================================================================================

