#include "loginpage.h"
#include "registerpage.h"
#include "mainwindow.h"
#include "ui_loginpage.h"
#include <QSQlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QWindow>
#include "sessionmanager.h"
#include "admin.h"
#include "regularuser.h"


loginPage::loginPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginPage)
{

    ui->setupUi(this);

    this->setStyleSheet(
        "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
        "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI', sans-serif; font-size: 12pt; }"
        "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 6px 15px; font-weight: bold; min-width: 70px; }"
        "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
        );

    QPixmap pixmap(":/new/prefix1/logo/Gemini_Generated_Image_nlog6vnlog6vnlog__1_-removebg-preview.png");
    int width = ui->labelLogo->width();
    int height = ui->labelLogo->height();
    QPixmap scaledPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->labelLogo->setPixmap(scaledPixmap);
}

loginPage::~loginPage()
{
    delete ui;
}



void loginPage::on_pushButton_2_clicked()
{
    RegisterPage *window = new RegisterPage;
    this->hide();
    window->show();
}


void loginPage::on_pushButton_clicked()
{
    QString Login = ui->lineEdit->text();
    QString Password = ui->lineEdit_2->text();

    if (Login.isEmpty() || Password.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, заповніть усі поля для входу!");
        return;
    }

    // Очищаємо попередню сесію, якщо вона існувала
    if (currentUserSession != nullptr) {
        delete currentUserSession;
        currentUserSession = nullptr;
    }

    QSqlQuery query;

    if (ui->checkBox->isChecked()) {
        query.prepare("SELECT Password FROM Administrator WHERE Login = :Login");
        query.bindValue(":Login", Login);

        if (query.exec() && query.next()) {
            QString dbPassword = query.value(0).toString();

            if (Password == dbPassword) {
                currentUserSession = new Admin(Login, 10);
                globalSessionManager.loginUser(currentUserSession);

                QMessageBox::information(this, "Успішно", "Авторизація успішна! Вітаємо, ви увійшли як адміністратор!");

                MainWindow *window = new MainWindow;
                window->setAttribute(Qt::WA_DeleteOnClose);
                window->show();
                this->close();
                return;
            }
        }
    }
    else {
        query.prepare("SELECT Password, trainCount FROM User WHERE Login = :Login");
        query.bindValue(":Login", Login);

        if (query.exec() && query.next()) {
            QString dbPassword = query.value(0).toString();
            int dbTrainCount = query.value(1).toInt();

            if (Password == dbPassword) {
                currentUserSession = new ClientUser(Login, dbTrainCount);
                globalSessionManager.loginUser(currentUserSession);

                QMessageBox::information(this, "Успішно", "Авторизація успішна! Вітаємо, в системі!");

                MainWindow *window = new MainWindow;
                window->setAttribute(Qt::WA_DeleteOnClose);
                window->show();
                this->close();
                return;
            }
        }
    }

    QMessageBox::critical(this, "Error", "Невірний логін або пароль!");
}

void loginPage::on_checkBox_clicked()
{

}

