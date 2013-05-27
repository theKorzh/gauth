/*
 * compute_code.cpp
 *
 *  Created on: May 26, 2013
 *      Author: CongDanh
 */
#include "compute_code.h"
#include "hmac.h"
#include "sha1.h"
#include "time.h"

int getHotpCode(const uint8_t* secret, int secretLen, unsigned long /*value*/ step) {
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
	truncatedHash %= 1000000;
	return truncatedHash;
}

int getHotpCode(const QString& secret, int secretLen, unsigned long /*value*/ step) {
	const char* data = secret.toUtf8().constData();
	const uint8_t* s = (const uint8_t*) data;
	return getHotpCode(s, secretLen, step);
}

int getTotpCode(const uint8_t *secret, int secretLen){
	time_t t = time(NULL);
	unsigned long step = t / 30;
	return getHotpCode(secret, secretLen, step);
}
