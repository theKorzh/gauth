// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/QListDataModel>

namespace bb {
namespace cascades {
class Application;
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

Q_PROPERTY(int remain READ remain NOTIFY remainChanged FINAL)
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
	void remove(int id);

	Q_INVOKABLE
	int remain();

signals:
	void remainChanged(int remain);
private:
	Q_INVOKABLE
	void init();
private:
	int m_iRemainTime;
	bb::cascades::QListDataModel<AccountItem*> *dataModel() const;

	bb::cascades::QListDataModel<AccountItem*> *m_dataModel;
};

#endif /* ApplicationUI_HPP_ */
