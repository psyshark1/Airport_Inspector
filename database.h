#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
//#include <QtSql/QSqlQueryModel>
//#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "structs.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase();
    void RequestToDB(const QString& request, const int& reqType);
    QSqlError GetLastError();
    void ConnectToDataBase();
    QSqlQuery& GetQuery();
    const bool& GetConnectionStatus();
    QList<QString>* sqlbindvals;

signals:
    void sig_SendStatusConnection(const bool& status);
    void sig_SendStatusRequest(QSqlError err, const int& reqType);

private:
    QSqlDatabase* dataBase;
    QSqlQuery *query;
    bool connStatus{false};
};

#endif // DATABASE_H
