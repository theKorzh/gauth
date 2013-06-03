// Default empty project template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>

#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>

#include <QtSql/QtSql>
#include <QDebug>

#include "AccountItem.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app), m_iRemainTime(10) {
	m_dataModel = new GroupDataModel(this);
	m_dataModel->setGrouping(ItemGrouping::None);
	m_dataModel->setSortingKeys(QStringList() << "id");
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
	// delete m_dataModel;
}

bb::cascades::DataModel* ApplicationUI::dataModel() const {
	return m_dataModel;
}

void ApplicationUI::add(QString account, QString key, int type) {
	int counter = 0;
	QSqlDatabase database = QSqlDatabase::database();
	QSqlQuery query(database);
	query.prepare("INSERT INTO accounts"
	                  "    (email, secret, counter, type) "
	                  "    VALUES (:email, :secret, :counter, :type)");
	query.bindValue(":email", account);
	query.bindValue(":secret", key);
	query.bindValue(":counter", counter);
	query.bindValue(":type", type);
	if(query.exec()){
		int id = query.lastInsertId().toInt();
		m_dataModel->insert(new AccountItem(id, account, key, type, counter, this));
	} else {
		qDebug() << "Create record error: " << query.lastError().text();
	}
	database.close();
}

bool ApplicationUI::remove(int id) {
	QSqlDatabase database = QSqlDatabase::database();
	QSqlQuery query(database);
	query.prepare("DELETE FROM accounts WHERE id=:id");
	query.bindValue(":id", id);
	bool success = query.exec();
	database.close();
	return success;
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
			m_dataModel->insert(item);
		}
	}
}

