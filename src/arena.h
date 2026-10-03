#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

/* -------------------------------------
 * (JJB): Memory
 */

#if defined(__clang__) || defined(__GNUC__)
#define ALIGNOF(type) __alignof__(type)
#elif defined(_MSC_VER)
#define ALIGNOF(type) __alignof(type)
#else
#error "Need ALIGNOF implementation for this compiler"
#endif

/* -------------------------------------
 * (JJB): Arena Allocator
 */

#define ARENA_DEFAULT_ALIGNMENT 16

typedef struct arena_t
{
	void*  data;
	size_t capacity;
	size_t used;
} arena_t;

typedef struct allocators_t
{
	arena_t* permanent;
	arena_t* level;
	arena_t* scratch;
} allocators_t;

typedef enum arena_flags
{
	ARENA_NO_FLAGS	   = 1 << 0,
	ARENA_ZEROED_ALLOC = 1 << 1,
} arena_flags;

arena_t* arena_init(size_t bytes, arena_flags flags);
void	 arena_release(arena_t* a);

#define arena_push_array(a, type, count) (type*)arena_push((a), sizeof(type) * (count), ARENA_DEFAULT_ALIGNMENT)
#define arena_push_array_zero(a, type, count)                                                                          \
	(type*)arena_push_zero((a), sizeof(type) * (count), ARENA_DEFAULT_ALIGNMENT)
#define arena_push_struct(a, type)				 arena_push_array((a), type, 1)
#define arena_push_struct_zero(a, type)			 arena_push_array_zero((a), type, 1)
#define arena_push_struct_zero_count(a, type, c) arena_push_array_zero((a), type, c)

void* arena_push(arena_t* a, size_t size, size_t align);
void* arena_push_zero(arena_t* a, size_t size, size_t align);
void  arena_pop(arena_t* a, size_t size);
void  arena_clear(arena_t* a);

#endif
