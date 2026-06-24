#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "loginpage.h"
#include "doorder.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QStatusBar>
#include <QColorDialog>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QSettings>
#include "sessionmanager.h"
#include "foradministrator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboLanguage->addItem("Українська");
    ui->comboLanguage->addItem("English");
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnTrainers->setChecked(true);
    connect(ui->btnTrainers, &QPushButton::clicked, this, [this]() {
    ui->stackedWidget->setCurrentIndex(0);

        ui->btnSports->setChecked(false);
        ui->btnLessons->setChecked(false);
        ui->btnSettings->setChecked(false);
    });

    connect(ui->btnSports, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);

        ui->btnTrainers->setChecked(false);
        ui->btnLessons->setChecked(false);
        ui->btnSettings->setChecked(false);
    });

    connect(ui->btnLessons, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);

        ui->btnTrainers->setChecked(false);
        ui->btnSports->setChecked(false);
        ui->btnSettings->setChecked(false);
    });

    connect(ui->btnSettings, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);

        ui->btnTrainers->setChecked(false);
        ui->btnSports->setChecked(false);
        ui->btnLessons->setChecked(false);
    });

    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);




    QPixmap pixmap(":/new/prefix1/logo/Gemini_Generated_Image_nlog6vnlog6vnlog__1_-removebg-preview.png");


    if (pixmap.isNull()) {
        qDebug() << "КРИТИЧНО: Картинку НЕ знайдено в ресурсах вікна реєстрації!";
    } else {

        int width = ui->labelLogo->width() > 0 ? ui->labelLogo->width() : 200;
        int height = ui->labelLogo->height() > 0 ? ui->labelLogo->height() : 200;

        QPixmap scaledPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labelLogo->setPixmap(scaledPixmap);
    }

    ui->statusBar->setStyleSheet(
        "QStatusBar { color: white; }"
        );

    ui->menubar->setStyleSheet(
        "QMenuBar {"
        "background-color: black;"
        "color: white;"
        "}"
        "QMenuBar::item:selected {"
        "background-color: #87CEFA;"
        "}"
        "QMenu {"
        "background-color: black;"
        "color: white;"
        "}"
        "QMenu::item:selected {"
        "background-color: #87CEFA;"
        "}"
        );

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM User WHERE trainCount > 0");

    if (query.exec() && query.next()) {
        int activePeople = query.value(0).toInt();
        int maxCapacity = 50;

        int percentage = (activePeople * 100) / maxCapacity;
        ui->progressBar->setValue(percentage);
    }

    loadSportsToComboBox();
    loadTrainersToTable();
    loadSportsToTable2();
    loadLessonsToTable3();
    showStatus();
    createMenuBar();
    populateSportFilter();
    loadSettings();

    applyFilter();

    connect(ui->comboLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        translateInterface(index);
    });

    connect(ui->leSearch, &QLineEdit::textChanged, this, &MainWindow::on_filterChanged);
    connect(ui->comboSportFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_filterChanged);
    connect(ui->rbMale, &QRadioButton::toggled, this, &MainWindow::on_filterChanged);
    connect(ui->rbFemale, &QRadioButton::toggled, this, &MainWindow::on_filterChanged);

    connect(ui->lineEdit_4, &QLineEdit::textChanged, this, [this]() {
        applyFilterLessons();
    });
    connect(ui->comboBox_3, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        applyFilterLessons();
    });

}

