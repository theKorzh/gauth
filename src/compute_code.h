/*
 * compute_code.h
 *
 *  Created on: May 26, 2013
 *      Author: CongDanh
 */

#ifndef COMPUTE_CODE_H_
#define COMPUTE_CODE_H_

#include <stdint.h>

int getHotpCode(const uint8_t *secret, int secretLen, unsigned long step, int digits = 6);
int getTotpCode(const uint8_t *secret, int secretLen, int digits = 6);

#endif /* COMPUTE_CODE_H_ */
