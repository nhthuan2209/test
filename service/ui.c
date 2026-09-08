#include "ui.h"

#include "stdio.h"

#include "user.h"
#include "definition.h"
#include "button.h"
#include "player.h"
#include "monster.h"
#include "battle.h"
#include "campaign.h"
#include "skill.h"
#include "potion.h"
#include "rfid.h"
#include "lcd.h"

int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_skill = 1;
uint8_t select_shop = 1;
uint8_t active_skill = 0;
uint8_t select_action = 1;
uint8_t select_potion = POTION_HEAL;

#define PLAYER_MODE_COUNT 2

static uint32_t map_endless = 0; 
static uint8_t battle_mode = 0;
static Player campaign_players[PLAYER_MODE_COUNT];
static Player endless_players[PLAYER_MODE_COUNT];
static Player *battle_player = 0;
static Monster *battle_monster = 0;
static uint8_t select_option = 1;
static uint8_t gameover_select_locked = 1;
uint8_t buying_potion = 0;
static uint8_t shop_potion_stock[POTION_COUNT] = {1, 1, 1, 1};

UiPage ui_page = UI_PAGE_WAITING;

uint16_t Shop_GetPotionPrice(PotionType potion)
{
	if (potion == POTION_HEAL)
	{
		return SHOP_POTION_HEAL_PRICE;
	}
	if (potion == POTION_PROTECT)
	{
		return SHOP_POTION_PROTECT_PRICE;
	}
	if (potion == POTION_FIRE)
	{
		return SHOP_POTION_FIRE_PRICE;
	}
	if (potion == POTION_POWER)
	{
		return SHOP_POTION_POWER_PRICE;
	}
	return 0;
}

void Shop_DisplayPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, int32_t gold_value, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price)
{
	LCD_DrawShopPotionMenu(hp_count, prt_count, fire_count, power_count, gold_value, hp_price, prt_price, fire_price, power_price);
}

void UI_HandleWaitingForRfidCard(void)
{
	if (ui_page == UI_PAGE_WAITING)
	{
		uint8_t uid[RFID_UID_LEN];
 
		if (Rfid_TryReadUid(uid, RFID_UID_LEN)) {
			current_player = User_FindUid(uid);
			if (current_player >= 0) {
				campaign_players[current_player] = player_list[current_player];
				endless_players[current_player] = player_list[current_player];
				ui_page = UI_PAGE_MENU;
				RESET_DISPLAY;
				LCD_DrawMenuGame();
			}
		}
	}
}

void UI_MoveAction(uint8_t *sl_action)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectAction(*sl_action);
		if (*sl_action < 4)
		{
			(*sl_action)++;
		}
		LCD_SelectAction(*sl_action);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectAction(*sl_action);
		if (*sl_action > 1)
		{
			(*sl_action)--;
		}
		LCD_SelectAction(*sl_action);
	}
}

#define UI_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)

void UI_PlayerStats(Player *player)
{
	char save[20];
	
	sprintf(save, "%3d", player->level);
	UI_WRITE_STAT(40, 20, save);
	
	sprintf(save, "%3d/%3d", player->hp, player->max_hp);
	UI_WRITE_STAT(35, 130, save);
	LCD_DrawHealthBar(10, 122, 90, player->hp, player->max_hp, GREEN);
	
	sprintf(save, "%3d", player->attack);
	UI_WRITE_STAT(35, 140, save);
	
	sprintf(save, "%5d", player->exp);
	UI_WRITE_STAT(30, 150, save);
}

void UI_MonsterStats(Monster *monster)
{
	char save[20];
	
	sprintf(save, "%3d/%3d", monster->hp, monster->max_hp);
	UI_WRITE_STAT(260, 130, save);
	LCD_DrawHealthBar(220, 122, 90, monster->hp, monster->max_hp, RED);
	
	sprintf(save, "%3d", monster->attack);
	UI_WRITE_STAT(270, 140, save);
	
	sprintf(save, "%5d", monster->exp_reward);
	UI_WRITE_STAT(270, 150, save);
}

void UI_UpdateStats(Player *player, Monster *monster)
{
	UI_PlayerStats(player);
	UI_MonsterStats(monster);
}

