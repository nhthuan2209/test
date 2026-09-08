#ifndef _RFID_H_
#define _RFID_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define RFID_UID_LEN					5

bool Rfid_TryReadUid(uint8_t* uid_out, size_t out_len);

#endif // _RFID_H_