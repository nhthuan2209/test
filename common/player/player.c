#include "player.h"
#include <stdlib.h>
#include "battle.h"

Player player_list[] =
{
	{"Warrior", 90, 90, 18, 5, 1, 0, 0, {0, 0, 0, 0}},
	{"Wizard", 60, 60, 32, 2, 1, 0, 0, {0, 0, 0, 0}}
};

Player player_list_default[] =
{
	{"Warrior", 90, 90, 18, 5, 1, 0, 0, {0, 0, 0, 0}},
	{"Wizard", 60, 60, 32, 2, 1, 0, 0, {0, 0, 0, 0}}
};

uint8_t Skill_Unlocked(struct Player *player, Skill skill)
{
	if (player == 0)
	{
		return 0;
	}

	if (skill == SKILL_FREEZE)
	{
		return player->level >= SKILL_FREEZE_LEVEL;
	}
	
	if (skill == SKILL_COUNTER)
	{
		return player->level >= SKILL_COUNTER_LEVEL;
	}
	return 0;
}

uint8_t Player_UnlockedSkill(Player *player, Skill skill)
{
	return Skill_Unlocked(player, skill);
}


uint8_t player_count = ARRAY_COUNT(player_list_default);

static void Player_UpLevel(Player *player)
{
	int16_t new_maxhp_player = player->max_hp + player->max_hp / 3;
	int16_t new_attack_player = player->attack + player->attack / 3;
	int16_t new_defense_player = player->defense + player->defense / 3;
	
	player->max_hp = new_maxhp_player;
	player->attack = new_attack_player;
	player->defense = new_defense_player;
	
	player->hp = player->max_hp;
	player->level++;
}

void Player_GainExp(Player *player, int16_t exp)
{
	player->exp = player->exp + exp;
	while(player->exp >= EXP_UP_LEVEL)
	{
		player->exp = player->exp - EXP_UP_LEVEL;
		Player_UpLevel(player);
	}
}

void Player_GainGold(Player *player, int16_t gold)
{
	player->gold = player->gold + gold;
}

uint8_t Player_UsePotion(Player *player, PotionType potion)
{
	if (player == 0)
	{
		return 0;
	}

	if (potion < POTION_HEAL || potion >= POTION_COUNT ||
		player->potion_count[potion] == 0)
	{
		return 0;
	}

	player->potion_count[potion]--;

	if (potion == POTION_HEAL)
	{
		player->hp = player->max_hp;
		return 1;
	}
	else if (potion == POTION_PROTECT)
	{
		return 1;
	}
	else if (potion == POTION_FIRE)
	{
		return 1;
	}
	else if (potion == POTION_POWER)
	{
		return 1;
	}

	return 0;
}

static int16_t player_attack_temp_damage = 0;
static int16_t player_freeze_temp = 0;
static int16_t player_counter_temp_damage = 0;

int16_t Player_GetAttackDamage(Player *player, uint8_t power_active)
{
	int16_t dmg;

	if (player == 0)
	{
		return 0;
	}

	dmg = player->attack;
	if ((rand() % 100) < BATTLE_CRIT_CHANCE_PERCENT)
	{
		dmg = dmg * BATTLE_CRIT_DAMAGE;
	}
	if (power_active)
	{
		dmg = dmg * 2;
	}

	return dmg;
}

int16_t Player_GetDamageAfterDefense(Player *player, int16_t damage, uint8_t protect_active, uint8_t defending_active)
{
	if (player == 0)
	{
		return 0;
	}

	if (protect_active)
	{
		return 0;
	}
	if (defending_active)
	{
		damage = damage / 2;
	}
	if (damage < 1)
	{
		damage = 1;
	}
	return damage;
}

void Player_TakeDamage(Player *player, int16_t damage)
{
	if (player == 0)
	{
		return;
	}

	player->hp -= damage;
	if (player->hp < 0)
	{
		player->hp = 0;
	}
}

void Player_AttackMonster(Player *player)
{
	if (player == 0)
	{
		player_attack_temp_damage = 0;
		return;
	}

	player_attack_temp_damage = Player_GetAttackDamage(player, 0);
}

uint8_t Player_GetFreezeState(Player *player)
{
	if (player == 0)
	{
		return 0;
	}
	return player_freeze_temp;
}

void Player_Freeze(Player *player)
{
	if (player == 0)
	{
		player_freeze_temp = 0;
		return;
	}

	player_freeze_temp = 1;
}

uint8_t Player_GetCounterState(Player *player)
{
	if (player == 0)
	{
		return 0;
	}
	return player_counter_temp_damage > 0;
}

void Player_Counter(Player *player)
{
	int16_t dmg;

	if (player == 0)
	{
		player_counter_temp_damage = 0;
		return;
	}

	dmg = player->defense;
	if (dmg < 1)
	{
		dmg = 1;
	}

	player_counter_temp_damage = dmg;
}

void Player_Reset(Player *player)
{
    uint8_t i;

    for (i = 0; i < player_count; i++)
    {
        if (player == &player_list_default[i])
        {
            player_list[i] = player_list_default[i];
            return;
        }
    }
}
