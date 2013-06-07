/*
 * QrScanner.h
 *
 *  Created on: Jun 4, 2013
 *      Author: CongDanh
 */

#ifndef QRSCANNER_H_
#define QRSCANNER_H_

#include <QObject>

namespace bb {

namespace cascades {

class Sheet;

} // namespace cascades

} // namespace bb

namespace gauth {

class QrScanner: public QObject {
Q_OBJECT
public:
	explicit QrScanner(QObject* parent = 0);
	virtual ~QrScanner();

	Q_INVOKABLE
	void process(const QString& data);
	Q_INVOKABLE void markForDelete();

public slots:
	void open();
	void close();

signals:
	void detected(const QString &account, const QString &key, int digits, int hotp);
private:
	bb::cascades::Sheet *m_pRoot;
	bool m_bClosing;
};

} /* namespace gauth */
#endif /* QRSCANNER_H_ */
