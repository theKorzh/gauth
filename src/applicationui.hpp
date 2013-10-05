// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QMutex>

namespace bb {
namespace cascades {
class Application;
class DataModel;
class GroupDataModel;
}
namespace system {
class Clipboard;
}  // namespace system
}
class AccountItem;
/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI: public QObject {
Q_OBJECT

Q_PROPERTY(int elapsed READ elapsed NOTIFY elapsedChanged FINAL)
Q_PROPERTY(bb::cascades::DataModel* dataModel READ dataModel CONSTANT)
public:
	ApplicationUI(bb::cascades::Application *app);
	virtual ~ApplicationUI();

public slots:
	/*!
	 * @brief Add new key Entry
	 */
	void add(const QString &account, const QString &key, int digits, int type);
public:
	/*!
	 * @brief Remove an Entry based on its Id
	 */
	Q_INVOKABLE
	bool remove(int id);

	Q_INVOKABLE
	void scanBarcode();

	Q_INVOKABLE
	void insertToClipboard(const QString &data);

signals:
	void elapsedChanged(int elapsed);
private slots:
	void updateTime();
	void nextTotp();
private:
	Q_INVOKABLE
	void init();
	int elapsed();
	bb::cascades::DataModel* dataModel() const;

	int m_iElapsed;
	bb::cascades::GroupDataModel *m_dataModel;
	bb::system::Clipboard *m_pClipboard;
	QMutex m_mutex;
};

#endif /* ApplicationUI_HPP_ */
