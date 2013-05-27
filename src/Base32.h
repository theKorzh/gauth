/*
 * Base32.h
 *
 *  Created on: May 9, 2013
 *      Author: CongDanh
 */

#ifndef BASE32_H_
#define BASE32_H_
#include <stdint.h>
#include <QByteArray>
#include <QString>
class Base32 {
public:
	static int decode(const char *encoded, char *result, int bufSize);
	static QByteArray decode(const QString& encoded);
	static int encode(const char *data, int length, char *result, int bufSize);
	static QString encode(const QByteArray& data);
};

#endif /* BASE32_H_ */
