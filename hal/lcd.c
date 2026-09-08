#include "lcd.h"
#include "stdio.h"
#include "potion.h"
#include "player.h"


#define LCD_WRITE_MENU(x, y, text) 			ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define LCD_WRITE_OPTION(x, y, text) 		ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define LCD_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define LCD_WRITE_STAT_SELECT(x, y, text)	ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
#define LCD_WRITE_STAT_DESELECT(x, y, text)	ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
#define LCD_WRITE_CIRCLE(x, y, r)			ST7789_DrawCircle(x, y, r, LETTER_MENU_GAME_COLOR);
#define LCD_WRITE_LINE(x1, y1, x2, y2)		ST7789_DrawLine(x1, y1, x2, y2, LETTER_MENU_GAME_COLOR);
#define LCD_WRITE_SELECT(x, y, text)		ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
#define LCD_WRITE_DESELECT(x, y, text)		ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

void LCD_DrawMenuGame(void)
{
	LCD_WRITE_MENU  (88, 8, "RFID GAME");
	LCD_WRITE_OPTION(78, 42, "CAMPAIGN");
	LCD_WRITE_OPTION(88, 72, "ENDLESS");
	LCD_WRITE_OPTION(98, 102, "SHOP");
	LCD_WRITE_OPTION(108, 132, "SETTING");
}

void LCD_DrawSkillMenu(void)
{
	LCD_WRITE_MENU(105, 8, "SKILLS");
	LCD_WRITE_OPTION(98, 60, "FREEZE");
	LCD_WRITE_OPTION(98, 100, "COUNTER");
	LCD_WRITE_OPTION(105, 126, "BACK");
}

void LCD_DrawShopMenu(int32_t gold)
{
	char save[24];

	LCD_WRITE_MENU(105, 8, "SHOP");
	sprintf(save, "GOLD: %ld", (long)gold);
	LCD_WRITE_OPTION(10, 25, save);
	LCD_WRITE_OPTION(98, 60, "BUY POTION");
	LCD_WRITE_OPTION(98, 100, "BUY STATS");
	LCD_WRITE_OPTION(110, 140, "BACK");
}

void LCD_SelectShop(uint8_t select_shop)
{
	switch (select_shop)
	{
		case 1:
			LCD_WRITE_SELECT(98, 60, "BUY POTION");
			break;
		case 2:
			LCD_WRITE_SELECT(98, 100, "BUY STATS");
			break;
		case 3:
			LCD_WRITE_SELECT(110, 140, "BACK");
			break;
	}
}

void LCD_DeselectShop(uint8_t deselect_shop)
{
	switch (deselect_shop)
	{
		case 1:
			LCD_WRITE_DESELECT(98, 60, "BUY POTION");
			break;
		case 2:
			LCD_WRITE_DESELECT(98, 100, "BUY STATS");
			break;
		case 3:
			LCD_WRITE_DESELECT(110, 140, "BACK");
			break;
	}
}

void LCD_BuyMaxHpEffect()
{
	LCD_WRITE_MENU(65, 70, "MAX HP + 10");
}

void LCD_BuyAttackEffect()
{
	LCD_WRITE_MENU(65, 70, "ATTACK + 5");
}

void LCD_BuyDefenseEffect()
{
	LCD_WRITE_MENU(65, 70, "DEFENSE + 5");
}

void LCD_DrawShopStats(uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price)
{
	char save[32];
	char price[16];
	LCD_WRITE_MENU(105, 8, "SHOP");
	sprintf(save, "MAX HP: %d", max_hp);
	sprintf(price, "PRICE: %d", max_hp_price);
	LCD_WRITE_STAT(10, 40, save);
	LCD_WRITE_STAT(200, 40, price);

	sprintf(save, "ATTACK: %d", attack);
	sprintf(price, "PRICE: %d", attack_price);
	LCD_WRITE_STAT(10, 80, save);
	LCD_WRITE_STAT(200, 80, price);

	sprintf(save, "DEFENSE: %d", defense);
	sprintf(price, "PRICE: %d", defense_price);
	LCD_WRITE_STAT(10, 100, save);
	LCD_WRITE_STAT(200, 100, price);
}

