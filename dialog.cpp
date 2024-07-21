#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
    , sumInMemory(0.0), sumSoFar(0.0)
    , factorSoFar(0.0), waitingForOperand(true)
{
    //ui->setupUi(this);

    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);
    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = createButton(QString::number(i), &Dialog::digitClicked);

    Button* pointButton = createButton(tr("."), &Dialog::pointClicked);
    Button* changeSignButton = createButton(tr("\302\261"), &Dialog::changeSignClicked);

    Button* backspaceButton = createButton(tr("Backspace"), &Dialog::backspaceClicked);
    Button* clearButton = createButton(tr("Clear"), &Dialog::clear);
    Button* clearAllButton = createButton(tr("Clear All"), &Dialog::clearAll);

    Button* clearMemoryButton = createButton(tr("MC"), &Dialog::clearMemory);
    Button* readMemoryButton = createButton(tr("MR"), &Dialog::readMemory);
    Button* setMemoryButton = createButton(tr("MS"), &Dialog::setMemory);
    Button* addToMemoryButton = createButton(tr("M+"), &Dialog::addToMemory);

    Button* divisionButton = createButton(tr("\303\267"), &Dialog::multiplicativeOperatorClicked);
    Button* timesButton = createButton(tr("\303\227"), &Dialog::multiplicativeOperatorClicked);
    Button* minusButton = createButton(tr("-"), &Dialog::additiveOperatorClicked);
    Button* plusButton = createButton(tr("+"), &Dialog::additiveOperatorClicked);

    Button* squareRootButton = createButton(tr("Sqrt"), &Dialog::unaryOperatorClicked);
    Button* powerButton = createButton(tr("x\302\262"), &Dialog::unaryOperatorClicked);
    Button* reciprocalButton = createButton(tr("1/x"), &Dialog::unaryOperatorClicked);
    Button* equalButton = createButton(tr("="), &Dialog::equalClicked);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 6);
    mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
    mainLayout->addWidget(clearButton, 1, 2, 1, 2);
    mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);

    mainLayout->addWidget(clearMemoryButton, 2, 0);
    mainLayout->addWidget(readMemoryButton, 3, 0);
    mainLayout->addWidget(setMemoryButton, 4, 0);
    mainLayout->addWidget(addToMemoryButton, 5, 0);

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    mainLayout->addWidget(digitButtons[0], 5, 1);
    mainLayout->addWidget(pointButton, 5, 2);
    mainLayout->addWidget(changeSignButton, 5, 3);

    mainLayout->addWidget(divisionButton, 2, 4);
    mainLayout->addWidget(timesButton, 3, 4);
    mainLayout->addWidget(minusButton, 4, 4);
    mainLayout->addWidget(plusButton, 5, 4);

    mainLayout->addWidget(squareRootButton, 2, 5);
    mainLayout->addWidget(powerButton, 3, 5);
    mainLayout->addWidget(reciprocalButton, 4, 5);
    mainLayout->addWidget(equalButton, 5, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("Calculator"));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::digitClicked()
{
    Button* clickedButton = qobject_cast<Button*>(sender());
    int digitValue = clickedButton->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->text() + QString::number(digitValue));
}

void Dialog::unaryOperatorClicked()
{
    Button* clickedButton = qobject_cast<Button*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;

    if (clickedOperator == tr("Sqrt")) {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        result = std::sqrt(operand);
    }
    else if (clickedOperator == tr("x\302\262")) {
        result = std::pow(operand, 2.0);
    }
    else if (clickedOperator == tr("1/x")) {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }
    display->setText(QString::number(result));
    waitingForOperand = true;
}

void Dialog::additiveOperatorClicked()
{
    Button* clickedButton = qobject_cast<Button*>(sender());
    if (!clickedButton)
        return;
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        display->setText(QString::number(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        display->setText(QString::number(sumSoFar));
    }
    else {
        sumSoFar = operand;
    }
    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Dialog::multiplicativeOperatorClicked()
{
    Button* clickedButton = qobject_cast<Button*>(sender());
    if (!clickedButton)
        return;
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        display->setText(QString::number(factorSoFar));
    }
    else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

void Dialog::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    }
    else {
        sumSoFar = operand;
    }

    display->setText(QString::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}

void Dialog::pointClicked()
{
    if (waitingForOperand)
        display->setText("0");
    if (!display->text().contains('.'))
        display->setText(display->text() + tr("."));
    waitingForOperand = false;
}

void Dialog::changeSignClicked()
{
    QString text = display->text();
    double value = text.toDouble();

    if (value > 0.0) {
        text.prepend(tr("-"));
    }
    else if (value < 0.0) {
        text.remove(0, 1);
    }
    display->setText(text);
}

void Dialog::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}

void Dialog::clear()
{
    if (waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}

void Dialog::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void Dialog::clearMemory()
{
    sumInMemory = 0.0;
}

void Dialog::readMemory()
{
    display->setText(QString::number(sumInMemory));
    waitingForOperand = true;
}

void Dialog::setMemory()
{
    equalClicked();
    sumInMemory = display->text().toDouble();
}

void Dialog::addToMemory()
{
    equalClicked();
    sumInMemory += display->text().toDouble();
}

template<typename PointerToMemberFunction>
Button* Dialog::createButton(const QString& text, const PointerToMemberFunction& member)
{
    Button* button = new Button(text);
    connect(button, &Button::clicked, this, member);
    return button;
}

void Dialog::abortOperation()
{
    clearAll();
    display->setText(tr("####"));
}

bool Dialog::calculate(double rightOperand, const QString& pendingOperator)
{
    if (pendingOperator == tr("+")) {
        sumSoFar += rightOperand;
    }
    else if (pendingOperator == tr("-")) {
        sumSoFar -= rightOperand;
    }
    else if (pendingOperator == tr("\303\227")) {
        factorSoFar *= rightOperand;
    }
    else if (pendingOperator == tr("\303\267")) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar /= rightOperand;
    }
    return true;
}
