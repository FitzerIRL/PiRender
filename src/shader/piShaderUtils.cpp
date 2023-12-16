#include <chrono>


#include <piUtils.h>
#include <piShaderUtils.h>


// assume premultiplied
static const char *fSolidShaderText =
  "#ifdef GL_ES \n"
  "  precision mediump float; \n"
  "#endif \n"
  "uniform float u_alpha;"
  "uniform vec4 a_color;"
  "void main()"
  "{"
  "  gl_FragColor = a_color*u_alpha;"
  "}";


/*static*/ const char *vShaderText =
  "uniform vec2 u_resolution;"
  "uniform mat4 amymatrix;"
  "uniform bool u_flipY;"
  "attribute vec2 pos;"
  "attribute vec2 uv;"
  "varying vec2 v_uv;"
  "void main()"
  "{"
  // map from "pixel coordinates"
  "  vec4 p = amymatrix * vec4(pos, 0, 1);"
  "  vec4 zeroToOne = p / vec4(u_resolution, u_resolution.x, 1);"
  "  vec4 zeroToTwo = zeroToOne * vec4(2.0, 2.0, 1, 1);"
  "  vec4 clipSpace = zeroToTwo - vec4(1.0, 1.0, 0, 0);"
  "  clipSpace.w = 1.0+clipSpace.z;"
  "  int yScale = -1;"
  "  if (u_flipY) { yScale = 1; }"
  "  gl_Position = clipSpace * vec4(1, yScale, 1, 1);"
  "  v_uv = uv;"
  "}";




static  std::chrono::time_point<std::chrono::high_resolution_clock> epoch; //= std::chrono::high_resolution_clock::now();

extern const char* vShaderText;
extern int currentGLProgram;

extern piValue copyUniform(UniformType_t type, piValue &val);        // helper to copy "variant" like UNIFORM types

////////////////////////////////////////////////////////////////
//
// Debug Statistics
#ifdef USE_RENDER_STATS

extern uint32_t gDrawCalls;
extern uint32_t gTexBindCalls;
extern uint32_t gFboBindCalls;

#define TRACK_DRAW_CALLS()   { gDrawCalls++;    }
#define TRACK_TEX_CALLS()    { gTexBindCalls++; }
#define TRACK_FBO_CALLS()    { gFboBindCalls++; }

#else

#define TRACK_DRAW_CALLS()
#define TRACK_TEX_CALLS()
#define TRACK_FBO_CALLS()

#endif

#define SAFE_DELETE(p)  if(p) { delete p; p = NULL; };

////////////////////////////////////////////////////////////////

shaderProgram::shaderProgram() : mProgram(-1), mFragShader(-1), mVertShader(-1), mTimeLoc(-1),
mResolutionLoc(-1), mMatrixLoc(-1), mPosLoc(-1),
mUVLoc(-1), mAlphaLoc(-1), mColorLoc(-1)
{
  mUniform_map.clear();

  // Get the current time point
  epoch = std::chrono::high_resolution_clock::now();

}

shaderProgram::~shaderProgram()
{
  glDetachShader(mProgram, mFragShader);
  glDetachShader(mProgram, mVertShader);
  glDeleteShader(mFragShader);
  glDeleteShader(mVertShader);
  glDeleteProgram(mProgram);

  // mUniform_map.clear();
}

piError shaderProgram::initShader(const char* v, const char* f)
{
  fprintf(stderr,"INFO: initShader() - ENTER  123\n");

  if(f)
  {
    std::string vtxStr(vShaderText); // (v);
    std::string frgStr(fSolidShaderText); // (f);

    glShaderProgDetails_t details = createShaderProgram( vtxStr.c_str(), frgStr.c_str());

    if(details.didError)
    {
      mProgram     = -1; // ERROR
      mCompilation = details.compilation;
      return PI_FAIL;
    }

    mProgram    = details.program;
    mFragShader = details.fragShader;
    mVertShader = details.vertShader;

    prelink();
    linkShaderProgram(mProgram);
    postlink();
  }
  else
  {
    fprintf(stderr,"No FRAGMENT Shader defined. \n");
    return PI_FAIL;
  }

  return PI_OK;
}

