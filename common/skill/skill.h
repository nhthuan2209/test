#ifndef _SKILL_H_
#define _SKILL_H_

#include <stdint.h>

struct Player;

typedef enum
{
	SKILL_NONE = 0,
	SKILL_FREEZE = 1,
	SKILL_COUNTER = 2
} Skill;

#define SKILL_FREEZE_LEVEL			3
#define SKILL_COUNTER_LEVEL			6

uint8_t Skill_Unlocked(struct Player *player, Skill skill);

#endif // _SKILL_H_