void MainWindow::loadTrainersToTable()
{
    ui->tableWidget->setRowCount(0);

    QStringList headers;
    headers << "ID_Тренера" << "Ім'я" << "Номер телефону" << "ID_Виду спорту" << "Стать";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlQuery query;
    if (!query.exec("SELECT ID_Coach, Name, PhoneNumber, ID_Service, Sex FROM Coach")) {
        qDebug() << "ПОМИЛКА БД: запит не вдався!" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {



        ui->tableWidget->insertRow(row);

        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        QString phone = query.value(2).toString();
        QString category = query.value(3).toString();
        QString gender = query.value(4).toString();

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(phone));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(category));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(gender));

        ui->tableWidget->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 4)->setTextAlignment(Qt::AlignCenter);

        row++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::loadSportsToTable2()
{
    ui->tableWidget_2->setRowCount(0);

    QStringList headers;
    headers << "ID_Спорту" << "Назва" << "Ціна" << "ID_Тренера";
    ui->tableWidget_2->setColumnCount(headers.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);

    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlQuery query;
    if (!query.exec("SELECT ID_Service, Name, Price, ID_Coach FROM Service")) {
        qDebug() << "ПОМИЛКА БД: запит не вдався!" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {



        ui->tableWidget_2->insertRow(row);

        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        QString price = QString::number(query.value(2).toDouble(), 'f', 2);
        QString idCoach = query.value(3).toString();

        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(name));
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(price));
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(idCoach));

        ui->tableWidget_2->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        row++;
    }
    ui->tableWidget_2->resizeColumnsToContents();
}

void MainWindow::loadLessonsToTable3()
{
    ui->tableWidget_4->setRowCount(0);

    QStringList headers;
    headers << "ID Тренера" << "ПІБ Тренера" << "Вид спорту" << "Номер телефону" << "Стать";
    ui->tableWidget_4->setColumnCount(headers.size());
    ui->tableWidget_4->setHorizontalHeaderLabels(headers);

    ui->tableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlQuery query;
    QString sql = "SELECT ID_Coach, Name, "
                  "CASE ID_Service "
                  "  WHEN 1 THEN 'BodyBuilding' "
                  "  WHEN 2 THEN 'powerLifting' "
                  "  WHEN 3 THEN 'Boxing' "
                  "  ELSE 'Інше' "
                  "END, "
                  "PhoneNumber, Sex FROM Coach";

    if (!query.exec(sql)) {
        qDebug() << "ПОМИЛКА формування списку тренувань:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_4->insertRow(row);

        QString idCoach = query.value(0).toString();
        QString name = query.value(1).toString();
        QString sportName = query.value(2).toString();
        QString phone = query.value(3).toString();
        QString sex = query.value(4).toString();

        ui->tableWidget_4->setItem(row, 0, new QTableWidgetItem(idCoach));
        ui->tableWidget_4->setItem(row, 1, new QTableWidgetItem(name));
        ui->tableWidget_4->setItem(row, 2, new QTableWidgetItem(sportName));
        ui->tableWidget_4->setItem(row, 3, new QTableWidgetItem(phone));
        ui->tableWidget_4->setItem(row, 4, new QTableWidgetItem(sex));

        if (ui->tableWidget_4->item(row, 0)) ui->tableWidget_4->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        if (ui->tableWidget_4->item(row, 2)) ui->tableWidget_4->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        if (ui->tableWidget_4->item(row, 3)) ui->tableWidget_4->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        if (ui->tableWidget_4->item(row, 4)) ui->tableWidget_4->item(row, 4)->setTextAlignment(Qt::AlignCenter);

        row++;
    }
    ui->tableWidget_4->resizeColumnsToContents();
}

void MainWindow::statusBarCount()
{
    qDebug() << "statusBarCount() called";

    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM Coach");

    if(query.next())
    {
        int count = query.value(0).toInt();
        qDebug() << "Count =" << count;

        ui->statusBar->showMessage(
            "Кількість тренерів: " + QString::number(count)
            );
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}


void MainWindow::on_pushButton_clicked()
{
    loginPage *window = new loginPage;
    this->close();
    window->show();
}


void MainWindow::on_pushButton_5_clicked()
{
    doOrder *window = new doOrder;
    window ->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    this->close();
}

void MainWindow::showStatus()
{
    statusBar()->showMessage("Програма запущена");
}



void MainWindow::on_toolButton_clicked()
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Info");
    msgBox.setText("Кількість тренерів: 12");

    msgBox.setStyleSheet(
        "QMessageBox { background-color: #1A1A1F; }"
        "QLabel { color: #E3E3E6; font-size: 12pt; }"
        "QPushButton {"
        "background-color: #5B0606;"
        "color: white;"
        "padding: 5px 15px;"
        "}"
        );

    msgBox.exec();
}