static void UI_ShowCampaign(void)
{
	LCD_DrawCampaignMap();
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 0;
	battle_player = &campaign_players[current_player];
	battle_monster = &monster_list[campaign[current_state].monster_id];
	Battle_Start(battle_player, battle_monster);
	UI_DisplayBattleStat();
}

#define UI_WRITE_ANNOUNCEMENT(x, y, text)					ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_ShowEndless(void)
{
	char announe_nextmap[20];
	
	HAL_Delay(800);
	RESET_DISPLAY;
	sprintf(announe_nextmap, "MAP:%d", map_endless + 1);
	UI_WRITE_ANNOUNCEMENT(100, 80, announe_nextmap);
	HAL_Delay(500);
	
	RESET_DISPLAY;
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 1;
	battle_player = &endless_players[current_player];
	battle_monster = &monster_list[map_endless % monster_count];
	Battle_Start(battle_player, battle_monster);
	UI_DisplayBattleStat();
}

void UI_ShowSetting(Player *campaign_player, Player *endless_player)
{
	char save[32];
	
	UI_WRITE_ANNOUNCEMENT(10, 10, "CAMPAIGN");
	UI_WRITE_ANNOUNCEMENT(180, 10, "ENDLESS");

	sprintf(save, "Name: %s", campaign_player->name);
	UI_WRITE_STAT(10, 35, save);
	sprintf(save, "Name: %s", endless_player->name);
	UI_WRITE_STAT(170, 35, save);
	
	sprintf(save, "HP:%d/%d", campaign_player->hp, campaign_player->max_hp);
	UI_WRITE_STAT(10, 55, save);
	sprintf(save, "HP:%d/%d", endless_player->hp, endless_player->max_hp);
	UI_WRITE_STAT(170, 55, save);
	
	sprintf(save, "ATK:%d", campaign_player->attack);
	UI_WRITE_STAT(10, 75, save);
	sprintf(save, "ATK:%d", endless_player->attack);
	UI_WRITE_STAT(170, 75, save);
	
	sprintf(save, "DEF:%d", campaign_player->defense);
	UI_WRITE_STAT(10, 95, save);
	sprintf(save, "DEF:%d", endless_player->defense);
	UI_WRITE_STAT(170, 95, save);
	
	sprintf(save, "EXP:%d", campaign_player->exp);
	UI_WRITE_STAT(10, 115, save);
	sprintf(save, "EXP:%d", endless_player->exp);
	UI_WRITE_STAT(170, 115, save);
	
	sprintf(save, "LV:%d", campaign_player->level);
	UI_WRITE_STAT(10, 135, save);
	sprintf(save, "LV:%d", endless_player->level);
	UI_WRITE_STAT(170, 135, save);
	
	UI_WRITE_STAT(10, 155, "SELECT:RESET");
	UI_WRITE_STAT(170, 155, "BACK:EXIT");
}

static uint8_t setting_confirm = 1;

void UI_ResetStat(void)
{
	if (!HAL_Buttonselect())
	{
		setting_confirm = 1;
		return;
	}
	else
	{
		setting_confirm = 0;
	}
	RESET_DISPLAY;
	if (battle_mode == 1)
	{
		map_endless = 0;
		endless_players[current_player].hp = endless_players[current_player].max_hp;
	}
	else
	{
		Campaign_Reset();
		campaign_players[current_player].hp = campaign_players[current_player].max_hp;
	}
	RESET_DISPLAY;
	UI_WRITE_ANNOUNCEMENT(48, 70, "RESET COMPLETE");
	HAL_Delay(800);

	RESET_DISPLAY;
	UI_ShowSetting(&campaign_players[current_player], &endless_players[current_player]);
}

