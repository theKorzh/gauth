// Default empty project template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/multimedia/BarcodeDetector>

#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>

#include <bb/system/Clipboard>

#include <QtSql/QtSql>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMutexLocker>

#include <time.h>

#include "AccountItem.h"
#include "QrScanner.h"
#include "log.h"

unsigned long g_lTimeStamp;

using namespace bb::cascades;
using namespace bb::system;

using namespace gauth;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app), m_iElapsed(0), m_pClipboard(NULL) {
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

void ApplicationUI::add(const QString &account, const QString &key, int digits, int type) {
	QMutexLocker locker(&m_mutex);
	logToConsole("Add Function Called");
	QSqlDatabase database = QSqlDatabase::database();
	QSqlQuery query(database);
	QList<QObject*> objects = m_dataModel->toListOfObjects();
	bool isNew = true;
	AccountItem* item = NULL;
	for (QList<QObject*>::iterator it = objects.begin();
			isNew && (it != objects.end()); ++it) {
		item = qobject_cast<AccountItem*>(*it);
		if (item->email() == account) {
			isNew = false;
			m_dataModel->remove(item);
		}
	}
	if (isNew) {
		query.prepare(
				"INSERT INTO accounts "
				"(email, secret, type, digits) "
				"VALUES (:email, :secret, :type, :digit)"
				);
	} else {
		query.prepare(
				"UPDATE accounts "
				"SET email=:email, secret=:secret, type=:type, digits=:digit "
				"WHERE id=:id"
				);
		query.bindValue(":id",item->id());
	}
	query.bindValue(":email", account);
	query.bindValue(":secret", key);
	query.bindValue(":type", type);
	query.bindValue(":digit", digits);
	if (query.exec()) {
		int id = isNew ? query.lastInsertId().toInt() : item->id();
		delete item;
		m_dataModel->insert(
				new AccountItem(id, account, key, type, 0, digits, this));
		logToConsole(QString("Added/Updated Id: %1").arg(id));
	} else {
		logToConsole(QString("Create record error: ").arg(query.lastError().text()));
	}
	database.close();
}

bool ApplicationUI::remove(int id) {
	QMutexLocker locker(&m_mutex);
	QSqlDatabase database = QSqlDatabase::database();
	QSqlQuery query(database);
	query.prepare("DELETE FROM accounts WHERE id=:id");
	query.bindValue(":id", id);
	bool success = query.exec();
	database.close();
	return success;
}

int ApplicationUI::elapsed() {
	return m_iElapsed;
}

void ApplicationUI::init() {
	timer_t t = time(NULL);
	g_lTimeStamp = t / 30;

	QTimer *pCountDownTimer = new QTimer(this);
	connect(pCountDownTimer, SIGNAL(timeout()), this, SLOT(updateTime()));

	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(nextTotp()));

	m_iElapsed = (t % 30) * 10 + QTime::currentTime().msec() / 100;
	pCountDownTimer->start(100);
	pTimer->start(30000 - m_iElapsed * 100);

	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName("./data/data.sqlite");

	if (database.open() == false) {
		const QSqlError error = database.lastError();
		logToConsole( "\nDatabase NOT opened." );
		return; // return as if we cannot open a connection to the db, then below calls
				// will also fail
	}
	QSqlQuery query(database);
	query.exec(
			"CREATE TABLE IF NOT EXISTS accounts"
					" (id INTEGER PRIMARY KEY, email TEXT NOT NULL, secret TEXT NOT NULL,"
					" counter INTEGER DEFAULT 0, type INTEGER, digits INTEGER DEFAULT 6)");

	if(query.exec("PRAGMA table_info(accounts)")){
		int nameIdx = query.record().indexOf("name");
		bool bb = true;
		while(bb && query.next()){
			if(!query.value(nameIdx).toString().compare("digits", Qt::CaseInsensitive)){
				logToConsole("Column Digits Existed.");
				bb = false;
			}
		}
		if(bb){
			query.exec("ALTER TABLE accounts ADD COLUMN digits INTEGER DEFAULT 6");
			logToConsole("Column Digits Created.");
		}
	}
	if (query.exec("SELECT id, email, secret, type, counter, digits FROM accounts")) {
		while (query.next()) {
			AccountItem *item = new AccountItem(query.value(0).toInt(), // Id
					query.value(1).toString(), // Email
					query.value(2).toString(), // Secret
					query.value(3).toInt(), // Type
					query.value(4).toInt(), // Counter
					query.value(5).toInt(), // Digits
					this // Parent
					);
			logToConsole(QString("Id: %1, Email %2").arg(item->id()).arg(item->email()));
			m_dataModel->insert(item);
		}
	}
}

void ApplicationUI::updateTime() {
	++m_iElapsed;
	elapsedChanged(m_iElapsed);
}

void ApplicationUI::nextTotp() {
	m_iElapsed = 0;
	elapsedChanged(0);
	++g_lTimeStamp;
	QTimer *pTimer = qobject_cast<QTimer*>(sender());
	pTimer->start(30000);
	QList<QObject*> objects = m_dataModel->toListOfObjects();
	for (QList<QObject*>::iterator it = objects.begin(); it != objects.end();
			++it) {
		AccountItem* item = qobject_cast<AccountItem*>(*it);
		if (!item->type()) {
			item->next();
		}
	}
}

void ApplicationUI::scanBarcode() {
	QrScanner *pScanner = new QrScanner(this);
	QObject::connect(pScanner,
			SIGNAL(detected(const QString&,const QString&, int, int)), this,
			SLOT(add(const QString&,const QString&, int, int)));
	QObject::connect(pScanner,
			SIGNAL(detected(const QString&,const QString&, int, int)), pScanner,
			SLOT(close()));
	pScanner->open();
}

void ApplicationUI::insertToClipboard(const QString& code){
	if(!m_pClipboard){
		m_pClipboard = new Clipboard(this);
	}
	m_pClipboard->insert("text/plain", code.toAscii());
}
