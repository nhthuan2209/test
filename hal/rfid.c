#include "rfid.h"
#include "rc522.h"

bool Rfid_TryReadUid(uint8_t* uid_out, size_t out_len)
{
	if (out_len < RFID_UID_LEN) {
		return false;
	}

	if (MFRC522_Request(PICC_REQIDL, uid_out) != MI_OK) {
		return false;
	}

	if (MFRC522_Anticoll(uid_out) != MI_OK) {
		return false;
	}

	return true;
}