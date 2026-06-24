#ifndef FORADMINISTRATOR_H
#define FORADMINISTRATOR_H

#include <QMainWindow>

namespace Ui {
class forAdministrator;
}

class forAdministrator : public QMainWindow
{
    Q_OBJECT

public:
    explicit forAdministrator(QWidget *parent = nullptr);
    ~forAdministrator();

private slots:
    void on_leSearch_textChanged(const QString &arg1);


    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::forAdministrator *ui;
    void loadUsers(QString searchText = "");
};

#endif // FORADMINISTRATOR_H
