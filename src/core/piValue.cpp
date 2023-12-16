#include <stdio.h>
#include <iostream>
#include <string>

// #include "rtCore.h"
// #include "std::string.h"
// #include "rtObject.h"

// #define rtLogError printf

#include <piCore.h>
#include <piValue.h>

piValue::piValue()                      :mType(0) { setEmpty();   }
piValue::piValue(bool v)                :mType(0) { setBool(v);   }
piValue::piValue(int8_t v)              :mType(0) { setInt8(v);   }
piValue::piValue(uint8_t v)             :mType(0) { setUInt8(v);  }
piValue::piValue(int32_t v)             :mType(0) { setInt32(v);  }
piValue::piValue(uint32_t v)            :mType(0) { setUInt32(v); }
piValue::piValue(int64_t v)             :mType(0) { setInt64(v);  }
piValue::piValue(uint64_t v)            :mType(0) { setUInt64(v); }
piValue::piValue(float v)               :mType(0) { setFloat(v);  }
piValue::piValue(double v)              :mType(0) { setDouble(v); }

piValue::piValue(glm::vec2 v)           :mType(0) { setVec2(v);   }
piValue::piValue(glm::vec3 v)           :mType(0) { setVec3(v);   }
piValue::piValue(glm::vec4 v)           :mType(0) { setVec4(v);   }


  // void asn(glm::vec2 &v)                    { setVec2(v);     }
  // void asn(glm::vec3 &v)                    { setVec3(v);     }
  // void asn(glm::vec4 &v)                    { setVec4(v);     }


// piValue::piValue(const char* v)         :mType(0) { setString(v); }
// piValue::piValue(const std::string& v)     :mType(0) { setString(v); }
// piValue::piValue(const rtIObject* v)    :mType(0) { setObject(v); }
// piValue::piValue(const rtObjectRef& v)  :mType(0) { setObject(v); }
// piValue::piValue(const rtIFunction* v)  :mType(0) { setFunction(v); }
// piValue::piValue(const rtFunctionRef& v):mType(0) { setFunction(v); }
piValue::piValue(const piValue& v)      :mType(0) { setValue(v);  }
// piValue::piValue(voidPtr v)             :mType(0) { setVoidPtr(v); }
piValue::piValue(piValue&& v) noexcept  :mType(v.mType), mValue(v.mValue), mIsEmpty(v.mIsEmpty)
{
  // v.mValue.stringValue = nullptr;
  // v.mValue.objectValue = nullptr;
  // v.mValue.functionValue = nullptr;
  // v.mValue.voidPtrValue = nullptr;
}

piValue::~piValue()
{
  term();
}

bool piValue::operator==(const piValue& rhs) const
{
  return compare(*this, rhs);
}

piValue& piValue::operator=(piValue&& v) noexcept
{
  term();
  mType = v.mType;
  mValue = v.mValue;
  mIsEmpty = v.mIsEmpty;
  // v.mValue.stringValue = nullptr;
  // v.mValue.objectValue = nullptr;
  // v.mValue.functionValue = nullptr;
  return *this;
}

bool piValue::compare(const piValue& lhs, const piValue& rhs)
{
  bool result = false;
  if (lhs.getType() == rhs.getType())
  {
    switch(lhs.getType())
    {
    case PI_voidType:     result = true; break;
    case PI_boolType:     result = (lhs.mValue.boolValue   == rhs.mValue.boolValue);   break;
    case PI_int8_tType:   result = (lhs.mValue.int8Value   == rhs.mValue.int8Value);   break;
    case PI_uint8_tType:  result = (lhs.mValue.uint8Value  == rhs.mValue.uint8Value);  break;
    case PI_int32_tType:  result = (lhs.mValue.int32Value  == rhs.mValue.int32Value);  break;
    case PI_uint32_tType: result = (lhs.mValue.uint32Value == rhs.mValue.uint32Value); break;
    case PI_int64_tType:  result = (lhs.mValue.int64Value  == rhs.mValue.int64Value);  break;
    case PI_uint64_tType: result = (lhs.mValue.uint64Value == rhs.mValue.uint64Value); break;
    case PI_floatType:    result = (lhs.mValue.floatValue  == rhs.mValue.floatValue);  break;
    case PI_doubleType:   result = (lhs.mValue.doubleValue == rhs.mValue.doubleValue); break;
    case PI_stringType:
    {
      // if (lhs.mValue.stringValue && rhs.mValue.stringValue)
      //   result = (*lhs.mValue.stringValue == *rhs.mValue.stringValue);
      // else
      //   result = (lhs.mValue.stringValue == rhs.mValue.stringValue);
    }
    break;
    // case PI_objectType:   result = (lhs.mValue.objectValue == rhs.mValue.objectValue); break;
    // case PI_functionType: result = (lhs.mValue.functionValue == rhs.mValue.functionValue); break;
    }
  }
  return result;
}

