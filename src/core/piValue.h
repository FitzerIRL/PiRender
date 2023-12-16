#pragma once
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define finline inline

#define PI_voidType               '\0'
#define PI_valueType              'v'
#define PI_rtValueType            'v'
#define PI_boolType               'b'
#define PI_int8_tType             '1'
#define PI_uint8_tType            '2'
#define PI_intType                '4'
#define PI_int32_tType            '4'
#define PI_uint32_tType           '5'
#define PI_int64_tType            '6'
#define PI_uint64_tType           '7'
#define PI_floatType              'e'
#define PI_doubleType             'd'

#define PI_vec2Type               'l'
#define PI_vec3Type               'm'
#define PI_vec4Type               'n'

#define PI_stringType             's'
#define PI_rtStringType           's'
#define PI_objectType             'o'
#define PI_rtObjectRefType        'o'
#define PI_functionType           'f'
#define PI_rtFunctionRefType      'f'
#define PI_voidPtrType            'z'

// TODO JR Hack Only needed for reflection... method signature
//Try #define CHARIZE(x) #x[0]
//http://www.complete-concrete-concise.com/programming/c/preprocessor-%E2%80%93-understanding-the-stringizing-operator

#define PI_voidType2               "\0"
#define PI_valueType2              "v"
#define PI_rtValueType2            "v"
#define PI_boolType2               "b"
#define PI_int8_tType2             "1"
#define PI_uint8_tType2            "2"
#define PI_intType2                "4"
#define PI_int32_tType2            "4"
#define PI_uint32_tType2           "5"
#define PI_int64_tType2            "6"
#define PI_uint64_tType2           "7"
#define PI_floatType2              "e"
#define PI_doubleType2             "d"
#define PI_rtStringType2           "s"
#define PI_rtObjectRefType2        "o"
#define PI_rtFunctionRefType2      "f"
#define PI_voidPtrType2            "z"

class rtIObject;
class rtIFunction;
class rtObject;
class rtObjectRef;
class rtFunctionRef;

const char* rtStrType(char t); //fwd

typedef void* voidPtr;

union rtValue_
{
  bool        boolValue;
  int8_t      int8Value;
  uint8_t     uint8Value;
  int32_t     int32Value;
  int64_t     int64Value;
  uint64_t    uint64Value;
  uint32_t    uint32Value;
  float       floatValue;
  double      doubleValue;

  glm::vec2   vec2Value;
  glm::vec3   vec3Value;
  glm::vec4   vec4Value;

  // std::string    *stringValue;
  // rtIObject   *objectValue;
  // rtIFunction *functionValue;
  // voidPtr     voidPtrValue;  // For creating mischief
};

typedef char rtType;

class piValue
{
 public:
  piValue();
  piValue(bool v);
  piValue(int8_t v);
  piValue(uint8_t v);
  piValue(int32_t v);
  piValue(uint32_t v);
  piValue(int64_t v);
  piValue(uint64_t v);
  piValue(float v);
  piValue(double v);
  piValue(const char* v);
  // piValue(const std::string& v);
  // piValue(const rtIObject* v);
  // piValue(const rtObjectRef& v);
  // piValue(const rtIFunction* v);
  // piValue(const rtFunctionRef& v);

  piValue(glm::vec2 v);
  piValue(glm::vec3 v);
  piValue(glm::vec4 v);

  piValue(const piValue& v);
  piValue(voidPtr v);
  piValue(piValue&& v) noexcept;
  ~piValue();

  void term() { setEmpty(); }

  finline piValue& operator=(bool v)                { setBool(v);     return *this; }
  finline piValue& operator=(int8_t v)              { setInt8(v);     return *this; }
  finline piValue& operator=(uint8_t v)             { setUInt8(v);    return *this; }
  finline piValue& operator=(int32_t v)             { setInt32(v);    return *this; }
  finline piValue& operator=(uint32_t v)            { setUInt32(v);   return *this; }
  finline piValue& operator=(int64_t v)             { setInt64(v);    return *this; }
  finline piValue& operator=(uint64_t v)            { setUInt64(v);   return *this; }
  finline piValue& operator=(float v)               { setFloat(v);    return *this; }
  finline piValue& operator=(double v)              { setDouble(v);   return *this; }

