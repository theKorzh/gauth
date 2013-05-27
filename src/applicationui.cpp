// Default empty project template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include "AccountItem.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app), m_iRemainTime(10), m_dataModel(
				new QListDataModel<AccountItem*>()) {
	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("_app", this);
	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);
	QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

ApplicationUI::~ApplicationUI() {
	delete m_dataModel;
}

bb::cascades::QListDataModel<AccountItem*>* ApplicationUI::dataModel() const {
	return m_dataModel;
}

void ApplicationUI::add(QString account, QString key, int type) {
}

void ApplicationUI::remove(int id) {
}

int ApplicationUI::remain() {
	return m_iRemainTime;
}

void ApplicationUI::init() {
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName("./data/data.sqlite");

	if (database.open() == false) {
		const QSqlError error = database.lastError();
		qDebug() << "\nDatabase NOT opened.";
		return; // return as if we cannot open a connection to the db, then below calls
				// will also fail
	}
	QSqlQuery query(database);
	query.exec(
			"CREATE TABLE IF NOT EXISTS accounts"
					" (id INTEGER PRIMARY KEY, email TEXT NOT NULL, secret TEXT NOT NULL,"
					" counter INTEGER DEFAULT 0, type INTEGER)");
	if (query.exec("SELECT id, email, secret, type, counter FROM accounts")) {
		while (query.next()) {
			QSqlRecord record = query.record();
			AccountItem *item = new AccountItem(record.value(0).toInt(), // Id
					record.value(1).toString(), // Email
					record.value(2).toString(), // Secret
					record.value(3).toInt(), // Type
					record.value(4).toInt(), // Counter
					this // Parent
					);
			m_dataModel->append(item);
		}
	}
}

