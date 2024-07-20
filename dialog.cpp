#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QObject::connect(ui->num0Button, &Dialog::on_num0Button_clicked
                     , ui->plainTextEdit, &Dialog::on_plainTextEdit_textChanged);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_plainTextEdit_textChanged(int number)
{
    ui->plainTextEdit->
}


void Dialog::on_num0Button_clicked()
{

}


void Dialog::on_numCommaButton_clicked()
{

}


void Dialog::on_numEqualButton_clicked()
{

}


void Dialog::on_num1Button_clicked()
{

}


void Dialog::on_num2Button_clicked()
{

}


void Dialog::on_num3Button_clicked()
{

}


void Dialog::on_num4Button_clicked()
{

}


void Dialog::on_num5Button_clicked()
{

}


void Dialog::on_num6Button_clicked()
{

}


void Dialog::on_num7Button_clicked()
{

}


void Dialog::on_num8Button_clicked()
{

}


void Dialog::on_num9Button_clicked()
{

}


void Dialog::on_numPlusButton_clicked()
{

}


void Dialog::on_numMinusButton_clicked()
{

}


void Dialog::on_numMultiplyButton_clicked()
{

}


void Dialog::on_numDivideButton_clicked()
{

}


void Dialog::on_numPercentButton_clicked()
{

}


void Dialog::on_numSwitchMinusButton_clicked()
{

}

