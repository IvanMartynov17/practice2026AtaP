#ifndef DOORDER_H
#define DOORDER_H

#include <QMainWindow>

namespace Ui {
class doOrder;
}

class doOrder : public QMainWindow
{
    Q_OBJECT

public:
    explicit doOrder(QWidget *parent = nullptr);
    ~doOrder();

private slots:
    void on_pushButton_3_clicked();

    void on_btnPay_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::doOrder *ui;
    void loadServicesToComboBox();
    void loadCoachByService();
    void calculatePrice();
};

#endif // DOORDER_H
