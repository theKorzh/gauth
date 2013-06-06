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
	QmlDocument *qml = QmlDocument::create("asset:///QRScanner.qml").parent(this);
	qml->setContextProperty("_scanner", this);
	// create root object for the UI
	m_pRoot = qml->createRootObject<Sheet>();
	m_pRoot->open();
}

QrScanner::~QrScanner() {
	// m_pRoot->close();
}

void QrScanner::process(QString data){
	QString lower = data.toLower();
	if(lower.startsWith("otpauth://")){
		QStringRef ref = lower.midRef(10);
		bool hotp = true;
		if(ref.startsWith("totp/")){
			hotp = false;
		}
		ref = lower.midRef(15);
		// TODO: Debug here
		QStringList list = ref.string()->split('?');
		if(list[2].startsWith("secret=")){
			QString secret = list[2].mid(7);
			Q_EMIT detected(list[1], secret, hotp);
		}
	}
}

} /* namespace gauth */