// rtObjectRef piValue::toObject() const
// {
//   rtObjectRef v;
//   getObject(v);
//   return v;
// }

// rtFunctionRef piValue::toFunction() const
// {
//   rtFunctionRef f;
//   getFunction(f);
//   return f;
// }

void piValue::setEmpty()
{
  mIsEmpty = true;

  // if (mType == PI_objectType)
  // {
  //   if (mValue.objectValue)
  //   {
  //     mValue.objectValue->Release();
  //     mValue.objectValue = NULL;
  //   }
  // }
  // else if (mType == PI_functionType)
  // {
  //   if (mValue.functionValue)
  //   {
  //      mValue.functionValue->Release();
  //      mValue.functionValue = NULL;
  //   }
  // }
  // else if (mType == PI_stringType)
  // {
  //   if (mValue.stringValue)
  //   {
  //     delete mValue.stringValue;
  //     mValue.stringValue = NULL;
  //   }
  // }

  // TODO setting this to '0' makes node wrappers unhappy
  mType = 0;
  // TODO do we really need thi
  mValue.uint64Value = 0;
  //memset(&mValue, 0, sizeof(mValue));
}

void piValue::setValue(const piValue& v)
{
  if (this != &v)
  {
    setEmpty();
    mType = v.mType;
//     if (mType == PI_objectType && v.mValue.objectValue != NULL)
//     {
//       mValue.objectValue = v.mValue.objectValue;
//       mValue.objectValue->AddRef();
//     }
//     else if (mType == PI_functionType && v.mValue.functionValue != NULL)
//     {
//       mValue.functionValue = v.mValue.functionValue;
//       mValue.functionValue->AddRef();
//     }
// #if 1
//     else if (mType == PI_stringType && v.mValue.stringValue != NULL)
//     {
//       setString(*(v.mValue.stringValue));
//     }
// #endif
//    else
      mValue = v.mValue;
    mIsEmpty = v.mIsEmpty;
  }
}

void piValue::setBool(bool v)
{
  setEmpty();
  mType    = PI_boolType; mValue.boolValue = v;
  mIsEmpty = false;
}

void piValue::setInt8(int8_t v)
{
  setEmpty();
  mType    = PI_int8_tType; mValue.int8Value = v;
  mIsEmpty = false;
}

void piValue::setUInt8(uint8_t v)
{
  setEmpty();
  mType    = PI_uint8_tType; mValue.uint8Value = v;
  mIsEmpty = false;
}

void piValue::setInt32(int32_t v)
{
  setEmpty();
  mType     = PI_int32_tType; mValue.int32Value = v;
  mIsEmpty = false;
}

void piValue::setUInt32(uint32_t v)
{
  setEmpty();
  mType    = PI_uint32_tType; mValue.uint32Value = v;
  mIsEmpty = false;
}

void piValue::setInt64(int64_t v)
{
  setEmpty();
  mType    = PI_int64_tType; mValue.int64Value = v;
  mIsEmpty = false;
}

void piValue::setUInt64(uint64_t v)
{
  setEmpty();
  mType    = PI_uint64_tType; mValue.uint64Value = v;
  mIsEmpty = false;
}

void piValue::setFloat(float v)
{
  setEmpty();
  mType    = PI_floatType; mValue.floatValue = v;
  mIsEmpty = false;
}

void piValue::setDouble(double v)
{
  setEmpty();
  mType    = PI_doubleType; mValue.doubleValue = v;
  mIsEmpty = false;
}


void piValue::setVec2(glm::vec2 &v)
{
  setEmpty();
  mType    = PI_doubleType; mValue.vec2Value = v;
  mIsEmpty = false;
}