void LCD_SelectShopStats(uint8_t select_shop, uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price)
{
	char save[32];
	char price[16];
	switch (select_shop)
	{
		case 1:
			sprintf(save, "MAX HP: %d", max_hp);
			sprintf(price, "PRICE: %d", max_hp_price);
			LCD_WRITE_STAT_SELECT(10, 40, save);
			LCD_WRITE_STAT_SELECT(200, 40, price);
			break;
		case 2:
			sprintf(save, "ATTACK: %d", attack);
			sprintf(price, "PRICE: %d", attack_price);
			LCD_WRITE_STAT_SELECT(10, 80, save);
			LCD_WRITE_STAT_SELECT(200, 80, price);
			break;
		case 3:
			sprintf(save, "DEFENSE: %d", defense);
			sprintf(price, "PRICE: %d", defense_price);
			LCD_WRITE_STAT_SELECT(10, 100, save);
			LCD_WRITE_STAT_SELECT(200, 100, price);
			break;
	}
}

void LCD_DeselectShopStats(uint8_t deselect_shop, uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price)
{
	char save[32];
	char price[16];
	switch (deselect_shop)
	{
		case 1:
			sprintf(save, "MAX HP: %d", max_hp);
			sprintf(price, "PRICE: %d", max_hp_price);
			LCD_WRITE_STAT_DESELECT(10, 40, save);
			LCD_WRITE_STAT_DESELECT(200, 40, price);
			break;
		case 2:
			sprintf(save, "ATTACK: %d", attack);
			sprintf(price, "PRICE: %d", attack_price);
			LCD_WRITE_STAT_DESELECT(10, 80, save);
			LCD_WRITE_STAT_DESELECT(200, 80, price);
			break;
		case 3:
			sprintf(save, "DEFENSE: %d", defense);
			sprintf(price, "PRICE: %d", defense_price);
			LCD_WRITE_STAT_DESELECT(10, 100, save);
			LCD_WRITE_STAT_DESELECT(200, 100, price);
			break;
	}
}

void LCD_DrawShopPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, int32_t gold_value, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price)
{
	char save [32];
	char price[16];
	LCD_WRITE_MENU(105, 8, "SHOP");
	sprintf(save, "GOLD: %ld", (long)gold_value);
	LCD_WRITE_OPTION(10, 120, save);
	sprintf(save, "HEAL POTION: %d", hp_count);
	sprintf(price, "PRICE: %d", hp_price);
	LCD_WRITE_OPTION(10, 40, save);
	LCD_WRITE_OPTION(200, 40, price);

	sprintf(save, "PROTECT POTION: %d", prt_count);
	sprintf(price, "PRICE: %d", prt_price);
	LCD_WRITE_OPTION(10, 60, save);
	LCD_WRITE_OPTION(200, 60, price);

	sprintf(save, "FIRE POTION: %d", fire_count);
	sprintf(price, "PRICE: %d", fire_price);	
	LCD_WRITE_OPTION(10, 80, save);
	LCD_WRITE_OPTION(200, 80, price);

	sprintf(save, "POWER POTION: %d", power_count);
	sprintf(price, "PRICE: %d", power_price);
	LCD_WRITE_OPTION(10, 100, save);
	LCD_WRITE_OPTION(200, 100, price);
}


void LCD_SelectShopPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price)
{
	char save[32];
	char price[16];

	switch (potion)
	{
		case POTION_HEAL:
			sprintf(save, "HEAL POTION: %d", hp_count);
			sprintf(price, "PRICE: %d", hp_price);
			LCD_WRITE_SELECT(10, 40, save);
			LCD_WRITE_SELECT(200, 40, price);
			break;
		case POTION_PROTECT:
			sprintf(save, "PROTECT POTION: %d", prt_count);
			sprintf(price, "PRICE: %d", prt_price);
			LCD_WRITE_SELECT(10, 60, save);
			LCD_WRITE_SELECT(200, 60, price);
			break;
		case POTION_FIRE:
			sprintf(save, "FIRE POTION: %d", fire_count);
			sprintf(price, "PRICE: %d", fire_price);
			LCD_WRITE_SELECT(10, 80, save);
			LCD_WRITE_SELECT(200, 80, price);
			break;
		case POTION_POWER:
			sprintf(save, "POWER POTION: %d", power_count);
			sprintf(price, "PRICE: %d", power_price);
			LCD_WRITE_SELECT(10, 100, save);
			LCD_WRITE_SELECT(200, 100, price);
			break;
	}
}

