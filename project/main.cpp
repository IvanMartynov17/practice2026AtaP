#include "mainwindow.h"
#include "loginpage.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QApplication>
#include <QSqlError>
#include <QApplication>
#include "sessionmanager.h"

User* currentUserSession = nullptr;
SessionManager globalSessionManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginPage w;
    w.show();


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/User/Desktop/Практика 2026/база даних/dataBaseSportClubPractice2026.db");
    if(db.open())
    {
        qDebug() << "+";
    }else
    {
        qDebug() << "-";
    }

    QSqlQuery query;
    if(query.exec("SELECT * FROM User"))
    {

        while (query.next())
        {
            int iD = query.value("ID_User").toInt();
            QString login = query.value("Login").toString();
            QString password = query.value("Password").toString();
            qDebug() << iD << " " << login << " " << password;
        }
    }
    else
        qDebug() << query.lastError().text();

    return QCoreApplication::exec();
}
