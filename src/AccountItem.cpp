/*
 * AccountItem.cpp
 *
 *  Created on: May 27, 2013
 *      Author: CongDanh
 */

#include "AccountItem.h"

#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>
#include <QtSql/QtSql>

#include "log.h"
#include "compute_code.h"
#include "base32.h"

AccountItem::AccountItem(QObject* parent) :
		QObject(parent), m_iId(0), m_iType(0), m_iCounter(0), m_len(0), m_pSecret(
				NULL), m_iEnabled(1), m_code("000000"), m_email("") {

}
AccountItem::AccountItem(const AccountItem& src) :
		QObject(src.parent()), m_iId(src.m_iId), m_iType(src.m_iType), m_iCounter(
				src.m_iCounter), m_len(src.m_len), m_pSecret(src.m_pSecret), m_iEnabled(
				src.m_iEnabled), m_code(src.m_code), m_email(src.m_email) {
	logToConsole("Copied.");
}
AccountItem::AccountItem(int id, const QString& email, const QString& secret,
		int type, int counter, QObject* parent) :
		QObject(parent), m_iId(id), m_iType(type), m_iCounter(counter), m_iEnabled(
				1), m_code(""), m_email(email) {
	uint8_t* pTmp = new uint8_t[100];
	m_len = base32_decode((const uint8_t*) secret.toAscii().constData(), pTmp,
			100);
	m_pSecret = new uint8_t[m_len];
	memcpy(m_pSecret, pTmp, m_len);
	delete pTmp;
	int code;
	if (m_iType) {
		code = 0;
	} else {
		code = getTotpCode(m_pSecret, m_len);
	}
	m_code.sprintf("%06d", code);
}

AccountItem::~AccountItem() {
	if (m_pSecret) {
		delete[] m_pSecret;
	}
}

int AccountItem::id() const {
	return m_iId;
}

QString AccountItem::email() const {
	return m_email;
}

QString AccountItem::code() const {
	return m_code;
}

int AccountItem::type() const {
	return m_iType;
}

bool AccountItem::enabled() const {
	return m_iEnabled;
}

void AccountItem::setCode(int code) {
	m_code.sprintf("%06d", code);
	codeChanged(m_code);
}

void AccountItem::setEnabled(bool enabled) {
	if (m_iEnabled != enabled) {
		m_iEnabled = enabled;
		enabledChanged(m_iEnabled);
	}
}

bool AccountItem::next() {
	if (m_iType) {
		if (m_iEnabled) {
			m_iEnabled = false;
			enabledChanged(false);
			QSqlDatabase database = QSqlDatabase::database();
			QSqlQuery query(database);
			query.prepare("UPDATE accounts SET counter=:counter WHERE id=:id");
			query.bindValue(":id", m_iId);
			query.bindValue(":counter", m_iCounter + 1);
			if (query.exec()) {
				setCode(getHotpCode(m_pSecret, m_len, ++m_iCounter));
				QTimer *pTimer = new QTimer();
				connect(pTimer, SIGNAL(timeout()), this, SLOT(setEnabled()));
				connect(pTimer, SIGNAL(timeout()), pTimer, SLOT(stop()));
				connect(pTimer, SIGNAL(timeout()), pTimer, SLOT(deleteLater()));
				pTimer->start(5000);
			} else {
				m_iEnabled = true;
				enabledChanged(true);
			}
			database.close();
		}
		return false;
	} else {
		setCode(getTotpCode(m_pSecret, m_len));
		return true;
	}
}