void MainWindow::on_pushButton_7_clicked()
{
        QColor color = QColorDialog::getColor(QColor("#5B0606"), this, "Оберіть акцентний колір інтерфейсу");
        if (!color.isValid()) return;

        QString accentColor = color.name();
        QString accentHoverColor = color.lighter(115).name();

        this->setStyleSheet(
            QString(
                "QMainWindow, QWidget { background-color: #1A1A1F; color: #E3E3E6; font-family: 'Segoe UI', Arial, sans-serif; }"
                "QPushButton { background-color: %1; color: white; border: none; padding: 6px 16px; font-weight: bold; border-radius: 4px; }"
                "QPushButton:hover { background-color: %2; }"
                "QPushButton:checked { background-color: %2; border-left: 4px solid white; }"
                "QLineEdit, QComboBox, QTableWidget, QHeaderView::section { background-color: #25252A; color: #FFFFFF; border: 1px solid #333339; border-radius: 4px; padding: 4px; }"
                "QLineEdit:focus, QComboBox:on { border: 1px solid %1; }"
                "QTableWidget { gridline-color: #2D2D34; selection-background-color: %1; selection-color: white; }"
                "QHeaderView::section { background-color: #141419; font-weight: bold; border: none; border-bottom: 2px solid %1; padding: 6px; }"
                "QGroupBox { border: 1px solid #333339; border-radius: 6px; margin-top: 12px; padding-top: 12px; }"
                "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 5px; color: %1; font-weight: bold; }"
                "QStatusBar { background-color: #141419; color: #8E8E93; }"
                "QMenuBar { background-color: #141419; color: #E3E3E6; }"
                "QMenuBar::item:selected { background-color: %1; color: white; }"
                "QMenu { background-color: #141419; color: #E3E3E6; border: 1px solid #333339; }"
                "QMenu::item:selected { background-color: %1; color: white; }"
                ).arg(accentColor).arg(accentHoverColor)
            );

        ui->statusBar->setStyleSheet(QString("QStatusBar { background-color: #141419; color: %1; }").arg(accentColor));
        ui->menubar->setStyleSheet(
            QString(
                "QMenuBar { background-color: #141419; color: #E3E3E6; }"
                "QMenuBar::item:selected { background-color: %1; }"
                "QMenu { background-color: #141419; color: #E3E3E6; }"
                "QMenu::item:selected { background-color: %1; }"
                ).arg(accentColor)
            );
}


void MainWindow::createMenuBar()
{

        menuBar()->clear();

        QMenu *fileMenu = menuBar()->addMenu("Файл");

        QAction *openAction = new QAction("Відкрити файл...", this);
        connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
        fileMenu->addAction(openAction);

        fileMenu->addSeparator();

        QAction *exitAction = new QAction("Вихід", this);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        fileMenu->addAction(exitAction);

        QMenu *helpMenu = menuBar()->addMenu("Довідка");

        QAction *aboutAction = new QAction("Про програму", this);
        connect(aboutAction, &QAction::triggered, this, &MainWindow::on_actionAbout_triggered);
        helpMenu->addAction(aboutAction);
}


void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Виберіть файл",
        "",
        "Усі файли (*.*)"
        );

    if(!fileName.isEmpty())
    {
        QMessageBox::information(
            this,
            "Файл обрано",
            "Файл:\n" + fileName
            );
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle("Про програму");
    aboutBox.setText("Gym Manager \"Сильний Барбос\"\n\n"
                     "Версія: 1.0\n"
                     "Розробник: Мартинов Іван\n"
                     "База даних: SQLite\n"
                     "Фреймворк: Qt 6\n\n"
                     "Система управління спортивним клубом.");
    aboutBox.setStyleSheet(
                "QMessageBox { background-color: #5B0606; }"
                "QLabel { color: #FFFFFF; font-size: 11pt; }"
                "QPushButton {"
                "  background-color: #141419;"
                "  color: white;"
                "  padding: 6px 20px;"
                "  font-weight: bold;"
                "  border-radius: 4px;"
                "  border: 1px solid #2D2D3A;"
                "}"
                "QPushButton:hover { background-color: #252530; }"
        );

    aboutBox.exec();
}

