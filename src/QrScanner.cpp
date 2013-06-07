/*
 * QrScanner.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: CongDanh
 */

#include "QrScanner.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/Sheet>

#include <QUrl>

#include "log.h"

using namespace bb::cascades;


namespace gauth {

QrScanner::QrScanner(QObject* parent) :
		QObject(parent), m_bClosing(false) {
	QmlDocument *qml = QmlDocument::create("asset:///QRScanner.qml").parent(
			this);
	qml->setContextProperty("_scanner", this);
	// create root object for the UI
	m_pRoot = qml->createRootObject<Sheet>();
	m_pRoot->setParent(this);
}

QrScanner::~QrScanner() {
	logToConsole("Destroyed.");
	// m_pRoot->close();
}

void QrScanner::process(const QString& data) {
	if(m_bClosing)
		return;
	QUrl url(data);
	logToConsole(QString("Processing: %1").arg(data));
	logToConsole(QString("Scheme: %1").arg(url.scheme()));
	if(!QString("otpauth").compare(url.scheme(), Qt::CaseInsensitive)){
		bool hotp = false;
		int digits = 6;
		QString path = QUrl::fromPercentEncoding(url.path().mid(1).toAscii());
		QString secret = url.queryItemValue("secret");
		if(QString("totp").compare(url.host(), Qt::CaseInsensitive)){
			hotp = true;
		}
		if(url.hasQueryItem("digits")){
			digits = url.queryItemValue("digits").toInt();
		}
		Q_EMIT detected(path, secret, digits, hotp);
		logToConsole(data);
		logToConsole(QString("Path: %1, Secret: %2, Digit: %3, HOTP: %4").arg(path).arg(secret).arg(digits).arg(hotp));
	}
}

void QrScanner::markForDelete() {
	deleteLater();
}

void QrScanner::open(){
	m_pRoot->open();
}

void QrScanner::close() {
	m_bClosing = true;
	m_pRoot->close();
}

} /* namespace gauth */
