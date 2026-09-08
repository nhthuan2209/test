#ifndef MONSTER_H
#define MONSTER_H

#include "stdint.h"

#ifndef ARRAY_COUNT
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#endif

struct Player;
struct Battle;

typedef struct Monster
{
    char name[20];

    int32_t max_hp;
    int32_t hp;

    int16_t attack;
    int16_t defense;

    int16_t exp_reward;
    int32_t gold_reward;

} Monster;

extern Monster monster_list[];
extern uint8_t monster_count;

int16_t Monster_GetAttackDamage(Monster *monster);
void Monster_TakeDamage(Monster *monster, int16_t damage);
void Monster_AttackPlayer(Monster *monster);

#endif