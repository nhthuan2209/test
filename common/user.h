#ifndef _USER_H_
#define _USER_H_

#include "stdint.h"

#ifndef ARRAY_COUNT
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#endif

typedef struct 
{
	uint8_t uid[4];
	char name[20];
	
	uint8_t character_id;
}Card;

int8_t User_FindUid(uint8_t *uid);
#endif // _USER_H_