#define UI_WRITE_RESULT(x, y, text) 							ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_BattleEnd(uint8_t win)
{
	if(win)
	{
		char save[32];
		int32_t gold_reward = battle_monster->gold_reward;
		int16_t exp_reward = battle_monster->exp_reward;
		battle_player = 0;
		battle_monster = 0;

		RESET_DISPLAY;
		UI_WRITE_RESULT(80, 40, "VICTORY!");
		sprintf(save, "GOLD: %d", gold_reward);
		UI_WRITE_RESULT(80, 80, save);
		sprintf(save, "EXP: %d", exp_reward);
		UI_WRITE_RESULT(80, 120, save);
		HAL_Delay(1500);
		if(battle_mode == 1)
		{
			map_endless++;
			UI_ShowEndless();
			return;
		}
		
		if (current_state >= campaign_state_count - 1)
		{
			RESET_DISPLAY;
			UI_WRITE_RESULT(40, 75, "CAMPAIGN CLEAR!");
			HAL_Delay(1500);

			Campaign_Reset();
			RESET_DISPLAY;
			ui_page = UI_PAGE_MENU;
			LCD_DrawMenuGame();
			return;			
		}
		Campaign_NextStage();
		RESET_DISPLAY;
		UI_ShowCampaign();
	}
	else
	{
		battle_player = 0;
		battle_monster = 0;
		select_option = 1;
		ui_page = UI_PAGE_GAME_OVER;
		gameover_select_locked = 1;
		RESET_DISPLAY;
		LCD_DrawEndPage();
		LCD_SelectOption(select_option);
	}
}

void UI_GameOverPage(void)
{
	if (gameover_select_locked)
	{
		if (!HAL_Buttonselect())
		{
			gameover_select_locked = 0;
		}
		return;
	}

	UI_MoveOption(&select_option);

	if (!HAL_Buttonselect())
	{
		return;
	}

	if (select_option == 1)
	{
		RESET_DISPLAY;
		if (battle_mode == 1)
		{
			map_endless = 0;
			endless_players[current_player].hp = endless_players[current_player].max_hp;
			UI_ShowEndless();
		}
		else
		{
			Campaign_Reset();
			campaign_players[current_player].hp = campaign_players[current_player].max_hp;
			UI_ShowCampaign();
		}
	}
	else
	{
		map_endless = 0;
		Campaign_Reset();
		ui_page = UI_PAGE_MENU;
		RESET_DISPLAY;
		LCD_DrawMenuGame();
	}
}

void UI_DisplayBattleStat(void)
{
	ST7789_Fill_Color(BLACK);
	LCD_DrawMonsterAppearance();
	LCD_DrawPlayerAppearance();
	LCD_DrawStats();
	LCD_Action();
	if(battle_player != 0 && battle_monster != 0)
	{
		UI_UpdateStats(battle_player, battle_monster);
	}	
}

static uint8_t select_locked = 0;
void UI_BattlePage(void)
{
	if(battle_player == 0 || battle_monster == 0)
	{
		return;
	}
	if(Battle_GetState() == BATTLE_VICTORY || Battle_GetState() == BATTLE_DEFEAT)
	{
		return;
	}
	LCD_SelectAction(select_action);
	UI_MoveAction(&select_action);
	if(!HAL_Buttonselect())
	{
		select_locked = 0;   
	}
	else if(!select_locked)
	{
		select_locked = 1;
		if (select_action == 1)
		{
			LCD_AttackEffect(1);
			Battle_Attack_Monster();
			UI_UpdateStats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				Player_GainGold(battle_player, battle_monster->gold_reward);
				Player_GainExp(battle_player, battle_monster->exp_reward);
				UI_BattleEnd(1);
				return;
			}
				if (active_skill == 1)
				{
					active_skill = 0;
					Battle_Freeze();
					UI_UpdateStats(battle_player, battle_monster);
					return;
				}
			HAL_Delay(1000);
			LCD_AttackEffect(0);
				if (active_skill == 2)
				{
					Battle_Counter();
					active_skill = 0;
				}
				else
				{
					Battle_Attack_Player();
				}
			UI_UpdateStats(battle_player, battle_monster);
			
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_BattleEnd(0);
				return;
			}
		}
		else if (select_action == 2)
		{
			Battle_Defending();
			UI_UpdateStats(battle_player, battle_monster);
			
			HAL_Delay(400);
			LCD_AttackEffect(0);
			Battle_Attack_Player();
			UI_UpdateStats(battle_player, battle_monster);
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_BattleEnd(0);
				return;
			}
		}
		else if (select_action == 3)
		{
			RESET_DISPLAY;
			ui_page = UI_PAGE_BATTLE_SKILLS;
			select_skill = 1;
			LCD_DrawSkillMenu();
			LCD_SelectBattleSkill(select_skill);
		}
		else if (select_action == 4)
		{
			RESET_DISPLAY;
			ui_page = UI_PAGE_POTIONS;
			buying_potion = 0;
			select_potion = POTION_HEAL;
			LCD_DrawPotionMenu(Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
			LCD_SelectPotion(select_potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
		}
	}
}

