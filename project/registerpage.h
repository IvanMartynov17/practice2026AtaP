#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QMainWindow>

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
