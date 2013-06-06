/*
 * QrScanner.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: CongDanh
 */

#include "QrScanner.h"
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Sheet>

using namespace bb::cascades;

namespace gauth {

QrScanner::QrScanner(QObject* parent) :
		QObject(parent) {
	QmlDocument *qml = QmlDocument::create("asset:///QRScanner.qml").parent(
			this);
	qml->setContextProperty("_scanner", this);
	// create root object for the UI
	m_pRoot = qml->createRootObject<Sheet>();
	m_pRoot->open();
}

QrScanner::~QrScanner() {
	logToConsole("Scanner Destroyed");
	// m_pRoot->close();
}

void QrScanner::process(const QString& data) {
	QString lower = data.toLower();
	if (lower.startsWith("otpauth://")) {
		QStringRef ref = lower.midRef(10);
		bool hotp = true;
		if (ref.startsWith("totp/")) {
			hotp = false;
		}
		QString substr = lower.mid(15);
		// TODO: Debug here
		QStringList list = substr.split('?');
		if (list[1].startsWith("secret=")) {
			QString secret = list[1].mid(7);
			logToConsole("Emitting Detecting.");
			Q_EMIT detected(list[0], secret, hotp);
			logToConsole("Emitting Detected.");
		}
	}
}

void QrScanner::logToConsole(const QString& msg) {
	fprintf(stdout, "%s\n", msg.toUtf8().constData());
	fflush(stdout);
}

void QrScanner::destroy() {
	qDebug() << "Sheet Closing";
	m_pRoot->close();
}

} /* namespace gauth */
