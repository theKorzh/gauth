/*
 * AccountItem.h
 *
 *  Created on: May 27, 2013
 *      Author: CongDanh
 */

#ifndef ACCOUNTITEM_H_
#define ACCOUNTITEM_H_

#include <stdint.h>
#include <qt4/QtCore/QObject>
#include <qt4/QtCore/qmetatype.h>

class AccountItem: public QObject {
	Q_OBJECT
	Q_PROPERTY(int id READ id FINAL)
	Q_PROPERTY(int code READ code NOTIFY codeChanged FINAL)
	Q_PROPERTY(int type READ type FINAL)
	Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged FINAL)
	Q_PROPERTY(QString email READ email FINAL)
	Q_PROPERTY(bool next READ next FINAL)
public:
	AccountItem(QObject* parent = 0);
	AccountItem(const AccountItem&);
	AccountItem(int id, const QString& email, const QString& secret, int type, int counter, QObject* parent = 0);
	virtual ~AccountItem();

	int id() const;
	QString email() const;
	int code() const;
	int type() const;
	bool enabled() const;
	bool next();

	Q_SIGNALS:
	void codeChanged(int code);
	void enabledChanged(bool enabled);

private slots:
	void setCode(int code);
	void setEnabled(bool enabled);
private:
	int m_iId;
	int m_iCode;
	int m_iType;
	int m_iCounter;
	int m_len;
	uint8_t* m_pSecret;
	bool m_iEnabled;
	QString m_email;
};

// Q_DECLARE_METATYPE( AccountItem* )
#endif /* ACCOUNTITEM_H_ */