int shaderProgram::getUniformLocation(const char* name)
{
  if(mProgram == -1)
  {
    fprintf(stderr,"getUniformLocation() - '%s' ... Shader program undefined.  (-1)", name);
    return -1;
  }

  int l = glGetUniformLocation(mProgram, name);

  if (l == -1)
  {
    fprintf(stderr,"Shader does not define uniform '%s'.\n", name);
  }

  return l;
}

void shaderProgram::use()
{
  if(currentGLProgram != mProgram)
  {
    currentGLProgram = mProgram;
    glUseProgram(mProgram);
  }
}

void shaderProgram::saveUniforms()
{
  copyUniforms(mUniform_map, mUniform_map2);
}

void shaderProgram::restoreUniforms()
{
  copyUniforms(mUniform_map2, mUniform_map, true); // needs update
}

void shaderProgram::copyUniforms(UniformMap_t &from, UniformMap_t &to, bool needsUpdate /* = false */)
{
  //
  // Copy the UNIFORM: "from" -> "to" ...
  //
  if(from.size() > 0)
  {
    for(UniformMapIter_t it  = from.begin();
                         it != from.end(); ++it)
    {
      const std::string &n = (*it).first;
      uniformLoc_t   &p = (*it).second;

      to[n].value = copyUniform(p.type, p.value);

	  to[n].needsUpdate = needsUpdate;
    }
  }//ENDIF
}


static double last_ms = 0.0;

piError shaderProgram::draw(int resW, int resH, float* matrix, float alpha,
                            piTextureRef t,
                            GLenum mode,
                            const void* pos,
                            const void* uv,
                            int count)
{

  if(!resW || !resH || !matrix ) //|| !pos)
  {
    return PI_FAIL;
  }

  if(mResolutionLoc == -1)
  {
    return PI_FAIL;
  }

  use(); // this program ! (if different)

  checkGLError("Background - draw USE ... ");

  //
  // Always update UNIFORM(S) ...
  //
  glUniformMatrix4fv(mMatrixLoc, 1, GL_FALSE, matrix);
  glUniform2f(mResolutionLoc, static_cast<GLfloat>(resW), static_cast<GLfloat>(resH));

  fprintf(stderr,"INFO: draw() - update  mResolutionLoc: %d    WxH: %d x %d\n", mResolutionLoc, resW, resH);
  fprintf(stderr,"INFO: draw() - update  t: %d    alpha: %f\n", t, alpha);

  checkGLError("Background - draw RES ...");

  //
  // Update specific UNIFORMS ...
  //
  if(mTimeLoc != -1)
  {
    // fprintf(stderr,"INFO: draw() - update TIME \n");

    // double time_ms = pxMilliseconds();

    // Get the time elapsed since the start
    auto end = std::chrono::high_resolution_clock::now();
    // double time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - epoch);

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - epoch);
    double time_ms = static_cast<double>(duration.count());


    if(last_ms == 0)
      last_ms = time_ms;

    glUniform1f(mTimeLoc, (float) ((time_ms - last_ms) / 1000.0) );
  }

  if(mAlphaLoc != -1)
  {
    fprintf(stderr,"INFO: draw() - update ALPHA \n");

    glUniform1f(mAlphaLoc, alpha);
  }

//  if(mColorLoc != -1)
//  {
//    float color[4] = {1,0,0,1};
//    glUniform4fv(mColorLoc, 1, color);
//  }

  //
  // Bind to object FBO if needed
  //
