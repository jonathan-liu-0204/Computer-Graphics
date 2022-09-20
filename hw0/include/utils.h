#pragma once
#include <stdexcept>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef DELETE_COPY
#define DELETE_COPY(ClassName)           \
  ClassName(const ClassName &) = delete; \
  ClassName &operator=(const ClassName &) = delete;
#endif

#ifndef DEFAULT_COPY
#define DEFAULT_COPY(ClassName)           \
  ClassName(const ClassName &) = default; \
  ClassName &operator=(const ClassName &) = default;
#endif

#ifndef DELETE_MOVE
#define DELETE_MOVE(ClassName)      \
  ClassName(ClassName &&) = delete; \
  ClassName &operator=(ClassName &&) = delete;
#endif

#ifndef DEFAULT_MOVE
#define DEFAULT_MOVE(ClassName)      \
  ClassName(ClassName &&) = default; \
  ClassName &operator=(ClassName &&) = default;
#endif

#ifndef MOVE_ONLY
#define MOVE_ONLY(ClassName) DELETE_COPY(ClassName) DEFAULT_MOVE(ClassName)
#endif

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ExceptionType, message)                                                                      \
  do {                                                                                                               \
    throw ExceptionType(std::string("[") + __FILE__ + ":" + std::to_string(__LINE__) + "] " + std::string(message)); \
  } while (false)
#endif