  finline piValue& operator=(glm::vec2 v)           { setVec2(v);     return *this; }
  finline piValue& operator=(glm::vec3 v)           { setVec3(v);     return *this; }
  finline piValue& operator=(glm::vec4 v)           { setVec4(v);     return *this; }

  // finline piValue& operator=(const char* v)         { setString(v);   return *this; }
  // finline piValue& operator=(const std::string& v)     { setString(v);   return *this; }
  // finline piValue& operator=(const rtIObject* v)    { setObject(v);   return *this; }
  // finline piValue& operator=(const rtObjectRef& v)  { setObject(v);   return *this; }
  // finline piValue& operator=(const rtIFunction* v)  { setFunction(v); return *this; }
  // finline piValue& operator=(const rtFunctionRef& v){ setFunction(v); return *this; }
  finline piValue& operator=(const piValue& v)      { setValue(v);    return *this; }
  // finline piValue& operator=(voidPtr v)             { setVoidPtr(v);  return *this; }

  bool operator!=(const piValue& rhs) const { return !(*this == rhs); }
  bool operator==(const piValue& rhs) const;

  piValue& operator=(piValue&& v) noexcept;

  finline bool       toBool()     const { bool v;        getBool(v);   return v; }
  finline int8_t     toInt8()     const { int8_t v;      getInt8(v);   return v; }
  finline uint8_t    toUInt8()    const { uint8_t v;     getUInt8(v);  return v; }
  finline int32_t    toInt32()    const { int32_t v;     getInt32(v);  return v; }
  finline uint32_t   toUInt32()   const { uint32_t v(0); getUInt32(v); return v; }
  finline int64_t    toInt64()    const { int64_t v(0);  getInt64(v);  return v; }
  finline uint64_t   toUInt64()   const { uint64_t v(0); getUInt64(v); return v; }
  finline float      toFloat()    const { float v;       getFloat(v);  return v; }
  finline double     toDouble()   const { double v;      getDouble(v); return v; }

  finline glm::vec2  toVec2()     const { glm::vec2 v;   getVec2(v);   return v; }
  finline glm::vec3  toVec3()     const { glm::vec3 v;   getVec3(v);   return v; }
  finline glm::vec4  toVec4()     const { glm::vec4 v;   getVec4(v);   return v; }

  // finline std::string   toString()   const { std::string v;    getString(v); return v; }
  // rtObjectRef        toObject()   const;
  // rtFunctionRef      toFunction() const;
  // voidPtr            toVoidPtr()  const { voidPtr v;     getVoidPtr(v);return v; }

  rtType getType() const { return mType; }

  const char *getTypeStr() const { return ::rtStrType(mType); }

  finline bool isEmpty() const { return mIsEmpty; };

  void setEmpty();
  void setValue(const piValue& v);
  void setBool(bool v);
  void setInt8(int8_t v);
  void setUInt8(uint8_t v);
  void setInt32(int32_t v);
  void setUInt32(uint32_t v);
  void setInt64(int64_t v);
  void setUInt64(uint64_t v);
  void setFloat(float v);
  void setDouble(double v);
  // void setString(const std::string& v);
  // void setObject(const rtIObject* v);
  // void setObject(const rtObjectRef& v);
  // void setFunction(const rtIFunction* v);
  // void setFunction(const rtFunctionRef& v);

  void setVec2(glm::vec2 &v);
  void setVec3(glm::vec3 &v);
  void setVec4(glm::vec4 &v);

  void setVoidPtr(voidPtr v);