void MainWindow::populateSportFilter()
{
    ui->comboSportFilter->clear();

    ui->comboSportFilter->addItem("Усі", 0);

    QSqlQuery query;

    if (query.exec("SELECT ID_Service, Name FROM Service ORDER BY ID_Service ASC")) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();

            ui->comboSportFilter->addItem(name, id);
        }
    } else {
        qDebug() << "ПОМИЛКА завантаження видів спорту для фільтра:" << query.lastError().text();
    }
}

void MainWindow::applyFilter()
{
    ui->tableWidget->setRowCount(0);

    QString sql = "SELECT ID_Coach, Name, PhoneNumber, ID_Service, Sex FROM Coach WHERE 1=1";
    QMap<QString, QVariant> bindValues;


    QString searchText = ui->leSearch->text().trimmed();
    if (!searchText.isEmpty()) {
        sql += " AND Name LIKE :name";
        bindValues[":name"] = "%" + searchText + "%";
    }

    int sportIndex = ui->comboSportFilter->currentIndex();
    if (sportIndex > 0) {
        sql += " AND ID_Service = :sportId";
        bindValues[":sportId"] = sportIndex;
    }


    if (ui->rbMale->isChecked()) {
        sql += " AND Sex = 'Male'";
    } else if (ui->rbFemale->isChecked()) {
        sql += " AND Sex = 'Female'";
    }

    QSqlQuery query;
    query.prepare(sql);

    QMap<QString, QVariant>::const_iterator i = bindValues.constBegin();
    while (i != bindValues.constEnd()) {
        query.bindValue(i.key(), i.value());
        ++i;
    }

    if (!query.exec()) {
        qDebug() << "ПОМИЛКА ФІЛЬТРАЦІЇ БД:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        QString phone = query.value(2).toString();
        QString category = query.value(3).toString();
        QString gender = query.value(4).toString();

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(phone));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(category));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(gender));

        ui->tableWidget->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(row, 4)->setTextAlignment(Qt::AlignCenter);

        row++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_filterChanged()
{
    applyFilter();
}

