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
	bool remove(int id);

	Q_INVOKABLE
	int remain();

signals:
	void remainChanged(int remain);
private:
	Q_INVOKABLE
	void init();
private:
	bb::cascades::DataModel* dataModel() const;

	int m_iRemainTime;
	bb::cascades::GroupDataModel *m_dataModel;
};

#endif /* ApplicationUI_HPP_ */
