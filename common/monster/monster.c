#include "monster.h"
#include "battle.h"

Monster monster_list[] =
{
    {"Slime", 45, 45, 8, 2, 15, 10},
    {"Skeleton", 60, 60, 12, 3, 20, 15},
    {"Zombie", 80, 80, 15, 5, 30, 20},
    {"Orc", 100, 100, 20, 6, 35, 25},
    {"Troll", 120, 120, 25, 8, 50, 30},
    {"Vampire", 150, 150, 28, 10, 60, 40},
    {"Werewolf", 180, 180, 32, 12, 75, 50},
    {"Demon", 200, 200, 35, 15, 90, 60},
    {"Dragon", 250, 250, 40, 18, 120, 80},
    {"Giant", 300, 300, 45, 20, 150,100},
    {"Hydra", 350, 350, 50, 25, 200, 120},
    {"Minotaur", 400, 400, 55, 30, 250, 150},
    {"Phoenix", 450, 450, 60, 35, 300, 180},
    {"Kraken", 500, 500, 65, 40, 400, 200},
    {"Titan", 600, 600, 70, 45, 500, 250},
    {"Leviathan", 700, 700, 75, 50, 600,300},
    {"Behemoth",800 ,800 ,80 ,55 ,700 ,350 },
    {"Cerberus",900 ,900 ,85 ,60 ,800 ,400 },
    {"Chimera",1000 ,1000 ,90 ,65 ,900 ,450 },
    {"Medusa",1100 ,1100 ,95 ,70 ,1000 ,500 },
    {"Goblin", 70, 70, 13, 4, 25, 15},
    {"Wolf", 95, 95, 18, 7, 40, 25},
    {"Dragon", 180, 180, 30, 12, 80, 50},
    {"Lord", 280, 280, 42, 17, 150, 100},
    {"Bat", 55, 55, 11, 3, 20, 10},
    {"Orc", 125, 125, 23, 9, 55, 30},
    {"Troll", 190, 190, 30, 13, 85, 50},
    {"Demon", 260, 260, 38, 17, 120, 80},
    {"Titan", 360, 360, 50, 22, 200, 120},
    {"Wraith", 230, 230, 45, 14, 130, 80},
    {"Kraken", 430, 430, 58, 24, 260, 150},
    {"Dragon King", 550, 550, 68, 30, 350, 200}
};

uint8_t monster_count = ARRAY_COUNT(monster_list);

static int16_t monster_attack_temp_damage = 0;

int16_t Monster_GetAttackDamage(Monster *monster)
{
	if (monster == 0)
	{
		return 0;
	}

	return monster->attack;
}

void Monster_TakeDamage(Monster *monster, int16_t damage)
{
	if (monster == 0)
	{
		return;
	}

	monster->hp -= damage;
	if (monster->hp < 0)
	{
		monster->hp = 0;
	}
}

void Monster_AttackPlayer(Monster *monster)
{
	int16_t dmg;

	if (monster == 0)
	{
		monster_attack_temp_damage = 0;
		return;
	}

	dmg = Monster_GetAttackDamage(monster);
	monster_attack_temp_damage = dmg;
}