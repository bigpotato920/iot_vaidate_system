#include "sqlhelper.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>

SQLHelper::SQLHelper()
{
}


bool SQLHelper::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("sensor_platform");
    db.setUserName("root");
    db.setPassword("su");

    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }

    return true;
}
