#include "registerpage.h"
#include "ui_registerpage.h"
#include "loginpage.h"
#include <QPixmap>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
RegisterPage::RegisterPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterPage)
{
    ui->setupUi(this);


    this->setStyleSheet(
        "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
        "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI', sans-serif; font-size: 12pt; }"
        "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 6px 15px; font-weight: bold; min-width: 70px; }"
        "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
        );


    QPixmap pixmap(":/new/prefix1/logo/Gemini_Generated_Image_nlog6vnlog6vnlog__1_-removebg-preview.png");


    if (pixmap.isNull()) {
        qDebug() << "КРИТИЧНО: Картинку НЕ знайдено в ресурсах вікна реєстрації!";
    } else {

        int width = ui->labelLogo->width() > 0 ? ui->labelLogo->width() : 200;
        int height = ui->labelLogo->height() > 0 ? ui->labelLogo->height() : 200;

        QPixmap scaledPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labelLogo->setPixmap(scaledPixmap);
    }
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::on_pushButton_2_clicked()
{
    loginPage *window = new loginPage;
    this->hide();
    window->show();
}


void RegisterPage::on_pushButton_clicked()
{
    QString Login = ui->lineEdit->text().trimmed();
    QString Password = ui->lineEdit_2->text();
    QString confirmPassword = ui->lineEdit_3->text();

    if(Login.isEmpty() || Password.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Поля не можуть бути пусті");
        return;
    }

    if(Password.length() < 8)
    {
        QMessageBox::warning(this, "Слабкий пароль",
                             "Пароль занадто короткий! Довжина має бути мінімум 8 символів.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT Login FROM User WHERE Login = :Login");
    checkQuery.bindValue(":Login", Login);

    if (checkQuery.exec()) {
        if (checkQuery.next()) {
            QMessageBox::warning(this, "Помилка реєстрації",
                                 "Користувач з логіном \"" + Login + "\" вже існує! Виберіть інший логін.");
            return;
        }
    } else {
        QMessageBox::critical(this, "Помилка БД", "Не вдалося перевірити логін: ");
        return;
    }

    int newId = 100;
    QSqlQuery idQuery("SELECT MAX(ID_User) FROM User"); //
    if (idQuery.exec() && idQuery.next()) {
        int maxId = idQuery.value(0).toInt();
        if (maxId > 0) {
            newId = maxId + 1;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO User (ID_User, Login, Password) VALUES (:ID_User, :Login, :Password)");
    query.bindValue(":ID_User", newId);
    query.bindValue(":Login", Login);
    query.bindValue(":Password", Password);

    if(Password!=confirmPassword)
    {
        QMessageBox::critical(this, "Error", "Паролі не співпадають");
        return;
    }else if (query.exec()) {
        QMessageBox::information(this, "Успіх", "Реєстрація пройшла успішно!");

        loginPage *window = new loginPage;
        window->setAttribute(Qt::WA_DeleteOnClose);
        this->close();
        window->show();
    } else {
        QMessageBox::critical(this, "Помилка БД", "Не вдалось зареєструватись");
    }
}

