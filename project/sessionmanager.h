#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QList>
#include "user.h"

class SessionManager;

void systemLogUser(const SessionManager& manager);
void forceLogoutAll(SessionManager& manager);

class SessionManager
{
private:
    QList<User*> activeUsers;
    QList<QString> administrators;

public:
    SessionManager()
    {
    }

    virtual ~SessionManager()
    {
        qDeleteAll(activeUsers);
        activeUsers.clear();
        administrators.clear();
    }

    void loginUser(User* user)
    {
        activeUsers.append(user);

        if (user->getRole() == "Адміністратор") {
            administrators.append(user->getUsername());
        }
    }

    bool isAdminInVector(QString login) const
    {
        return administrators.contains(login);
    }

    friend void systemLogUser(const SessionManager& manager);
    friend void forceLogoutAll(SessionManager& manager);
};

inline void systemLogUser(const SessionManager& manager) { }
inline void forceLogoutAll(SessionManager& manager) { }

extern User* currentUserSession;
extern SessionManager globalSessionManager;

#endif // SESSIONMANAGER_H