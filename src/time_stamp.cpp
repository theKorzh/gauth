/*
 * time_stamp.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: CongDanh
 */

#include "time_stamp.h"
#include "time.h"

unsigned long s_lTimeStamp;

void time_stamp_init() {
	s_lTimeStamp = time(NULL) / 30;
}

void time_stamp_update() {
	++s_lTimeStamp;
}
