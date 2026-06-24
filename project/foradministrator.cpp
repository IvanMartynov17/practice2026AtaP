#include "foradministrator.h"
#include "ui_foradministrator.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "mainwindow.h"

forAdministrator::forAdministrator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::forAdministrator)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    QStringList headers;
    headers << "ID Користувача" << "Логін (Ім'я)" << "Пароль" << "Кількість тренувань";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    loadUsers();
}

forAdministrator::~forAdministrator()
{
    delete ui;
}

void forAdministrator::loadUsers(QString searchText)
{

    ui->tableWidget->setRowCount(0);

    QSqlQuery query;

    if (searchText.isEmpty()) {
        query.prepare("SELECT ID_User, Login, Password, trainCount FROM User");
    } else {

        query.prepare("SELECT ID_User, Login, Password, trainCount FROM User WHERE Login LIKE :search");
        query.bindValue(":search", "%" + searchText + "%");
    }

    if (query.exec()) {
        int row = 0;

        while (query.next()) {

            ui->tableWidget->insertRow(row);

            QString id = query.value(0).toString();
            QString login = query.value(1).toString();
            QString password = query.value(2).toString();
            QString trainCount = query.value(3).toString();

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(login));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(password));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(trainCount));

            row++;
        }
    }
}

void forAdministrator::on_leSearch_textChanged(const QString &arg1)
{
    loadUsers(arg1);
}


void forAdministrator::on_pushButton_6_clicked()
{
    this->close();
}


void forAdministrator::on_pushButton_7_clicked()
{
    MainWindow *window = new MainWindow;
    window->show();
    this->close();
}

