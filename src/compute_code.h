/*
 * compute_code.h
 *
 *  Created on: May 26, 2013
 *      Author: CongDanh
 */

#ifndef COMPUTE_CODE_H_
#define COMPUTE_CODE_H_

#include <stdint.h>
#include <QString>

int getHotpCode(const uint8_t *secret, int secretLen, unsigned long step);
int getHotpCode(const QString& secret, int secretLen, unsigned long /*value*/ step);
int getTotpCode(const uint8_t *secret, int secretLen);

#endif /* COMPUTE_CODE_H_ */
