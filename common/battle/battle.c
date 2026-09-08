#include <stdlib.h>

#include "player.h"
#include "monster.h"
#include "battle.h"

static Battle current_battle;

void Battle_Start(Player *player, Monster *monster)
{
	current_battle.player = player;
	current_battle.monster = monster;
	current_battle.state = BATTLE_PLAYER_TURN;
	current_battle.player_defending = 0;
	current_battle.player_usedheal = 0;
	current_battle.potion_count[POTION_HEAL] = player->potion_count[POTION_HEAL];
	current_battle.potion_count[POTION_PROTECT] = player->potion_count[POTION_PROTECT];
	current_battle.potion_count[POTION_FIRE] = player->potion_count[POTION_FIRE];
	current_battle.potion_count[POTION_POWER] = player->potion_count[POTION_POWER];
	current_battle.protect_active = 0;
	current_battle.fire_active = 0;
	current_battle.power_active = 0;
	current_battle.temp_monster_damage = 0;
	current_battle.temp_player_damage = 0;
	monster->hp = monster->max_hp;
}

void Battle_Defending()
{
	current_battle.player_defending = 1;
	current_battle.state = BATTLE_MONSTER_TURN;
}

uint8_t Battle_CountPotion(PotionType potion)
{
	return current_battle.potion_count[potion];
}

void Battle_AddPotion(PotionType potion)
{
	if (potion >= POTION_HEAL && potion < POTION_COUNT)
	{
		current_battle.player->potion_count[potion]++;
		current_battle.potion_count[potion]++;
	}
}

uint8_t Battle_UsePotion(PotionType potion)
{
	if (current_battle.player == 0)
	{
		return 0;
	}

	if (potion < POTION_HEAL || potion >= POTION_COUNT ||
		current_battle.potion_count[potion] == 0)
	{
		return 0;
	}

	current_battle.potion_count[potion]--;
	if (Player_UsePotion(current_battle.player, potion))
	{
		if (potion == POTION_PROTECT)
		{
			current_battle.protect_active = 1;
		}
		else if (potion == POTION_FIRE)
		{
			current_battle.fire_active = 1;
		}
		else if (potion == POTION_POWER)
		{
			current_battle.power_active = 1;
		}
		current_battle.state = BATTLE_MONSTER_TURN;
		return 1;
	}

	return 0;
}

void Battle_Attack_Monster()
{
	if (current_battle.player == 0 || current_battle.monster == 0)
	{
		return;
	}

	Player_AttackMonster(current_battle.player);
	current_battle.temp_monster_damage = Player_GetAttackDamage(current_battle.player, current_battle.power_active);
	current_battle.power_active = 0;

	Monster_TakeDamage(current_battle.monster, current_battle.temp_monster_damage);
	if (current_battle.monster->hp <= 0)
	{
		current_battle.monster->hp = 0;
		current_battle.state = BATTLE_VICTORY;
	}
	else
	{
		current_battle.state = BATTLE_MONSTER_TURN;
	}
}

void Battle_Attack_Player()
{
	if (current_battle.player == 0 || current_battle.monster == 0)
	{
		return;
	}

	Monster_AttackPlayer(current_battle.monster);
	current_battle.temp_player_damage = Monster_GetAttackDamage(current_battle.monster);
	current_battle.temp_player_damage = Player_GetDamageAfterDefense(current_battle.player, current_battle.temp_player_damage, current_battle.protect_active, current_battle.player_defending);
	current_battle.protect_active = 0;
	current_battle.player_defending = 0;

	Player_TakeDamage(current_battle.player, current_battle.temp_player_damage);
	if (current_battle.player->hp <= 0)
	{
		current_battle.player->hp = 0;
		current_battle.state = BATTLE_DEFEAT;
	}
	else
	{
		current_battle.state = BATTLE_PLAYER_TURN;
	}
}

void Battle_Freeze(void)
{
	if (current_battle.player == 0)
	{
		return;
	}
	Player_Freeze(current_battle.player);
	current_battle.state = BATTLE_PLAYER_TURN;
}

void Battle_Counter(void)
{
	if (current_battle.player == 0 || current_battle.monster == 0)
	{
		return;
	}
	Player_Counter(current_battle.player);
	current_battle.temp_player_damage = Player_GetDamageAfterDefense(
		current_battle.player,
		Player_GetCounterState(current_battle.player) ? current_battle.monster->attack : 0,
		current_battle.protect_active,
		current_battle.player_defending
	);
	current_battle.protect_active = 0;
	current_battle.player_defending = 0;
	current_battle.state = BATTLE_PLAYER_TURN;
}

void Battle_Update(void)
{
    if(current_battle.state == BATTLE_MONSTER_TURN)
    {
        Battle_Attack_Player();
    }
}

BattleState Battle_GetState(void)
{
    return current_battle.state;
}