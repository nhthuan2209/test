#ifndef _UI_H_
#define _UI_H_

#include "stdint.h"
#include "player.h"
#include "potion.h"

typedef enum {
	UI_PAGE_WAITING = 0,
	UI_PAGE_MENU,
	UI_PAGE_BATTLE,
	UI_PAGE_ENDLESS,
	UI_PAGE_SETTING,
	UI_PAGE_GAME_OVER,
	UI_PAGE_SHOP,
	UI_PAGE_BATTLE_SKILLS,
	UI_PAGE_POTIONS,
	UI_PAGE_BUYING_POTION,
	UI_PAGE_BUYING_STATS
} UiPage;

extern uint8_t uid[4];
extern UiPage  ui_page;
extern int8_t  current_player;
extern uint8_t select_mode;
extern uint8_t select_skill;
extern uint8_t select_shop;
extern uint8_t select_action;
extern uint8_t select_potion;
extern uint8_t active_skill;
extern uint8_t buying_potion;

void UI_HandleWaitingForRfidCard(void);
void UI_DisplayBattleStat(void);
void UI_ConfirmMode(uint8_t mode);
void UI_MoveMode(uint8_t *sl_mode);
void UI_MoveOption(uint8_t *sl_option);
void Shop_DisplayPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, int32_t gold_value, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price);
uint16_t Shop_GetPotionPrice(PotionType potion);
void UI_BattlePage(void);
void UI_GameOverPage(void);
void UI_MoveShopMenu(uint8_t *sl_shop);
void UI_MoveBattleSkill(uint8_t *sl_skill);
void UI_ConfirmShopMenu(uint8_t shop);
void UI_ConfirmBattleSkill(uint8_t skill);
uint8_t UI_BackShopMenu(void);
uint8_t UI_BackBattleSkillMenu(void);
void UI_MovePotionMenu(uint8_t *potion);
void UI_MoveBuyPotion(uint8_t *potion);
void UI_BuyPotion(PotionType potion);
void UI_MoveBuyStats(uint8_t *sl_shop);
void UI_ConfirmBuyStats(uint8_t shop);
void UI_ConfirmPotion(PotionType potion);
uint8_t UI_BackPotionMenu(void);
void UI_ShowSetting(Player *campaign_player, Player *endless_player);
void UI_ResetStat(void);
void UI_BackMenuGame(void);

#endif // _UI_H_