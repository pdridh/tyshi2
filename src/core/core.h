#pragma once

#include <cstdint>

#if DEBUG_BUILD
#define Assert(Expression) \
  if (!(Expression))       \
  {                        \
    *(int *)0 = 0;         \
  }
#define DEBUG_LOG(STR) \
  printf(STR);         \
  printf("\n");
#else
#define Assert(Expression)
#define DEBUG_LOG(STR)
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;
