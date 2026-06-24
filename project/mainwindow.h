#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_toolButton_clicked();

    void on_pushButton_7_clicked();

    void openFile();

    void on_actionAbout_triggered();

    void on_filterChanged();

    void translateInterface(int languageIndex);
    void on_pushButton_8_clicked();

    void on_btnChangePassword_clicked();

    void on_btnExitAccount_clicked();

    void on_btnChangeName_clicked();

private:
    Ui::MainWindow *ui;
    void loadTrainersToTable();
    void loadSportsToTable2();
    void loadLessonsToTable3();
    void statusBarCount();
    void showStatus();
    void createMenuBar();
    void applyFilter();
    void populateSportFilter();
    void populateSportFilter2();
    void applyFilterLessons();
    void loadSportsToComboBox();
    void loadSettings();
    QLabel *statusLabel;
};
#endif // MAINWINDOW_H
