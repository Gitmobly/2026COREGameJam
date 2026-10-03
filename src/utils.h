#ifndef UTILS_H
#define UTILS_H

/* -------------------------------------
 * (Gizmobly): Simple Aliases & Macros
 */

#include <stdint.h>

#define ARRAYCOUNT(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BYTES(size)		((size_t)(size))
#define KILOBYTES(size) ((size_t)(size) * 1024ULL)
#define MEGABYTES(size) ((size_t)(size) * 1024ULL * 1024ULL)
#define GIGABYTES(size) ((size_t)(size) * 1024ULL * 1024ULL * 1024ULL)

#ifdef DEBUG_BUILD
#define ASSERT(expr)                                                                                                   \
	if (!(expr))                                                                                                       \
	{                                                                                                                  \
		*(volatile int*)0 = 0;                                                                                         \
	}
#else
#define ASSERT(expr) {};
#endif

#define global_variable static
#define internal		static
#define local_persist	static

typedef uint8_t	 u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t	s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint32_t b32;

typedef float  f32;
typedef double f64;

typedef struct dims_t
{
	f32 width, height;
} dims_t;

#endif
