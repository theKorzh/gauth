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

}  // namespace cascades

}  // namespace bb

namespace gauth {

class QrScanner: public QObject {
	Q_OBJECT
public:
	explicit QrScanner(QObject* parent = 0);
	virtual ~QrScanner();

	Q_INVOKABLE void process(QString data);
signals:
	void detected(const QString &account,const QString &key, int hotp);
private:
	bb::cascades::Sheet *m_pRoot;
};

} /* namespace gauth */
#endif /* QRSCANNER_H_ */