void piValue::setVec3(glm::vec3 &v)
{
  setEmpty();
  mType    = PI_doubleType; mValue.vec3Value = v;
  mIsEmpty = false;
}

void piValue::setVec4(glm::vec4 &v)
{
  setEmpty();
  mType    = PI_doubleType; mValue.vec4Value = v;
  mIsEmpty = false;
}


// void piValue::setString(const std::string& v)
// {
//   setEmpty();
//   mType    = PI_stringType; mValue.stringValue = new std::string(v);
//   mIsEmpty = false;
// }

// void piValue::setObject(const rtIObject* v)
// {
//   setEmpty();
//   mType    = PI_objectType;
//   mIsEmpty = false;
//   mValue.objectValue = (rtIObject*)v;
//   if (v)
//     mValue.objectValue->AddRef();
// }

// void piValue::setObject(const rtObjectRef& v)
// {
// #if 0
//   setEmpty();
//   mType = PI_objectType;
//   mValue.objectValue = v.getPtr();
//   if (v)
//     mValue.objectValue->AddRef();
// #else
//   setObject(v.getPtr());
// #endif
//   mIsEmpty = false;
// }

// void piValue::setFunction(const rtIFunction* v)
// {
//   setEmpty();
//   mType    = PI_functionType;
//   mIsEmpty = false;
//   mValue.functionValue = (rtIFunction*)v;
//   if (v) mValue.functionValue->AddRef();
// }

// void piValue::setFunction(const rtFunctionRef& v)
// {
//   setFunction(v.getPtr());
// }

// void piValue::setVoidPtr(voidPtr v)
// {
//   setEmpty();
//   mType    = PI_voidPtrType;
//   mIsEmpty = false;
//   mValue.voidPtrValue = v;
// }