void LCD_DeselectShopPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price)
{
	char save[32];
	char price[16];

	switch (potion)
	{
		case POTION_HEAL:
			sprintf(save, "HEAL POTION: %d", hp_count);
			sprintf(price, "PRICE: %d", hp_price);
			LCD_WRITE_DESELECT(10, 40, save);
			LCD_WRITE_DESELECT(200, 40, price);
			break;
		case POTION_PROTECT:
			sprintf(save, "PROTECT POTION: %d", prt_count);
			sprintf(price, "PRICE: %d", prt_price);
			LCD_WRITE_DESELECT(10, 60, save);
			LCD_WRITE_DESELECT(200, 60, price);
			break;
		case POTION_FIRE:
			sprintf(save, "FIRE POTION: %d", fire_count);
			sprintf(price, "PRICE: %d", fire_price);
			LCD_WRITE_DESELECT(10, 80, save);
			LCD_WRITE_DESELECT(200, 80, price);
			break;
		case POTION_POWER:
			sprintf(save, "POWER POTION: %d", power_count);
			sprintf(price, "PRICE: %d", power_price);
			LCD_WRITE_DESELECT(10, 100, save);
			LCD_WRITE_DESELECT(200, 100, price);
			break;
	}
}

void LCD_SelectBattleSkill(uint8_t select_skill)
{
	if (select_skill == 1)
	{
		LCD_WRITE_SELECT(98, 60, "FREEZE");
	}
	else if (select_skill == 2)
	{
		LCD_WRITE_SELECT(98, 100, "COUNTER");
	}
}

void LCD_DeselectBattleSkill(uint8_t deselect_skill)
{
	if (deselect_skill == 1)
	{
		LCD_WRITE_DESELECT(98, 60, "FREEZE");
	}
	else if (deselect_skill == 2)
	{
		LCD_WRITE_DESELECT(98, 100, "COUNTER");
	}
}

void LCD_SelectSkill(uint8_t select_skill)
{
	switch(select_skill)
	{
		case 1:
			LCD_WRITE_SELECT(98, 60, "FREEZE");
			break;
		case 2:
			LCD_WRITE_SELECT(98, 100, "COUNTER");
			break;
		case 3:
			LCD_WRITE_SELECT(105, 126, "BACK");
			break;
	}
}

void LCD_DeselectSkill(uint8_t deselect_skill)
{
	switch(deselect_skill)
	{
		case 1:
			LCD_WRITE_DESELECT(98, 60, "FREEZE");
			break;
		case 2:
			LCD_WRITE_DESELECT(98, 100, "COUNTER");
			break;
		case 3:
			LCD_WRITE_DESELECT(105, 126, "BACK");
			break;
	}
}

void LCD_DrawWaitingDisplay(void)
{
	LCD_WRITE_MENU(85, 20, "RFID GAME");
	LCD_WRITE_MENU(50, 60, " PLEASE SCAN");
	LCD_WRITE_MENU(50, 100, "  YOUR CARD");
	LCD_WRITE_MENU(110, 140, "RFID");
}

void LCD_Action(void)
{
	LCD_WRITE_OPTION(100, 25, "[ATTACK]");
	LCD_WRITE_OPTION(100, 50, "[DEFENSE]");
	LCD_WRITE_OPTION(100, 75, "[SKILL]");
	LCD_WRITE_OPTION(100, 100, "[POTION]");
}

void LCD_DrawMonsterAppearance(void)
{
	LCD_WRITE_CIRCLE(250, 50, 10);
	LCD_WRITE_LINE(250, 60, 250, 90);
	LCD_WRITE_LINE(250, 90, 230, 120);
	LCD_WRITE_LINE(250, 90, 270, 120);
	LCD_WRITE_LINE(250, 75, 230, 75);
	LCD_WRITE_LINE(250, 75, 270, 75);
}

void LCD_DrawPlayerAppearance(void)
{
	LCD_WRITE_CIRCLE(30, 50, 10);
	LCD_WRITE_LINE(30, 60, 30, 90);
	LCD_WRITE_LINE(30, 90, 10, 120);
	LCD_WRITE_LINE(30, 90, 50, 120);
	LCD_WRITE_LINE(30, 75, 10, 75);
	LCD_WRITE_LINE(30, 75, 50, 75);
}

static void LCD_RedrawCombat(uint8_t player)
{
	if (player)
	{
		ST7789_Fill(0, 30, 65, 119, BLACK);
		LCD_DrawPlayerAppearance();
	}
	else
	{
		ST7789_Fill(215, 30, 305, 119, BLACK);
		LCD_DrawMonsterAppearance();
	}
}

static void LCD_DrawHit(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
	ST7789_DrawCircle(x, y, radius, color);
}

