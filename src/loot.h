#ifndef LOOT_H
#define LOOT_H

typedef enum
{
	LT_UNKNOWN,
	LT_WEAPON
} loot_type_t;

typedef struct
{
	loot_type_t type;
} loot_t;

#endif