void MainWindow::translateInterface(int languageIndex)
{
    if (languageIndex == 1) {
        ui->btnTrainers->setText("Coaches");
        ui->btnSports->setText("Sports");
        ui->btnLessons->setText("Available Lessons");
        ui->btnSettings->setText("Settings");

        ui->labelWelcome->setText("Welcome to the system!");
        ui->groupBox->setTitle("Filter & Search");
        ui->labelSearchTrainer->setText("Search coach");
        ui->labelSex->setText("Gender");
        ui->rbMale->setText("Male");
        ui->rbFemale->setText("Female");
        ui->labelSport_2->setText("Sport type");

        QStringList coachesHeaders;
        coachesHeaders << "Coach ID" << "Name" << "Phone Number" << "Sport ID" << "Gender";
        ui->tableWidget->setHorizontalHeaderLabels(coachesHeaders);

        ui->labelSportClub->setText("Our Sports Directions");
        ui->groupBox_2->setTitle("Search Filter");
        ui->label_7->setText("Search direction");

        QStringList sportsHeaders;
        sportsHeaders << "Sport ID" << "Title" << "Price" << "Coach ID";
        ui->tableWidget_2->setHorizontalHeaderLabels(sportsHeaders);

        ui->labelAviableLessons->setText("Available Workouts & Coaches");
        ui->groupBox_4->setTitle("Search");
        ui->labelSearchTrainers->setText("Search by name");
        ui->labelSport->setText("Sport filter");

        QStringList lessonsHeaders;
        lessonsHeaders << "Coach ID" << "Full Name" << "Sport Direction" << "Phone Number" << "Gender";
        ui->tableWidget_4->setHorizontalHeaderLabels(lessonsHeaders);

        ui->labelSettings->setText("Settings");
        ui->labelInterface->setText("Interface");
        ui->labelLanguage->setText("Language");
        ui->labelTheme->setText("Theme");
        ui->pushButton_7->setText("Change Theme");

        ui->labelProfile->setText("Profile");
        ui->labelName->setText("Name");
        ui->labelPassword->setText("Change Password");

        ui->btnChangeName->setText("Change Name");
        ui->btnChangePassword->setText("Change Password");
        ui->btnExitAccount->setText("Log out of account");

        ui->pushButton_5->setText("Make an order");
        ui->pushButton_6->setText("Exit");

        statusBar()->showMessage("Program is running smoothly");

    }
    else {
        ui->btnTrainers->setText("Тренери");
        ui->btnSports->setText("Види спорта");
        ui->btnLessons->setText("Доступні тренування");
        ui->btnSettings->setText("Налаштування");

        ui->labelWelcome->setText("Вітаємо в системі!");
        ui->groupBox->setTitle("Фільтр та пошук");
        ui->labelSearchTrainer->setText("Пошук тренера");
        ui->labelSex->setText("Стать");
        ui->rbMale->setText("Чоловіча");
        ui->rbFemale->setText("Жіноча");
        ui->labelSport_2->setText("Вид спорту");

        QStringList coachesHeaders;
        coachesHeaders << "ID_Тренера" << "Ім'я" << "Номер телефону" << "ID_Виду спорту" << "Стать";
        ui->tableWidget->setHorizontalHeaderLabels(coachesHeaders);

        ui->labelSportClub->setText("Наші спортивні напрямки");
        ui->groupBox_2->setTitle("Фільтрація пошуку");
        ui->label_7->setText("Пошук напрямку");

        QStringList sportsHeaders;
        sportsHeaders << "ID_Спорту" << "Назва" << "Ціна" << "ID_Тренера";
        ui->tableWidget_2->setHorizontalHeaderLabels(sportsHeaders);

        ui->labelAviableLessons->setText("Доступні тренування та тренери");
        ui->groupBox_4->setTitle("Пошук");
        ui->labelSearchTrainers->setText("Пошук за ім'ям");
        ui->labelSport->setText("Фільтр спорту");

        QStringList lessonsHeaders;
        lessonsHeaders << "ID Тренера" << "ПІБ Тренера" << "Вид спорту" << "Номер телефону" << "Стать";
        ui->tableWidget_4->setHorizontalHeaderLabels(lessonsHeaders);

        ui->labelSettings->setText("Налаштування");
        ui->labelInterface->setText("Інтерфейс");
        ui->labelLanguage->setText("Мова");
        ui->labelTheme->setText("Тема");
        ui->pushButton_7->setText("Змінити фон");

        ui->labelProfile->setText("Профіль");
        ui->labelName->setText("Ім'я");
        ui->labelPassword->setText("Змінити пароль");

        ui->btnChangeName->setText("Змінити ім'я");
        ui->btnChangePassword->setText("Змінити пароль");
        ui->btnExitAccount->setText("Увійти з облікового запису");

        ui->pushButton_5->setText("Зробити замовлення");
        ui->pushButton_6->setText("Вихід");

        statusBar()->showMessage("Програма запущена успішно");
    }

    createMenuBar();

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_4->resizeColumnsToContents();

    QSettings settings("SportClubStrongBarbos", "GymManager");
    settings.setValue("language", languageIndex);
}

void MainWindow::populateSportFilter2()
{
    ui->comboSportFilter->clear();
    ui->comboBox_3->clear();

    QString allText = (ui->comboLanguage->currentIndex() == 1) ? "All" : "Усі";
    ui->comboSportFilter->addItem(allText, 0);
    ui->comboBox_3->addItem(allText, 0);

    QSqlQuery query;
    if (query.exec("SELECT ID_Service, Name FROM Service ORDER BY ID_Service ASC")) {
        while (query.next()) {
            QString name = query.value(1).toString();
            int id = query.value(0).toInt();

            ui->comboSportFilter->addItem(name, id);
            ui->comboBox_3->addItem(name, id);
        }
    } else {
        qDebug() << "ПОМИЛКА завантаження видів спорту для фільтрів:" << query.lastError().text();
    }
}