void LCD_AttackEffect(uint8_t attacker_is_player)
{
	uint16_t target_x;
	uint16_t color;
	uint8_t frame;

	if (attacker_is_player)
	{
		target_x = 250;
		color = YELLOW;
	}
	else
	{
		target_x = 30;
		color = RED;
	}

	for (frame = 0; frame < 3; frame++)
	{
		uint16_t radius = 10 + frame * 7;
		LCD_DrawHit(target_x, 75, radius, color);
		HAL_Delay(70);
	}

	LCD_RedrawCombat(!attacker_is_player);
}

void LCD_DrawStats(void)
{
	LCD_WRITE_LINE(10, 120, 320, 120);
	LCD_WRITE_STAT(10, 130, "HP: ");
	LCD_WRITE_STAT(10, 140, "ATK: ");
	LCD_WRITE_STAT(10, 20, "LEVEL: ");
	LCD_WRITE_STAT(10, 150, "EXP: ");
	
	
	LCD_WRITE_STAT(250, 130, "HP: ");
	LCD_WRITE_STAT(250, 140, "ATK: ");
	LCD_WRITE_STAT(250, 150, "EXP: ");
}

void LCD_DrawHealthBar(uint16_t x, uint16_t y, uint16_t width, int32_t hp, int32_t max_hp, uint16_t color)
{
	uint16_t health_bar = 0;

	if (max_hp > 0)
	{
		if (hp < 0)
		{
			hp = 0;
		}
		else if (hp > max_hp)
		{
			hp = max_hp;
		}

		health_bar = (width * hp) / max_hp;
	}

	ST7789_DrawFilledRectangle(x, y, width, 6, BLACK);
	ST7789_DrawRectangle(x, y, x + width, y + 6, WHITE);
	if (health_bar > 0)
	{
		ST7789_DrawFilledRectangle(x + 1, y + 1, health_bar - 1, 4, color);
	}
}

void LCD_DrawEndlessMap(void)
{
	LCD_WRITE_MENU(10, 10, "ENDLESS");
	LCD_WRITE_OPTION(28, 42, "START");
	LCD_WRITE_OPTION(105, 42, "NEXT");
	LCD_WRITE_OPTION(182, 42, "NEXT");
	LCD_WRITE_OPTION(259, 42, "NEXT");

	LCD_WRITE_CIRCLE(40, 75, 10);
	LCD_WRITE_CIRCLE(120, 75, 10);
	LCD_WRITE_CIRCLE(200, 75, 10);
	LCD_WRITE_CIRCLE(280, 75, 10);

	LCD_WRITE_LINE(50, 75, 110, 75);
	LCD_WRITE_LINE(130, 75, 190, 75);
	LCD_WRITE_LINE(210, 75, 270, 75);

	LCD_WRITE_LINE(280, 85, 280, 115);
	LCD_WRITE_LINE(280, 115, 40, 115);
	LCD_WRITE_LINE(40, 115, 40, 85);
	LCD_WRITE_OPTION(115, 125, "LOOP FOREVER");
}

void LCD_SelectMode(uint8_t select_mode)
{
	switch(select_mode)
	{
		case 1:
			LCD_WRITE_SELECT(78, 42, "CAMPAIGN");
			break;
		case 2:
			LCD_WRITE_SELECT(88, 72, "ENDLESS");
			break;
		case 3:
			LCD_WRITE_SELECT(98, 102, "SHOP");
			break;
		case 4:
			LCD_WRITE_SELECT(108, 132, "SETTING");
			break;
	}
}

void LCD_DeselectMode(uint8_t deselect_mode)
{
	switch(deselect_mode)
	{
		case 1:
			LCD_WRITE_DESELECT(78, 42, "CAMPAIGN");
			break;
		case 2:
			LCD_WRITE_DESELECT(88, 72, "ENDLESS");
			break;
		case 3:
			LCD_WRITE_DESELECT(98, 102, "SHOP");
			break;
		case 4:
			LCD_WRITE_DESELECT(108, 132, "SETTING");
			break;
	}
}

void LCD_SelectAction(uint8_t select_action)
{
	switch(select_action)
	{
		case 1:
			LCD_WRITE_SELECT(100, 25, "[ATTACK]");
			break;
		case 2:
			LCD_WRITE_SELECT(100, 50, "[DEFENSE]");
			break;
		case 3:
			LCD_WRITE_SELECT(100, 75, "[SKILL]");
			break;
		case 4:
			LCD_WRITE_SELECT(100, 100, "[POTION]");
			break;
	}	
}