void UI_ConfirmMode(uint8_t mode)
{
	if(HAL_Buttonselect())
	{
		switch(mode)
		{
			case 1:
				RESET_DISPLAY;
				UI_ShowCampaign();
				break;
			case 2:
				RESET_DISPLAY;
				LCD_DrawEndlessMap();
				UI_ShowEndless();
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SHOP;
				select_shop = 1;
				LCD_DrawShopMenu(battle_mode == 0 ? campaign_players[current_player].gold : endless_players[current_player].gold);
				LCD_SelectShop(select_shop);
				break;
			case 4:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SETTING;
				UI_ShowSetting(&campaign_players[current_player], &endless_players[current_player]);
				break;
		}
	}
}

void UI_MoveMode(uint8_t *sl_mode)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectMode(*sl_mode);
		if (*sl_mode < 4)
		{
			(*sl_mode)++;
		}
		LCD_SelectMode(*sl_mode);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectMode(*sl_mode);
		if (*sl_mode > 1)
		{
			(*sl_mode)--;
		}
		LCD_SelectMode(*sl_mode);
	}
}

void UI_MoveShopMenu(uint8_t *sl_shop)
{
	if (ui_page != UI_PAGE_SHOP)
	{
		return;
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectShop(*sl_shop);
		if (*sl_shop < 3)
		{
			(*sl_shop)++;
		}
		LCD_SelectShop(*sl_shop);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectShop(*sl_shop);
		if (*sl_shop > 1)
		{
			(*sl_shop)--;
		}
		LCD_SelectShop(*sl_shop);
	}
}


