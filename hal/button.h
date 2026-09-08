#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool up;
    bool down;
    bool select;
    bool back;
} ButtonState;

void HAL_ScanButtons(void);
bool HAL_Buttonup(void);
bool HAL_Buttondown(void);
bool HAL_Buttonselect(void);
bool HAL_Buttonback(void);

#endif // _BUTTON_H_