piError piValue::getBool(bool& v) const
{
  if (mType == PI_boolType)
    v = mValue.boolValue;
  else
  {
    v = false; // normalize to default
    switch(mType)
    {
    case PI_voidType:       /* do nothing */                          break;
    case PI_boolType:     v = mValue.boolValue;                       break;
    case PI_int8_tType:   v = (mValue.int8Value==0)     ? false:true; break;
    case PI_uint8_tType:  v = (mValue.uint8Value==0)    ? false:true; break;
    case PI_int32_tType:  v = (mValue.int32Value==0)    ? false:true; break;
    case PI_uint32_tType: v = (mValue.uint32Value==0)   ? false:true; break;
    case PI_int64_tType:  v = (mValue.int64Value==0)    ? false:true; break;
    case PI_uint64_tType: v = (mValue.uint64Value==0)   ? false:true; break;
    case PI_floatType:    v = (mValue.floatValue==0.0f) ? false:true; break;
    case PI_doubleType:   v = (mValue.doubleValue==0.0) ? false:true; break;
    case PI_stringType:
    {
      // if (mValue.stringValue) {
      //   v = (*mValue.stringValue=="")?false:true;
      // }
    }
    break;
    // case PI_objectType: v = mValue.objectValue?     true:false; break;
    // case PI_functionType: v = mValue.functionValue? true:false; break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to bool.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getInt8(int8_t& v)  const
{
  if (mType == PI_int8_tType)
    v = mValue.int8Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:       /* do nothing */                break;
    case PI_boolType:     v = (int8_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (int8_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (int8_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (int8_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (int8_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (int8_t)mValue.int64Value;    break;
    case PI_uint64_tType: v = (int8_t)mValue.uint64Value;   break;
// TODO look at faster float to fixed conversion
    case PI_floatType:    v = (int8_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (int8_t)mValue.doubleValue;   break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (int8_t)atol(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType: /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to int8_t.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getUInt8(uint8_t& v) const
{
  if (mType == PI_uint8_tType)
    v = mValue.uint8Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */                  break;
    case PI_boolType:     v = (uint8_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (uint8_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (uint8_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (uint8_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (uint8_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (uint8_t)mValue.int64Value;    break;
    case PI_uint64_tType: v = (uint8_t)mValue.uint64Value;   break;
// TODO look at faster float to fixed conversion
#ifdef PX_RTVALUE_CAST_UINT_BASIC
    case PI_floatType:    v = (uint8_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (uint8_t)mValue.doubleValue;   break;
#else
    case PI_floatType:    v = (uint8_t)((int8_t)(mValue.floatValue));    break;
    case PI_doubleType:   v = (uint8_t)((int8_t)(mValue.doubleValue));   break;
#endif //PX_RTVALUE_CAST_UINT_BASIC
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (uint8_t)atol(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to uint8_t.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getInt32(int32_t& v) const
{
  if (mType == PI_int32_tType)
    v = mValue.int32Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:       /* do nothing */                 break;
    case PI_boolType:     v = (int32_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (int32_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (int32_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (int32_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (int32_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (int32_t)mValue.int64Value;    break;
    case PI_uint64_tType: v = (int32_t)mValue.uint64Value;   break;
// TODO look at faster float to fixed conversion
    case PI_floatType:    v = (int32_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (int32_t)mValue.doubleValue;   break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (int32_t)atol(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to int32_t.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getUInt32(uint32_t& v) const
{
  if (mType == PI_uint32_tType)
    v = mValue.uint32Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:       /* do nothing */                  break;
    case PI_boolType:     v = (uint32_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (uint32_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (uint32_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (uint32_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (uint32_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (uint32_t)mValue.int32Value;    break;
    case PI_uint64_tType: v = (uint32_t)mValue.uint32Value;   break;
// TODO look at faster float to fixed conversion
#ifdef PX_RTVALUE_CAST_UINT_BASIC
    case PI_floatType:    v = (uint32_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (uint32_t)mValue.doubleValue;   break;
#else
    case PI_floatType:    v = (uint32_t)((int32_t)(mValue.floatValue));    break;
    case PI_doubleType:   v = (uint32_t)((int32_t)(mValue.doubleValue));   break;
#endif //PX_RTVALUE_CAST_UINT_BASIC
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (uint32_t)atol(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to uint32_t.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getInt64(int64_t& v) const
{
  if (mType == PI_int64_tType)
    v = mValue.int64Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:       /* do nothing */                 break;
    case PI_boolType:     v = (int64_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (int64_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (int64_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (int64_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (int64_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (int64_t)mValue.int64Value;    break;
    case PI_uint64_tType: v = (int64_t)mValue.uint64Value;   break;
// TODO look at faster float to fixed conversion
    case PI_floatType:    v = (int64_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (int64_t)mValue.doubleValue;   break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (int64_t)atoll(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to int64.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getUInt64(uint64_t& v) const
{
  if (mType == PI_uint64_tType)
    v = mValue.uint64Value;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */                   break;
    case PI_boolType:     v = (uint64_t)mValue.boolValue?1:0; break;
    case PI_int8_tType:   v = (uint64_t)mValue.int8Value;     break;
    case PI_uint8_tType:  v = (uint64_t)mValue.uint8Value;    break;
    case PI_int32_tType:  v = (uint64_t)mValue.int32Value;    break;
    case PI_uint32_tType: v = (uint64_t)mValue.uint32Value;   break;
    case PI_int64_tType:  v = (uint64_t)mValue.int64Value;    break;
    case PI_uint64_tType: v = (uint64_t)mValue.uint64Value;   break;
// TODO look at faster float to fixed conversion
#ifdef PX_RTVALUE_CAST_UINT_BASIC
    case PI_floatType:    v = (uint64_t)mValue.floatValue;    break;
    case PI_doubleType:   v = (uint64_t)mValue.doubleValue;   break;
#else
    case PI_floatType:    v = (uint64_t)((int64_t)(mValue.floatValue));    break;
    case PI_doubleType:   v = (uint64_t)((int64_t)(mValue.doubleValue));   break;
#endif
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (uint64_t)atoll(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to uint64_t.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getFloat(float& v) const
{
  if (mType == PI_floatType)
    v = mValue.floatValue;
  else
  {
    v = 0.0f; // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */               break;
    case PI_boolType:     v = mValue.boolValue?1.0f:0.0f; break;
    case PI_int8_tType:   v = (float)mValue.int8Value;    break;
    case PI_uint8_tType:  v = (float)mValue.uint8Value;   break;
    case PI_int32_tType:  v = (float)mValue.int32Value;   break;
    case PI_uint32_tType: v = (float)mValue.uint32Value;  break;
    case PI_int64_tType:  v = (float)mValue.int64Value;   break;
    case PI_uint64_tType: v = (float)mValue.uint64Value;  break;
//    case PI_floatType: break;
    case PI_doubleType: v = (float)mValue.doubleValue;    break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = (float)atof(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to float.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getDouble(double& v) const
{
  if (mType == PI_doubleType)
    v = mValue.doubleValue;
  else
  {
    v = 0; // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */               break;
    case PI_boolType:     v = mValue.boolValue?1.0:0.0;   break;
    case PI_int8_tType:   v = (double)mValue.int8Value;   break;
    case PI_uint8_tType:  v = (double)mValue.uint8Value;  break;
    case PI_int32_tType:  v = (double)mValue.int32Value;  break;
    case PI_uint32_tType: v = (double)mValue.uint32Value; break;
    case PI_int64_tType:  v = (double)mValue.int64Value;  break;
    case PI_uint64_tType: v = (double)mValue.uint64Value; break;
    case PI_floatType:    v = (double)mValue.floatValue;  break;
//    case PI_doubleType: break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = atof(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to double.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getVec2(glm::vec2& v) const
{
  if (mType == PI_vec2Type)
    v = mValue.vec2Value;
  else
  {
    v = glm::vec2(0, 0); // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */               break;
    case PI_boolType:     v = glm::vec2(mValue.boolValue ? 1.0:0.0, 0); break;
    case PI_int8_tType:   v = glm::vec2(mValue.int8Value,   0); break;
    case PI_uint8_tType:  v = glm::vec2(mValue.uint8Value,  0); break;
    case PI_int32_tType:  v = glm::vec2(mValue.int32Value,  0); break;
    case PI_uint32_tType: v = glm::vec2(mValue.uint32Value, 0); break;
    case PI_int64_tType:  v = glm::vec2(mValue.int64Value,  0); break;
    case PI_uint64_tType: v = glm::vec2(mValue.uint64Value, 0); break;
    case PI_floatType:    v = glm::vec2(mValue.floatValue,  0); break;
//    case PI_doubleType: break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = atof(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to double.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}


piError piValue::getVec3(glm::vec3& v) const
{
  if (mType == PI_vec3Type)
    v = mValue.vec3Value;
  else
  {
    v = glm::vec3(0, 0, 0); // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */               break;
    case PI_boolType:     v = glm::vec3(mValue.boolValue ? 1.0:0.0, 0, 0); break;
    case PI_int8_tType:   v = glm::vec3(mValue.int8Value,   0, 0); break;
    case PI_uint8_tType:  v = glm::vec3(mValue.uint8Value,  0, 0); break;
    case PI_int32_tType:  v = glm::vec3(mValue.int32Value,  0, 0); break;
    case PI_uint32_tType: v = glm::vec3(mValue.uint32Value, 0, 0); break;
    case PI_int64_tType:  v = glm::vec3(mValue.int64Value,  0, 0); break;
    case PI_uint64_tType: v = glm::vec3(mValue.uint64Value, 0, 0); break;
    case PI_floatType:    v = glm::vec3(mValue.floatValue,  0, 0); break;
//    case PI_doubleType: break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = atof(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to double.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

piError piValue::getVec4(glm::vec4& v) const
{
  if (mType == PI_vec4Type)
    v = mValue.vec4Value;
  else
  {
    v = glm::vec4(0, 0, 0, 0); // normalize to default
    switch(mType)
    {
    case PI_voidType:      /* do nothing */               break;
    case PI_boolType:     v = glm::vec4(mValue.boolValue ? 1.0:0.0, 0, 0, 0); break;
    case PI_int8_tType:   v = glm::vec4(mValue.int8Value,   0, 0, 0); break;
    case PI_uint8_tType:  v = glm::vec4(mValue.uint8Value,  0, 0, 0); break;
    case PI_int32_tType:  v = glm::vec4(mValue.int32Value,  0, 0, 0); break;
    case PI_uint32_tType: v = glm::vec4(mValue.uint32Value, 0, 0, 0); break;
    case PI_int64_tType:  v = glm::vec4(mValue.int64Value,  0, 0, 0); break;
    case PI_uint64_tType: v = glm::vec4(mValue.uint64Value, 0, 0, 0); break;
    case PI_floatType:    v = glm::vec4(mValue.floatValue,  0, 0, 0); break;
//    case PI_doubleType: break;
    case PI_stringType:
    {
      // if (mValue.stringValue)
      //   v = atof(mValue.stringValue->cString());
    }
    break;
    // case PI_objectType:   /* Leave as default */ break;
    // case PI_functionType: /* Leave as default */ break;
    // case PI_voidPtrType:  /* Leave as default */ break;
    default:
      fprintf(stderr,"No conversion from %s to double.", rtStrType(mType));
      break;
    }
  }
  return PI_OK;
}

// piError piValue::getString(std::string& v) const
// {
//   if (mType == PI_stringType && mValue.stringValue)
//     v = *mValue.stringValue;
//   else
//   {
//     // TODO EVIL buffer on stack
//     char buffer[256]; buffer[0] = 0;
//     switch(mType)
//     {
//     case PI_voidType:     /* do nothing */               break;
//     case PI_boolType:     sprintf(buffer, "%s", mValue.boolValue?"true":"false"); break;
//     case PI_int8_tType:   sprintf(buffer, "%d", mValue.int8Value);   break;
//     case PI_uint8_tType:  sprintf(buffer, "%u", mValue.uint8Value);  break;
//     case PI_int32_tType:  sprintf(buffer, "%d", mValue.int32Value);  break;
//     case PI_uint32_tType: sprintf(buffer, "%u", mValue.uint32Value); break;
//     case PI_int64_tType:  sprintf(buffer, "%lld", (long long int)mValue.int64Value); break;
//     case PI_uint64_tType: sprintf(buffer, "%llu", (unsigned long long int)mValue.uint64Value); break;
//     case PI_floatType:    sprintf(buffer, "%g", mValue.floatValue);  break;
//     case PI_doubleType:   sprintf(buffer, "%g", mValue.doubleValue); break;
//       // TODO call toString or description on object
//     case PI_objectType: break;
//     case PI_functionType: break;
//     case PI_voidPtrType:  /* Leave as default */ break;
//     default:
//       fprintf(stderr,"No conversion from %s to string.", rtStrType(mType));
//       break;
//     }
//     v = buffer;
//   }
//   return PI_OK;
// }

// piError piValue::getObject(rtObjectRef& v) const
// {
//   if (mType == PI_objectType)
//     v = mValue.objectValue;
//   else
//   {
//     // No other types are convertable to object
//     v = NULL;
//     return PI_ERROR_TYPE_MISMATCH;;
//   }
//   return PI_OK;
// }

// piError piValue::getFunction(rtFunctionRef& v) const
// {
//   if (mType == PI_functionType)
//     v = mValue.functionValue;
//   else
//   {
//     // No other types are convertable to function
//     v = NULL;
//     return PI_ERROR_TYPE_MISMATCH;;
//   }
//   return PI_OK;
// }

// piError piValue::getVoidPtr(voidPtr& v) const
// {
//   if (mType == PI_voidPtrType)
//     v = mValue.voidPtrValue;
//   else
//   {
//     // No other types are convertable
//     v = NULL;
//     return PI_ERROR_TYPE_MISMATCH;
//   }
//   return PI_OK;
// }

piError piValue::getValue(piValue& v) const
{
  v = *this;
  return PI_OK;
}

const char* rtStrType(char t)
{
  const char* s = "UNKNOWN";
  switch(t)
  {
    PI_TYPE_CASE(PI_voidType);
    PI_TYPE_CASE(PI_boolType);
    PI_TYPE_CASE(PI_int8_tType);
    PI_TYPE_CASE(PI_uint8_tType);
    PI_TYPE_CASE(PI_int32_tType);
    PI_TYPE_CASE(PI_uint32_tType);
    PI_TYPE_CASE(PI_int64_tType);
    PI_TYPE_CASE(PI_uint64_tType);
    PI_TYPE_CASE(PI_floatType);
    PI_TYPE_CASE(PI_doubleType);
    PI_TYPE_CASE(PI_stringType);
    PI_TYPE_CASE(PI_objectType);
    PI_TYPE_CASE(PI_functionType);
    PI_TYPE_CASE(PI_voidPtrType);
    default:
    break;
  }
  return s;
}