void UI_ConfirmShopMenu(uint8_t shop)
{
	if(HAL_Buttonselect())
	{
		switch(shop)
		{
			case 1:
				RESET_DISPLAY;
				ui_page = UI_PAGE_BUYING_POTION;
				buying_potion = 1;
				select_potion = POTION_HEAL;
				LCD_DrawShopPotionMenu(shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], battle_mode == 0 ? campaign_players[current_player].gold : endless_players[current_player].gold, SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
				LCD_SelectShopPotion(select_potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
				break;
			case 2:
				RESET_DISPLAY;
				ui_page = UI_PAGE_BUYING_STATS;
				select_shop = 1;
				if (battle_mode == 0)
				{
					LCD_DrawShopStats(campaign_players[current_player].max_hp, campaign_players[current_player].attack, campaign_players[current_player].defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
				}
				else
				{
					LCD_DrawShopStats(endless_players[current_player].max_hp, endless_players[current_player].attack, endless_players[current_player].defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
				}
				LCD_SelectShopStats(select_shop, battle_mode == 0 ? campaign_players[current_player].max_hp : endless_players[current_player].max_hp, battle_mode == 0 ? campaign_players[current_player].attack : endless_players[current_player].attack, battle_mode == 0 ? campaign_players[current_player].defense : endless_players[current_player].defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_MENU;
				LCD_DrawMenuGame();
				break;
		}
	}
}

void UI_MoveBuyPotion(uint8_t *potion)
{
	if (ui_page != UI_PAGE_BUYING_POTION)
	{
		return;
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectShopPotion(*potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
		if (*potion < POTION_COUNT - 1)
		{
			(*potion)++;
		}
		LCD_SelectShopPotion(*potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectShopPotion(*potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
		if (*potion > POTION_HEAL)
		{
			(*potion)--;
		}
		LCD_SelectShopPotion(*potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
	}
}

void UI_MoveBuyStats(uint8_t *sl_shop)
{
	Player *shop_player;

	if (ui_page != UI_PAGE_BUYING_STATS)
	{
		return;
	}

	if (battle_mode == 0)
	{
		shop_player = &campaign_players[current_player];
	}
	else
	{
		shop_player = &endless_players[current_player];
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectShopStats(*sl_shop, shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
		if (*sl_shop < 3)
		{
			(*sl_shop)++;
		}
		LCD_SelectShopStats(*sl_shop, shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectShopStats(*sl_shop, shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
		if (*sl_shop > 1)
		{
			(*sl_shop)--;
		}
		LCD_SelectShopStats(*sl_shop, shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
	}
}

void UI_EffectBuyStats(uint8_t shop)
{
	if (ui_page != UI_PAGE_BUYING_STATS)
	{
		return;
	}
	if (!HAL_Buttonselect())
	{
		return;
	}
	if (shop == 1)
	{
		campaign_players[current_player].hp += 10;
		endless_players[current_player].hp += 10;
		LCD_BuyMaxHpEffect();
	}
	else if (shop == 2)
	{
		campaign_players[current_player].attack += 5;
		endless_players[current_player].attack += 5;
		LCD_BuyAttackEffect();
	}
	else if (shop == 3)
	{
		campaign_players[current_player].defense += 5;
		endless_players[current_player].defense += 5;
		LCD_BuyDefenseEffect();
	}
}

void UI_ConfirmBuyStats(uint8_t shop)
{
	Player *shop_player;

	if (battle_mode == 0)
	{
		shop_player = &campaign_players[current_player];
	}
	else
	{
		shop_player = &endless_players[current_player];
	}

	if (!HAL_Buttonselect())
	{
		return;
	}

	switch(shop)
	{
		case 1:
			if (shop_player->gold >= SHOP_MAX_HP_PRICE)
			{
				shop_player->gold -= SHOP_MAX_HP_PRICE;
				shop_player->max_hp += 10;
				shop_player->hp += 10;
			}
			else
			{
				UI_WRITE_ANNOUNCEMENT(80, 75, "OUT OF GOLD");
				HAL_Delay(550);
			}
			break;
		case 2:
			if (shop_player->gold >= SHOP_ATTACK_PRICE)
			{
				shop_player->gold -= SHOP_ATTACK_PRICE;
				shop_player->attack += 5;
			}
			else
			{
				UI_WRITE_ANNOUNCEMENT(80, 75, "OUT OF GOLD");
				HAL_Delay(550);
			}
			break;
		case 3:
			if (shop_player->gold >= SHOP_DEFENSE_PRICE)
			{
				shop_player->gold -= SHOP_DEFENSE_PRICE;
				shop_player->defense += 5;
			}
			else
			{
				UI_WRITE_ANNOUNCEMENT(80, 75, "OUT OF GOLD");
				HAL_Delay(550);
			}
			break;
	}

	RESET_DISPLAY;
	LCD_DrawShopStats(shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
	LCD_SelectShopStats(select_shop, shop_player->max_hp, shop_player->attack, shop_player->defense, SHOP_MAX_HP_PRICE, SHOP_ATTACK_PRICE, SHOP_DEFENSE_PRICE);
}


void UI_BuyPotion(PotionType potion)
{
	Player *shop_player;

	uint16_t potion_price;

	if (!HAL_Buttonselect())
	{
		return;
	}

	if (battle_mode == 0)
	{
		shop_player = &campaign_players[current_player];
	}
	else
	{
		shop_player = &endless_players[current_player];
	}
	potion_price = Shop_GetPotionPrice(potion);

	if (shop_potion_stock[potion] == 0)
	{
		UI_WRITE_ANNOUNCEMENT(50, 75, "SOLD OUT");
		HAL_Delay(550);
		return;
	}

	if (shop_player->gold < potion_price)
	{
		UI_WRITE_ANNOUNCEMENT(50, 75, "NOT ENOUGH GOLD");
		HAL_Delay(550);
		RESET_DISPLAY;
		Shop_DisplayPotionMenu(shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], shop_player->gold, SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
		LCD_SelectShopPotion(select_potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
		return;
	}

	shop_player->gold -= potion_price;
	shop_player->potion_count[potion]++;
	shop_potion_stock[potion] = 0;
	RESET_DISPLAY;
	Shop_DisplayPotionMenu(shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], shop_player->gold, SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
	LCD_SelectShopPotion(select_potion, shop_potion_stock[POTION_HEAL], shop_potion_stock[POTION_PROTECT], shop_potion_stock[POTION_FIRE], shop_potion_stock[POTION_POWER], SHOP_POTION_HEAL_PRICE, SHOP_POTION_PROTECT_PRICE, SHOP_POTION_FIRE_PRICE, SHOP_POTION_POWER_PRICE);
}

void UI_MoveBattleSkill(uint8_t *sl_skill)
{
	if (ui_page != UI_PAGE_BATTLE_SKILLS)
	{
		return;
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectBattleSkill(*sl_skill);
		if (*sl_skill < 2)
		{
			(*sl_skill)++;
		}
		LCD_SelectBattleSkill(*sl_skill);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectBattleSkill(*sl_skill);
		if (*sl_skill > 1)
		{
			(*sl_skill)--;
		}
		LCD_SelectBattleSkill(*sl_skill);
	}
}

void UI_MovePotionMenu(uint8_t *potion)
{
	if (HAL_Buttondown())
	{
		LCD_DeselectPotion(*potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
		if (*potion < POTION_COUNT - 1)
		{
			(*potion)++;
		}
		LCD_SelectPotion(*potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
	}
	if (HAL_Buttonup())
	{
		LCD_DeselectPotion(*potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
		if (*potion > POTION_HEAL)
		{
			(*potion)--;
		}
		LCD_SelectPotion(*potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
	}
}

void UI_ConfirmPotion(PotionType potion)
{
	if (!HAL_Buttonselect())
	{
		return;
	}

	if (!Battle_UsePotion(potion))
	{
		UI_WRITE_ANNOUNCEMENT(80, 75, "OUT OF POTION");
		HAL_Delay(550);
		RESET_DISPLAY;
		LCD_DrawPotionMenu(Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
		LCD_SelectPotion(select_potion, Battle_CountPotion(POTION_HEAL), Battle_CountPotion(POTION_PROTECT), Battle_CountPotion(POTION_FIRE), Battle_CountPotion(POTION_POWER));
		return;
	}

	ui_page = UI_PAGE_BATTLE;
	RESET_DISPLAY;
	UI_DisplayBattleStat();
	LCD_SelectAction(select_action);
	HAL_Delay(400);
	LCD_AttackEffect(0);
	Battle_Attack_Player();
	UI_UpdateStats(battle_player, battle_monster);
	if (Battle_GetState() == BATTLE_DEFEAT)
	{
		UI_BattleEnd(0);
		return;
	}
	if (Battle_GetState() == BATTLE_VICTORY)
	{
		Player_GainExp(battle_player, battle_monster->exp_reward);
		Player_GainGold(battle_player, battle_monster->gold_reward);
		UI_BattleEnd(1);
	}
}

uint8_t UI_BackPotionMenu(void)
{
	if (HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_BATTLE;
		UI_DisplayBattleStat();
		LCD_SelectAction(select_action);
		return 1;
	}
	return 0;
}

void UI_ConfirmBattleSkill(uint8_t skill)
{
	if(HAL_Buttonselect())
	{
		switch(skill)
		{
			case 1:
				if (Skill_Unlocked(battle_player, SKILL_FREEZE))
				{
					active_skill = 1;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(90, 70, "FREEZE READY");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 3");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				break;
			case 2:
				if (Skill_Unlocked(battle_player, SKILL_COUNTER))
				{
					active_skill = 2;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(78, 70, "COUNTER READY");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 6");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				break;
		}
	}
}

uint8_t UI_BackShopMenu(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		if (ui_page == UI_PAGE_BUYING_POTION)
		{
			ui_page = UI_PAGE_SHOP;
			select_shop = 1;
			LCD_DrawShopMenu(battle_mode == 0 ? campaign_players[current_player].gold : endless_players[current_player].gold);
			LCD_SelectShop(select_shop);
		}
		else
		{
			ui_page = UI_PAGE_MENU;
			LCD_DrawMenuGame();
		}
		return 1;
	}
	return 0;
}

uint8_t UI_BackBattleSkillMenu(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_BATTLE;
		UI_DisplayBattleStat();
		LCD_SelectAction(select_action);
		return 1;
	}
	return 0;
}


void UI_MoveOption(uint8_t *sl_option)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectOption(*sl_option);
		if (*sl_option < 2)
		{
			(*sl_option)++;
		}
		LCD_SelectOption(*sl_option);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectOption(*sl_option);
		if (*sl_option > 1)
		{
			(*sl_option)--;
		}
		LCD_SelectOption(*sl_option);
	}
}


void UI_BackMenuGame(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_DrawMenuGame();
	}
}
