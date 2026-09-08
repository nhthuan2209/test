#ifndef _BATTLE_H_
#define _BATTLE_H_

#include <stdint.h>
#include "potion.h"

struct Player;
struct Monster;

typedef enum
{
	BATTLE_PLAYER_TURN,
	BATTLE_MONSTER_TURN,
	BATTLE_VICTORY,
	BATTLE_DEFEAT
} BattleState;

typedef struct Battle
{
	struct Player *player;
	struct Monster *monster;

	BattleState state;
	uint16_t player_defending;
	uint16_t player_usedheal;
	uint8_t potion_count[POTION_COUNT];
	uint8_t protect_active;
	uint8_t fire_active;
	uint8_t power_active;
	int16_t temp_monster_damage;
	int16_t temp_player_damage;
} Battle;

#define BATTLE_MAX_POTION_HEAL 		5
#define BATTLE_CRIT_CHANCE_PERCENT 	20
#define BATTLE_CRIT_DAMAGE		 	2

void Battle_Start(struct Player *player, struct Monster *monster);
void Battle_Defending(void);
void Battle_Attack_Monster(void);
void Battle_Attack_Player(void);
void Battle_Freeze(void);
void Battle_Counter(void);
void Battle_Update(void);
void Battle_Heal(void);
uint8_t Battle_Count_HealPotion(void);
uint8_t Battle_UsePotion(PotionType potion);
uint8_t Battle_CountPotion(PotionType potion);
void Battle_AddPotion(PotionType potion);
BattleState Battle_GetState(void);




#endif // _BATTLE_H_