  piError getValue(piValue& v)          const;
  piError getBool(bool& v)              const;
  piError getInt8(int8_t& v)            const;
  piError getUInt8(uint8_t& v)          const;
  piError getInt32(int32_t& v)          const;
  piError getInt64(int64_t& v)          const;
  piError getUInt64(uint64_t& v)        const;
  piError getUInt32(uint32_t& v)        const;
  piError getFloat(float& v)            const;
  piError getDouble(double& v)          const;

  piError getVec2(glm::vec2 &v)         const;
  piError getVec3(glm::vec3 &v)         const;
  piError getVec4(glm::vec4 &v)         const;

  // piError getString(std::string& v)        const;
  // piError getObject(rtObjectRef& v)     const;
  // piError getFunction(rtFunctionRef& v) const;
  // piError getVoidPtr(voidPtr& v)        const;

  // TODO rework this so we avoid a copy if the type matches
  template <typename T>
    T convert() const { T t; cvt(t); return t; }

  // template <typename T>
  // piError tryConvert (T& t) { return cvt(t); }

  template <typename T>
    void assign(const T t) { asn(t); }

 protected:

  // Both values must have the same type
  static bool compare(const piValue& lhs, const piValue& rhs);

   piError cvt(piValue& v)                const { return getValue(v);    }
   piError cvt(bool& v)                   const { return getBool(v);     }
   piError cvt(int8_t& v)                 const { return getInt8(v);     }
   piError cvt(uint8_t& v)                const { return getUInt8(v);    }
   piError cvt(int32_t& v)                const { return getInt32(v);    }
   piError cvt(uint32_t& v)               const { return getUInt32(v);   }
   piError cvt(int64_t& v)                const { return getInt64(v);    }
   piError cvt(uint64_t& v)               const { return getUInt64(v);   }
   piError cvt(float& v)                  const { return getFloat(v);    }
   piError cvt(double& v)                 const { return getDouble(v);   }

   piError cvt(glm::vec2 &v)              const { return getVec2(v);     }
   piError cvt(glm::vec3 &v)              const { return getVec3(v);     }
   piError cvt(glm::vec4 &v)              const { return getVec4(v);     }

  //  piError cvt(std::string& v)               const { return getString(v);   }
  //  piError cvt(rtObjectRef& v)            const { return getObject(v);   }
  //  piError cvt(rtFunctionRef& v)          const { return getFunction(v); }
  //  piError cvt(voidPtr& v)                const { return getVoidPtr(v);  }

  void asn(const piValue& v)                { setValue(v);    }
  void asn(bool v)                          { setBool(v);     }
  void asn(int8_t v)                        { setInt8(v);     }
  void asn(uint8_t v)                       { setUInt8(v);    }
  void asn(int32_t v)                       { setInt32(v);    }
  void asn(uint32_t v)                      { setUInt32(v);   }
  void asn(int64_t v)                       { setInt64(v);    }
  void asn(uint64_t v)                      { setUInt64(v);   }
  void asn(float v)                         { setFloat(v);    }
  void asn(double v)                        { setDouble(v);   }

  void asn(glm::vec2 &v)                    { setVec2(v);     }
  void asn(glm::vec3 &v)                    { setVec3(v);     }
  void asn(glm::vec4 &v)                    { setVec4(v);     }

  // void asn(const char* v)                   { setString(v);   }
  // void asn(const std::string& v)               { setString(v);   }
  // void asn(const rtIObject* v)              { setObject(v);   }
  // void asn(const rtObjectRef& v)            { setObject(v);   }
  // void asn(const rtIFunction* v)            { setFunction(v); }
  // void asn(const rtFunctionRef& v)          { setFunction(v); }
  // void asn(voidPtr v)                       { setVoidPtr(v);  }

  // piError coerceType(rtType newType);

  rtType   mType;
  rtValue_ mValue;

  bool     mIsEmpty;
};

#define PI_TYPE_CASE(t) case t: s = # t; break;

#define PI_TYPE_NAME(t) (# t)
