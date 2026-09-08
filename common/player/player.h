#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdint.h>
#include "potion.h"
#include "skill.h"

#define EXP_UP_LEVEL					40
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#endif

typedef struct Player
{
	char name[20];
	
	int32_t max_hp;
	int32_t hp;
	
	int32_t attack;
	int32_t defense;
	
	int32_t level;
	int32_t exp;

	int32_t gold;
	uint8_t potion_count[POTION_COUNT];
	
} Player;

struct Monster;
struct Battle;

extern Player player_list[];
extern uint8_t player_count;

uint8_t Skill_Unlocked(struct Player *player, Skill skill);
uint8_t Player_UsePotion(Player *player, PotionType potion);
int16_t Player_GetAttackDamage(Player *player, uint8_t power_active);
int16_t Player_GetDamageAfterDefense(Player *player, int16_t damage, uint8_t protect_active, uint8_t defending_active);
void Player_TakeDamage(Player *player, int16_t damage);
void Player_AttackMonster(Player *player);
uint8_t Player_GetFreezeState(Player *player);
void Player_Freeze(Player *player);
uint8_t Player_GetCounterState(Player *player);
void Player_Counter(Player *player);
void Player_GainGold(Player *player, int16_t gold);
void Player_GainExp(Player *player, int16_t exp);
void Player_Reset(Player *player);
#endif // _PLAYER_H_