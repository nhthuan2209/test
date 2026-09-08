#include "button.h"
#include "button_config.h"

#define BUTTON_COUNT                                        4
#define BUTTON_DEBOUNCE_MS                                  20
typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIO_PinState last_raw_state;
	GPIO_PinState stable_state;
	uint32_t last_change_tick;
} ButtonInfo;

static ButtonInfo button_info[BUTTON_COUNT] = {
	{.port=BUTTON_UP_PORT, 		 .pin=BUTTON_UP_PIN, 		.last_raw_state=GPIO_PIN_SET, .stable_state=GPIO_PIN_SET},
	{.port=BUTTON_DOWN_PORT, 	 .pin=BUTTON_DOWN_PIN, 		.last_raw_state=GPIO_PIN_SET, .stable_state=GPIO_PIN_SET},
	{.port=BUTTON_SELECT_PORT,   .pin=BUTTON_SELECT_PIN, 	.last_raw_state=GPIO_PIN_SET, .stable_state=GPIO_PIN_SET},
	{.port=BUTTON_BACK_PORT, 	 .pin=BUTTON_BACK_PIN, 		.last_raw_state=GPIO_PIN_SET, .stable_state=GPIO_PIN_SET},
};

static ButtonState button_state;

void HAL_ScanButtons(void)
{
	uint32_t now = HAL_GetTick();

	for (size_t i = 0; i < BUTTON_COUNT; i++) 
	{
		GPIO_PinState raw_state = HAL_GPIO_ReadPin(button_info[i].port, button_info[i].pin);

		if (raw_state != button_info[i].last_raw_state) {
			button_info[i].last_raw_state = raw_state;
			button_info[i].last_change_tick = now;
		}
		else if ((uint32_t)(now - button_info[i].last_change_tick) >= BUTTON_DEBOUNCE_MS && raw_state != button_info[i].stable_state) 
		{
			button_info[i].stable_state = raw_state;
			if (raw_state == GPIO_PIN_RESET) 
			{
				switch(i)
				{
					case 0:
						button_state.up = true;
						break;
					case 1:
						button_state.down = true;
						break;
					case 2:
						button_state.select = true;
						break;
					case 3:
						button_state.back = true;
						break;
				}
			}
		}
	}
}

bool HAL_Buttonup(void)
{
	bool pressed = button_state.up;
	button_state.up = false;
	return pressed;
}

bool HAL_Buttondown(void)
{
	bool pressed = button_state.down;
	button_state.down = false;
	return pressed;
}

bool HAL_Buttonselect(void)
{
	bool pressed = button_state.select;
	button_state.select = false;
	return pressed;
}

bool HAL_Buttonback(void)
{
	bool pressed = button_state.back;
	button_state.back = false;
	return pressed;
}

