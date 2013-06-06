/*
 * log.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: CongDanh
 */

#include "log.h"
#include <stdio.h>

#ifdef QT_DEBUG
void logToConsole(const QString& msg) {
	fprintf(stdout, "%s\n", msg.toUtf8().constData());
	fflush(stdout);
}
#endif
