#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

class Admin : public User
{
private:
    int securityLevel;

public:
    Admin(QString uN, int sL) : User(uN, "Адміністратор")
    {
        securityLevel = sL;
    }

    virtual ~Admin()
    {
    }

    virtual QString getPermissions() const
    {
        return "ПОВНИЙ ДОСТУП: Керування БД, фінанси, налаштування.";
    }

    virtual void displayDashboard() const
    {
        qDebug() << "Панель Admin:" << username << "| Рівень доступу до БД:" << securityLevel;
    }
};

#endif // ADMIN_H