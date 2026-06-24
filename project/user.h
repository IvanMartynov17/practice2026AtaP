#ifndef USER_H
#define USER_H

#include <QString>
#include <QDebug>

class User
{
protected:
    QString username;
    QString role;

public:
    User(QString uN, QString r)
    {
        username = uN;
        role = r;
    }

    virtual ~User()
    {
    }

    virtual QString getPermissions() const = 0;
    virtual void displayDashboard() const = 0;

    QString getUsername() const
    {
        return username;
    }

    QString getRole() const
    {
        return role;
    }
    void setUsername(const QString &newLog) {
        username = newLog;
    }
};

extern User* currentUserSession;

#endif // USER_H