void LCD_DeselectAction(uint8_t deselect_action)
{
	switch(deselect_action)
	{
		case 1:
			LCD_WRITE_DESELECT(100, 25, "[ATTACK]");
			break;
		case 2:
			LCD_WRITE_DESELECT(100, 50, "[DEFENSE]");
			break;
		case 3:
			LCD_WRITE_DESELECT(100, 75, "[SKILL]");
			break;
		case 4:
			LCD_WRITE_DESELECT(100, 100, "[POTION]");
			break;
	}	
}

void LCD_DrawPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count)
{
	char item[24];

	LCD_WRITE_MENU(92, 8, "POTIONS");
	sprintf(item, "  HP Potion     x%d", hp_count);
	LCD_WRITE_OPTION(45, 42, item);
	sprintf(item, "  Protect Potionx%d", prt_count);
	LCD_WRITE_OPTION(45, 72, item);
	sprintf(item, "  Fire Potion   x%d", fire_count);
	LCD_WRITE_OPTION(45, 102, item);
	sprintf(item, "  Power Potion  x%d", power_count);
	LCD_WRITE_OPTION(45, 132, item);
}

void LCD_SelectPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count)
{
	switch (potion)
	{
		char item[24];
		case POTION_HEAL:
			sprintf(item, "> HP Potion     x%d", hp_count);
			LCD_WRITE_SELECT(45, 42, item);
			break;
		case POTION_PROTECT:
			sprintf(item, "> Protect Potionx%d", prt_count);
			LCD_WRITE_SELECT(45, 72, item);
			break;
		case POTION_FIRE:
			sprintf(item, "> Fire Potion   x%d", fire_count);
			LCD_WRITE_SELECT(45, 102, item);
			break;
		case POTION_POWER:
			sprintf(item, "> Power Potion  x%d", power_count);
			LCD_WRITE_SELECT(45, 132, item);
			break;
	}
}

void LCD_DeselectPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count)
{
	switch (potion)
	{
		char item[24];
		case POTION_HEAL:
			sprintf(item, "  HP Potion     x%d", hp_count);
			LCD_WRITE_DESELECT(45, 42, item);
			break;
		case POTION_PROTECT:
			sprintf(item, "  Protect Potionx%d", prt_count);
			LCD_WRITE_DESELECT(45, 72, item);
			break;
		case POTION_FIRE:
			sprintf(item, "  Fire Potion   x%d", fire_count);
			LCD_WRITE_DESELECT(45, 102, item);
			break;
		case POTION_POWER:
			sprintf(item, "  Power Potion  x%d", power_count);
			LCD_WRITE_DESELECT(45, 132, item);
			break;
	}
}

void LCD_DrawEndPage(void)
{
	LCD_WRITE_MENU(88, 8, "GAME OVER");
	LCD_WRITE_OPTION(104, 43, "YOU DIED");
	LCD_WRITE_OPTION(99, 68, "BETTER LUCK");
	LCD_WRITE_OPTION(110, 91, "NEXT TIME");
	LCD_WRITE_OPTION(70, 132, "YES");
	LCD_WRITE_OPTION(210, 132, "NO");
}


void LCD_SelectOption(uint8_t select_option)
{
	switch(select_option)
	{
		case 1:
			LCD_WRITE_SELECT(70, 132, "YES");
			break;
		case 2:
			LCD_WRITE_SELECT(210, 132, "NO");
			break;
	}	
}

void LCD_DeselectOption(uint8_t deselect_option)
{
	switch(deselect_option)
	{
		case 1:
			LCD_WRITE_DESELECT(70, 132, "YES");
			break;
		case 2:
			LCD_WRITE_DESELECT(210, 132, "NO");
			break;
	}	
}

void LCD_DrawCampaignMap(void)
{
	LCD_WRITE_MENU(10, 10, "CAMPAIGN");
	LCD_WRITE_LINE(40, 70, 40, 120);
	LCD_WRITE_OPTION(10, 120, "Forest");
	LCD_WRITE_LINE(50, 60, 90, 60);
	LCD_WRITE_CIRCLE(40, 60, 10);
	
	LCD_WRITE_LINE(110, 60, 150, 60);
	LCD_WRITE_CIRCLE(100, 60, 10);
	
	LCD_WRITE_LINE(160, 70, 160, 110);
	LCD_WRITE_CIRCLE(160, 60, 10);
	
	LCD_WRITE_LINE(170, 120, 210, 120);
	LCD_WRITE_CIRCLE(160, 120, 10);
	
	LCD_WRITE_LINE(220, 70, 220, 110);
	LCD_WRITE_CIRCLE(220, 120, 10);

	LCD_WRITE_OPTION(200, 65, "Boss");
	
}