void MainWindow::applyFilterLessons()
{
    ui->tableWidget_4->setRowCount(0);

    QString sql = "SELECT ID_Coach, Name, "
                  "CASE ID_Service "
                  "  WHEN 1 THEN 'BodyBuilding' "
                  "  WHEN 2 THEN 'powerLifting' "
                  "  WHEN 3 THEN 'Boxing' "
                  "  ELSE 'Інше' "
                  "END, "
                  "PhoneNumber, Sex FROM Coach WHERE 1=1";

    QMap<QString, QVariant> bindValues;

    QString searchText = ui->lineEdit_4->text().trimmed();
    if (!searchText.isEmpty()) {
        sql += " AND Name LIKE :name";
        bindValues[":name"] = "%" + searchText + "%";
    }

    int sportIndex = ui->comboBox_3->currentIndex();
    if (sportIndex > 0) {
        sql += " AND ID_Service = :sportId";
        bindValues[":sportId"] = ui->comboBox_3->currentData();
    }

    QSqlQuery query;
    query.prepare(sql);

    QMap<QString, QVariant>::const_iterator i = bindValues.constBegin();
    while (i != bindValues.constEnd()) {
        query.bindValue(i.key(), i.value());
        ++i;
    }

    if (!query.exec()) {
        qDebug() << "ПОМИЛКА ФІЛЬТРАЦІЇ ТРЕНУВАНЬ:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_4->insertRow(row);
        for(int k = 0; k < 5; ++k) {
            ui->tableWidget_4->setItem(row, k, new QTableWidgetItem(query.value(k).toString()));
            if(k != 1) {
                ui->tableWidget_4->item(row, k)->setTextAlignment(Qt::AlignCenter);
            }
        }
        row++;
    }
    ui->tableWidget_4->resizeColumnsToContents();
}

void MainWindow::loadSportsToComboBox()
{
    ui->comboBox_3->clear();

    ui->comboBox_3->addItem("Усі", 0);

    QSqlQuery query;
    if (query.exec("SELECT ID_Service, Name FROM Service ORDER BY ID_Service ASC")) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();

            ui->comboBox_3->addItem(name, id);
        }
    } else {
        qDebug() << "Помилка завантаження даних у comboBox_3:" << query.lastError().text();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("SportClubStrongBarbos", "GymManager");

    int langIndex = settings.value("language", 0).toInt();

    ui->comboLanguage->blockSignals(true);
    ui->comboLanguage->setCurrentIndex(langIndex);
    ui->comboLanguage->blockSignals(false);

    translateInterface(langIndex);
}
void MainWindow::on_pushButton_8_clicked()
{
    if (currentUserSession != nullptr) {
        QString currentLogin = currentUserSession->getUsername();

        if (globalSessionManager.isAdminInVector(currentLogin)) {
            QMessageBox successBox(this);
            successBox.setWindowTitle("Доступ дозволено");
            successBox.setText("Успішно авторизовано! Обліковий запис '" + currentLogin + "' знайдено у векторі адміністраторів.");
            successBox.setIcon(QMessageBox::Information);

            successBox.setStyleSheet(
                "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
                "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI', sans-serif; font-size: 11pt; }"
                "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 70px; }"
                "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
                );
            successBox.exec();

            forAdministrator *window = new forAdministrator();
            window->show();
            this->close();
        }else{
            QMessageBox errorBox(this);
            errorBox.setWindowTitle("Відмовлено в доступі");
            errorBox.setText("Помилка доступу! Обліковий запис '" + currentLogin + "' відсутній у списку адміністраторів клубу.");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setStyleSheet(
                "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
                "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI', sans-serif; font-size: 11pt; }"
                "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 70px; }"
                "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
                );
            errorBox.exec();
        }
    } else {

        QMessageBox warningBox(this);
        warningBox.setStyleSheet(
            "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
            "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI', sans-serif; font-size: 11pt; }"
            "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 70px; }"
            "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
            );
        warningBox.setWindowTitle("Помилка сесії");
        warningBox.setText("Помилка: Користувач не розпізнаний. Перезайдіть у систему.");
        warningBox.setIcon(QMessageBox::Warning);

        warningBox.exec();
    }
}


