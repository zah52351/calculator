#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Button.h"

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

    void digitClicked();
    void unaryOperatorClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();


private:
    Ui::Dialog *ui;
    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;
    QLineEdit* display;
    enum { NumDigitButtons = 10 };
    Button* digitButtons[NumDigitButtons];
    template<typename PointerToMemberFunction>
    Button* createButton(const QString& text, const PointerToMemberFunction& member);
    void abortOperation();
    bool calculate(double rightOperand, const QString& pendingOperator);
};
#endif // DIALOG_H
