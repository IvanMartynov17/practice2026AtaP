#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>

namespace Ui {
class loginPage;
}

class loginPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginPage(QWidget *parent = nullptr);
    ~loginPage();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_checkBox_clicked();

private:
    Ui::loginPage *ui;
};

#endif // LOGINPAGE_H
