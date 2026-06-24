#include "doorder.h"
#include "ui_doorder.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

doOrder::doOrder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::doOrder)
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


    loadServicesToComboBox();
    loadCoachByService();
    connect(ui->comboBoxSport,
            &QComboBox::currentIndexChanged,
            this,
            [this]()
            {
                loadCoachByService();
            });
    calculatePrice();
    connect(ui->comboBoxSport,
            &QComboBox::currentIndexChanged,
            this,
            &doOrder::calculatePrice);

    connect(ui->spinBox,
            &QSpinBox::valueChanged,
            this,
            &doOrder::calculatePrice);

}

doOrder::~doOrder()
{
    delete ui;
}

void doOrder::on_pushButton_3_clicked()
{
    MainWindow *window = new MainWindow;
    this->close();
}


void doOrder::on_btnPay_clicked()
{
    QString login = ui->loginLine->text();
    QString passWord = ui->lineEdit_2->text();
    int trainCount = ui->spinBox->value();

    QSqlQuery query;

    query.prepare("SELECT ID_User, trainCount "
        "FROM User "
                  "WHERE Login = ? AND Password = ?");

    query.addBindValue(login);
    query.addBindValue(passWord);

    if(!query.exec() || !query.next())
    {
        QMessageBox::warning(this,
                             "Помилка",
                             "Невірний логін або пароль");
        return;
    }

    int userId = query.value(0).toInt();
    int currentCount = query.value(1).toInt();

    QSqlQuery updateQuery;
    updateQuery.prepare(
        "UPDATE User "
        "SET trainCount = ? "
        "WHERE ID_User = ?"
        );

    updateQuery.addBindValue(currentCount + trainCount);
    updateQuery.addBindValue(userId);

    if(!updateQuery.exec())
    {
        QMessageBox::critical(this,
                              "Помилка",
                              updateQuery.lastError().text());
        return;
    }

    int serviceId = ui->comboBoxSport->currentData().toInt();

    QSqlQuery saleQuery;
    saleQuery.prepare(
        "INSERT INTO Sales(ID_Service, ID_User, SaleDate) "
        "VALUES (?, ?, DATE('now'))"
        );

    saleQuery.addBindValue(serviceId);
    saleQuery.addBindValue(userId);

    if(!saleQuery.exec())
    {
        QMessageBox::critical(this,
                              "Помилка",
                              saleQuery.lastError().text());
        return;
    }

    QMessageBox::information(this,
                             "Успіх",
                             "Оплату проведено!");

}

void doOrder::loadServicesToComboBox()
{
    ui->comboBoxSport->clear();

    QSqlQuery query;
    query.exec("SELECT ID_Service, Name FROM Service");

    while(query.next())
    {
        ui->comboBoxSport->addItem(
            query.value("Name").toString(),
            query.value("ID_Service").toInt()
            );
    }
}

void doOrder::loadCoachByService()
{
    ui->listWidget->clear();

    int serviceId = ui->comboBoxSport->currentData().toInt();

    QSqlQuery query;
    query.prepare(
        "SELECT Name "
        "FROM Coach "
        "WHERE ID_Service = ?"
        );

    query.addBindValue(serviceId);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return;
    }

    while(query.next())
    {
        ui->listWidget->addItem(
            query.value(0).toString()
            );
    }
}

void doOrder::calculatePrice()
{
    int serviceId = ui->comboBoxSport->currentData().toInt();
    int trainCount = ui->spinBox->value();

    QSqlQuery query;
    query.prepare("SELECT Price FROM Service WHERE ID_Service = ?");
    query.addBindValue(serviceId);

    if(query.exec() && query.next())
    {
        double price = query.value(0).toDouble();
        double total = price * trainCount;

        ui->lineEdit_3->setText(
            QString::number(total, 'f', 2) + " грн"
            );
    }
}

void doOrder::on_radioButton_clicked()
{
    {
        QString text = ui->lineEdit_3->text();
        text.remove(" грн");

        double totalPrice = text.toDouble();

        if(ui->radioButton->isChecked()){
        totalPrice *= 0.9;
        }else
        {
            totalPrice /= 0.9;
        }
        ui->lineEdit_3->setText(
            QString::number(totalPrice, 'f', 2) + " грн"
            );
    }
}


void doOrder::on_radioButton_2_clicked()
{
    {
        QString text = ui->lineEdit_3->text();
        text.remove(" грн");

        double totalPrice = text.toDouble();
        if(ui->radioButton_2->isChecked())
        {
        totalPrice *= 0.75;
        }else
        {
            totalPrice/= 0.75;
        }
        ui->lineEdit_3->setText(
            QString::number(totalPrice, 'f', 2) + " грн"
            );
    }
}


void doOrder::on_pushButton_2_clicked()
{
    ui->loginLine->clear();
    ui->lineEdit_2->clear();
    ui->spinBox->clear();
    ui->lineEdit_3->clear();
}

