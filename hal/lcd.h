#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"
#include "st7789.h"
		
#define FONT_MENU_GAME							Font_16x26
#define LETTER_MENU_GAME_COLOR					YELLOW
#define LETTER_MODE_GAME_COLOR					RED
#define BACKGROUND_MENU_GAME_COLOR			    BLACK
#define FONT_OPTION_FUNCTION					Font_11x18
#define FONT_STATS								Font_7x10

void LCD_DrawMenuGame(void);
void LCD_DrawSkillMenu(void);
void LCD_DrawShopMenu(int32_t gold);
void LCD_SelectShop(uint8_t select_shop);
void LCD_DeselectShop(uint8_t deselect_shop);
void LCD_SelectBattleSkill(uint8_t select_skill);
void LCD_DeselectBattleSkill(uint8_t deselect_skill);
void LCD_SelectSkill(uint8_t select_skill);
void LCD_DeselectSkill(uint8_t deselect_skill);
void LCD_DrawWaitingDisplay(void);
void LCD_Action(void);
void LCD_DrawMonsterAppearance(void);
void LCD_DrawPlayerAppearance(void);
void LCD_AttackEffect(uint8_t player_attacks);
void LCD_DrawStats(void);
void LCD_DrawHealthBar(uint16_t x, uint16_t y, uint16_t width, int32_t hp, int32_t max_hp, uint16_t color);
void LCD_DrawEndlessMap(void);
void LCD_SelectMode(uint8_t select_mode);
void LCD_DeselectMode(uint8_t deselect_mode);
void LCD_SelectAction(uint8_t select_action);
void LCD_DeselectAction(uint8_t deselect_action);
void LCD_DrawPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count);
void LCD_SelectPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count);
void LCD_DeselectPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count);
void LCD_SelectShopPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price);
void LCD_DeselectShopPotion(uint8_t potion, uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price);
void LCD_BuyMaxHpEffect();
void LCD_BuyAttackEffect();
void LCD_BuyDefenseEffect();
void LCD_DrawShopStats(uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price);
void LCD_SelectShopStats(uint8_t select_shop, uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price);
void LCD_DeselectShopStats(uint8_t deselect_shop, uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t max_hp_price, uint16_t attack_price, uint16_t defense_price);
void LCD_DrawShopPotionMenu(uint8_t hp_count, uint8_t prt_count, uint8_t fire_count, uint8_t power_count, int32_t gold, uint16_t hp_price, uint16_t prt_price, uint16_t fire_price, uint16_t power_price);
void LCD_DrawEndPage(void);
void LCD_SelectOption(uint8_t select_option);
void LCD_DeselectOption(uint8_t deselect_option);
void LCD_DrawCampaignMap(void);


#endif // _LCD_H_