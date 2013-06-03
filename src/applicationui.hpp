// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

namespace bb {
namespace cascades {
class Application;
class DataModel;
class GroupDataModel;
}
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
	/*!
	 * @brief Add new key Entry
	 */
	Q_INVOKABLE
	void add(QString account, QString key, int type);
	/*!
	 * @brief Remove an Entry based on its Id
	 */
	Q_INVOKABLE
	bool remove(int id);

	Q_INVOKABLE
	int elapsed();

	Q_INVOKABLE void scanBarcode();
signals:
	void elapsedChanged(int elapsed);
private slots:
	void updateTime();
	void nextTotp();
private:
	Q_INVOKABLE
	void init();
private:
	bb::cascades::DataModel* dataModel() const;

	int m_iElapsed;
	bb::cascades::GroupDataModel *m_dataModel;
};

#endif /* ApplicationUI_HPP_ */
