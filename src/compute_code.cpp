/*
 * compute_code.cpp
 *
 *  Created on: May 26, 2013
 *      Author: CongDanh
 */
#include "compute_code.h"
#include "hmac.h"
#include "sha1.h"
#include <QDebug>

extern unsigned long g_lTimeStamp;

int powerOf10(int digits){
	switch (digits){
	case 6: return 1000000;
	case 0: return 1;
	case 1: return 10;
	case 2: return 100;
	case 3: return 1000;
	case 4: return 10000;
	case 5: return 100000;
	case 8: return 10000000;
	case 9: return 100000000;
	case 10: return 1000000000;
	default: return 1000000;
	}
}

int getHotpCode(const uint8_t* secret, int secretLen, unsigned long /*value*/ step, int digits) {
	uint8_t val[8];
	for (int i = 8; i--; step >>= 8) {
		val[i] = step;
	}
	uint8_t hash[SHA1_DIGEST_LENGTH];
	hmac_sha1(secret, secretLen, val, 8, hash, SHA1_DIGEST_LENGTH);
	// memset(val, 0, sizeof(val));
	int offset = hash[SHA1_DIGEST_LENGTH - 1] & 0xF;
	unsigned int truncatedHash = 0;
	for (int i = 0; i < 4; ++i) {
		truncatedHash <<= 8;
		truncatedHash |= hash[offset + i];
	}
	// memset(hash, 0, sizeof(hash));
	truncatedHash &= 0x7FFFFFFF;
	truncatedHash %= powerOf10(digits);
	return truncatedHash;
}

int getTotpCode(const uint8_t *secret, int secretLen, int digits){
	return getHotpCode(secret, secretLen, g_lTimeStamp, digits);
}
