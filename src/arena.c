#include "arena.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

internal inline size_t align_forward(size_t p, size_t align)
{
	size_t mask = align - 1;
	return (p + mask) & ~mask;
}

arena_t* arena_init(size_t bytes, arena_flags flags)
{
	arena_t* a = (arena_t*)malloc(sizeof(arena_t) + bytes);
	ASSERT(a);

	a->data		= (u8*)a + sizeof(arena_t);
	a->capacity = bytes;
	a->used		= 0;

	if (flags & ARENA_ZEROED_ALLOC)
	{
		memset(a->data, 0, bytes);
	}
	else
	{
		memset(a->data, 0xFF, bytes);
	}

	return a;
}

void arena_release(arena_t* a)
{
	ASSERT(a);
	free(a);
}

void* arena_push(arena_t* a, size_t size, size_t align)
{
	ASSERT(a);

	size_t aligned = align_forward(a->used, align);
	ASSERT(size <= (a->capacity - aligned));

	void* p = (u8*)a->data + aligned;
	a->used = aligned + size;

	return p;
};

void* arena_push_zero(arena_t* a, size_t size, size_t align)
{
	ASSERT(a);
	ASSERT(size > 0);

	size_t aligned = align_forward(a->used, align);
	ASSERT(size <= (a->capacity - aligned));

	void* p = (u8*)a->data + aligned;
	memset(p, 0, size);
	a->used = aligned + size;

	return p;
};

void arena_pop(arena_t* a, size_t size)
{
	ASSERT(a);
	ASSERT(size <= a->used);

	a->used -= size;
};

void arena_clear(arena_t* a)
{
	ASSERT(a);

	arena_pop(a, a->used);
};
