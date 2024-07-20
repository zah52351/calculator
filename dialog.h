#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void on_plainTextEdit_textChanged(int);

signals:

    void on_num0Button_clicked();

    void on_numCommaButton_clicked();

    void on_numEqualButton_clicked();

    void on_num1Button_clicked();

    void on_num2Button_clicked();

    void on_num3Button_clicked();

    void on_num4Button_clicked();

    void on_num5Button_clicked();

    void on_num6Button_clicked();

    void on_num7Button_clicked();

    void on_num8Button_clicked();

    void on_num9Button_clicked();

    void on_numPlusButton_clicked();

    void on_numMinusButton_clicked();

    void on_numMultiplyButton_clicked();

    void on_numDivideButton_clicked();

    void on_numPercentButton_clicked();

    void on_numSwitchMinusButton_clicked();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