/*
  if(t)
  {
    UniformMapIter_t found = mUniform_map.find( "s_texture" );
    if(found !=  mUniform_map.end() )
    {
      uniformLoc_t &p = (*found).second;

      if(t->bindGLTexture(p.loc) != PX_OK)  // to GL_TEXTURE1
      {
        fprintf(stderr,"Texture Bind failed !!");
      }
    }
  }
*/

  //
  // Apply updated UNIFORM values to GPU...
  //
  if(mUniform_map.size() > 0)
  {
    for(UniformMapIter_t it  = mUniform_map.begin();
                         it != mUniform_map.end(); ++it)
    {
      uniformLoc_t &p = (*it).second;

    fprintf(stderr,"\nINFO: draw() - update UNIFORM: %s", p.name.c_str());

      if(p.setFunc && p.needsUpdate)
      {
        p.setFunc(p); // SET UNIFORM ... set p.value .. calls glUniformXXX() and glBindTexture(3,4,5) calls ... etc.

        p.needsUpdate = (p.type == UniformType_Sampler2D); // always bind Samplers... otherwise once will do.
      }
    }
  }//ENDIF

  //
  // DRAW
  //
  if(uv)
  {
    fprintf(stderr,"INFO: draw() - UV \n");

    glVertexAttribPointer(mPosLoc, 2, GL_FLOAT, GL_FALSE, 0, pos);
    glVertexAttribPointer(mUVLoc,  2, GL_FLOAT, GL_FALSE, 0, uv);

    glEnableVertexAttribArray(mPosLoc);
    glEnableVertexAttribArray(mUVLoc);
    glDrawArrays(mode, 0, count);  TRACK_DRAW_CALLS();
    glDisableVertexAttribArray(mPosLoc);
    glDisableVertexAttribArray(mUVLoc);
  }
  else
  {
    fprintf(stderr,"INFO: draw() - nonUV    mPosLoc: %d  >> %d \n", mPosLoc, count);

    glVertexAttribPointer(mPosLoc, 2, GL_FLOAT, GL_FALSE, 0, pos);

checkGLError("Background - draw USE 11 ... ");

    glEnableVertexAttribArray(mPosLoc);

checkGLError("Background - draw USE 22... ");

    glDrawArrays(mode, 0, count);  TRACK_DRAW_CALLS();
    glDisableVertexAttribArray(mPosLoc);
  }

  return PI_OK;
}

//=====================================================================================================================================

glShaderProgDetails_t  createShaderProgram(const char* vShaderTxt, const char* fShaderTxt)
{
  fprintf(stderr,"INFO: createShaderProgram() - ENTER \n");

  glShaderProgDetails_t details = { 0,0,0,   false, "" };
  GLint stat;

  details.fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(details.fragShader, 1, (const char **) &fShaderTxt, NULL);
  glCompileShader(details.fragShader);
  glGetShaderiv(details.fragShader, GL_COMPILE_STATUS, &stat);

  if (!stat)
  {
    fprintf(stderr,"FRAGMENT SHADER - Error: Shader did not compile: %d", glGetError());

    GLint maxLength = 0;
    glGetShaderiv(details.fragShader, GL_INFO_LOG_LENGTH, &maxLength);

    //The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(details.fragShader, maxLength, &maxLength, (char *) &errorLog[0]);

    fprintf(stderr,"%s", (char *) &errorLog[0]);

    //Exit with failure.
    glDeleteShader(details.fragShader); //Don't leak the shader.

    details.didError    = true; // ERROR
    details.compilation = std::string("FRAGMENT SHADER - Compile Error: ") + &errorLog[0];

    return details;
  }

  details.vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(details.vertShader, 1, (const char **) &vShaderTxt, NULL);
  glCompileShader(details.vertShader);
  glGetShaderiv(details.vertShader, GL_COMPILE_STATUS, &stat);

  if (!stat)
  {
    char log[1000];
    GLsizei len;
    glGetShaderInfoLog(details.vertShader, 1000, &len, log);
    fprintf(stderr,"VERTEX SHADER - Failed to compile:  [%s]", log);

    GLenum err = glGetError();
    fprintf(stderr,"VERTEX SHADER - glGetError(): %d",err);

    details.didError    = true; // ERROR
    details.compilation = std::string("VERTEX SHADER - Compile Error: ") + std::string(log);

    return details;
  }

  details.program = glCreateProgram();
  glAttachShader(details.program, details.fragShader);
  glAttachShader(details.program, details.vertShader);
  return details;
}

void linkShaderProgram(GLuint program)
{
  GLint stat;

  glLinkProgram(program);  // needed to put attribs into effect
  glGetProgramiv(program, GL_LINK_STATUS, &stat);

  if (!stat)
  {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program, 1000, &len, log);
    fprintf(stderr,"VERTEX SHADER - Failed to link:  [%s]", log);

//    details.program     = -1; // ERROR
//    details.compilation = std::string("VERTEX SHADER - Link Error: ") + std::string(log);

    // TODO purge all exit calls
    // exit(1);
  }
}

//=====================================================================================================================================