void MainWindow::on_btnChangePassword_clicked()
{
    if (currentUserSession == nullptr) {
        QMessageBox::warning(this, "Помилка сесії", "Користувач не розпізнаний. Перезайдіть у систему.");
        return;
    }

    QString newPassword = ui->leNewPassword->text().trimmed();

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Поле пароля не може бути порожнім!");
        return;
    }

    if (newPassword.length() < 4) {
        QMessageBox::warning(this, "Помилка", "Пароль має бути не коротшим за 4 символи!");
        return;
    }

    QString currentLogin = currentUserSession->getUsername();
    QString currentRole = currentUserSession->getRole();

    QSqlQuery query;
    bool querySuccess = false;

    if (currentRole == "Адміністратор") {
        query.prepare("UPDATE Administrator SET Password = :newPassword WHERE Login = :login");
    } else {
        query.prepare("UPDATE User SET Password = :newPassword WHERE Login = :login");
    }

    query.bindValue(":newPassword", newPassword);
    query.bindValue(":login", currentLogin);

    querySuccess = query.exec();

    if (querySuccess) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Успіх");
        msgBox.setText("Пароль для облікового запису '" + currentLogin + "' успішно змінено в базі даних!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
            "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI'; font-size: 11pt; }"
            "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 70px; }"
            "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
            );
        msgBox.exec();

        ui->leNewPassword->clear();
    } else {
        QMessageBox::critical(this, "Помилка БД", "Не вдалося оновити пароль.\nПричина: " + query.lastError().text());
    }
}


void MainWindow::on_btnExitAccount_clicked()
{
    loginPage *window = new loginPage;
    this->close();
    window->show();
}


void MainWindow::on_btnChangeName_clicked()
{
    if (currentUserSession == nullptr) {
        QMessageBox::warning(this, "Помилка сесії", "Користувач не розпізнаний. Перезайдіть у систему.");
        return;
    }

    QString newLogin = ui->leNewLogin->text().trimmed();

    if (newLogin.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Поле логіну не може бути порожнім!");
        return;
    }

    QString oldLogin = currentUserSession->getUsername();
    QString currentRole = currentUserSession->getRole();

    if (newLogin == oldLogin) {
        QMessageBox::information(this, "Інформація", "Ви вже використовуєте цей логін.");
        return;
    }

    QSqlQuery query;

    if (currentRole == "Адміністратор") {
        query.prepare("SELECT COUNT(*) FROM Administrator WHERE Login = :newLogin");
    } else {
        query.prepare("SELECT COUNT(*) FROM User WHERE Login = :newLogin");
    }

    query.bindValue(":newLogin", newLogin);

    if (query.exec() && query.next()) {
        if (query.value(0).toInt() > 0) {
            QMessageBox::critical(this, "Помилка", "Користувач з логіном '" + newLogin + "' вже існує в системі! Оберіть інше ім'я.");
            return;
        }
    }

    if (currentRole == "Адміністратор") {
        query.prepare("UPDATE Administrator SET Login = :newLogin WHERE Login = :oldLogin");
    } else {
        query.prepare("UPDATE User SET Login = :newLogin WHERE Login = :oldLogin");
    }

    query.bindValue(":newLogin", newLogin);
    query.bindValue(":oldLogin", oldLogin);

    if (query.exec()) {

        currentUserSession->setUsername(newLogin);

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Успіх");
        msgBox.setText("Логін успішно змінено з '" + oldLogin + "' на '" + newLogin + "'!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #1A1A1F; border: 1px solid #5B0606; }"
            "QMessageBox QLabel { color: #E3E3E6; font-family: 'Segoe UI'; font-size: 11pt; }"
            "QMessageBox QPushButton { background-color: #5B0606; color: #FFFFFF; border-radius: 4px; padding: 5px 15px; font-weight: bold; min-width: 70px; }"
            "QMessageBox QPushButton:hover { background-color: #9E1A1A; }"
            );
        msgBox.exec();

        ui->leNewLogin->clear();
    } else {
        QMessageBox::critical(this, "Помилка БД", "Не вдалося оновити логін.\nПричина: " + query.lastError().text());
    }
}

