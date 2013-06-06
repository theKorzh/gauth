/*
 * log.h
 *
 *  Created on: Jun 6, 2013
 *      Author: CongDanh
 */

#ifndef LOG_H_
#define LOG_H_
#include <QString>
#ifdef QT_DEBUG
void logToConsole(const QString& msg);
#else
#define logToConsole(x)
#endif

#endif /* LOG_H_ */
