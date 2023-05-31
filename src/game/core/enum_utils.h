#pragma once

#include <map>
#include <sstream>
#include <string>

/**
 * Declares the enum and related functions. Use this in the header files.
 */
#define DECLARE_ENUM(name, type, values)                \
  enum class name : type { values };                    \
                                                        \
  namespace enums {                                     \
  namespace name {                                      \
  /*Returns the mapping of enum value to names*/        \
  const std::map<::crayon::name, const char*>& names(); \
  /*Returns the name of the enum*/                      \
  const char* c_str(::crayon::name value);              \
  } /*<EnumName>*/                                      \
  } /*enums*/

/**
 * Defines the enum-related functions. Use this in the source files.
 */
#define DEFINE_ENUM(name, values)                            \
  namespace enums {                                          \
  namespace name {                                           \
  using NameMapping = std::map<::crayon::name, const char*>; \
  const std::map<::crayon::name, const char*>& names() {     \
    static const NameMapping mapping = {values};             \
    return mapping;                                          \
  } /*names()*/                                              \
                                                             \
  const char* c_str(::crayon::name value) {                  \
    static const NameMapping& mapping = names();             \
    const auto& itr = mapping.find(value);                   \
    if (itr == mapping.end()) {                              \
      std::stringstream name;                                \
      name << #name "::[" << (int)value << "]";              \
      return name.str().c_str();                             \
    }                                                        \
    return itr->second;                                      \
  } /*c_str()*/                                              \
                                                             \
  } /*<EnumName>*/                                           \
  } /*enums*/