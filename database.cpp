#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}, sqlbindvals(new QList<QString>())
{
    dataBase = new QSqlDatabase();
    *dataBase = QSqlDatabase::addDatabase(POSTGRE_DRIVER,DB_NAME);
    dataBase->setHostName(dbParams.host);
    dataBase->setDatabaseName(dbParams.dbName);
    dataBase->setUserName(dbParams.user);
    dataBase->setPassword(dbParams.pass);
    dataBase->setPort(dbParams.port);
    query = new QSqlQuery(*dataBase);
}

Database::~Database()
{
    delete dataBase;
    delete query;
    delete sqlbindvals;
}
void Database::ConnectToDataBase()
{
    connStatus = dataBase->open();
    emit sig_SendStatusConnection(connStatus);
}

void Database::DisconnectFromDataBase()
{
    *dataBase = QSqlDatabase::database(DB_NAME);
    dataBase->close();
}

void Database::RequestToDB(const QString& request, const int& reqType)
{
    QSqlError err;
    query->clear();
    query->prepare(request);
    if(reqType != requestType::requestAirportList)
    {
        for (auto i = sqlbindvals->cbegin(); i != sqlbindvals->cend(); ++i)
        {
            query->addBindValue(*i);
        }
    }
    query->exec();
    err = query->lastError();
    emit sig_SendStatusRequest(std::move(err), reqType);
}

QSqlError Database::GetLastError()
{
    return dataBase->lastError();
}

QSqlQuery& Database::GetQuery()
{
    return *query;
}

const bool& Database::GetConnectionStatus()
{
    connStatus = dataBase->isOpen();
    return connStatus;
}
