#ifndef TRAINER_H
#define TRAINER_H

#include "user.h"

class ClientUser : public User
{
private:
    int trainCount;
public:
    ClientUser(QString uN, int tC) : User(uN, "Користувач")
    {
        trainCount = tC;
    }

    virtual ~ClientUser()
    {
    }

    virtual QString getPermissions() const
    {
        return "ОБМЕЖЕНИЙ ДОСТУП: Перегляд особистої інформації.";
    }

    virtual void displayDashboard() const
    {
        qDebug() << "Панель Користувача:" << username << "| Кількість тренувань:" << trainCount;
    }
};

#endif // TRAINER_H