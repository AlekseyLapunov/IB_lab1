#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "QCoreApplication"
#include "QMessageBox"
#include "QClipboard"
#include "QRandomGenerator"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->changed();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::begin()
{
    if(ui->inputText->toPlainText().size() == 0)
    {
        QMessageBox warn(QMessageBox::Warning, "Warning", "There is no symbols in input text field.");
        warn.exec();
        return;
    }
    transmitter = fixString();
    if(ui->codingButton->isChecked())
        doCoding(transmitter.size());
    else doDecoding(transmitter.size());

    /*
    QMessageBox temp(QMessageBox::Information, "Hello", "Hello", QMessageBox::NoButton, this);
    temp.exec();
    */
}

void MainWindow::quit()
{
    QCoreApplication::quit();
}

void MainWindow::showAbout()
{
    QMessageBox aboutBox(this);
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.setWindowTitle("About program");
    aboutBox.setText("\"Kardano Grid Coder/Encoder\" is program that allows you to code some text<br>"
                     "using Kardano Grid method. <br>Also you can decode message by choosing such option"
                     " (mind to specify Kardano Grid file path).<br><br>"
                     "\"Kardano Grid Coder/Encoder\" program is created by Lapunov Alexey Yuryevich,<br>"
                     "SFU student; group - KI19-08b");
    aboutBox.exec();
}

void MainWindow::clear()
{
    if(!ui->inputText->toPlainText().isEmpty()
       || !ui->outputText->toPlainText().isEmpty())
        QApplication::beep();
    ui->inputText->clear();
    ui->outputText->clear();
    ui->howMuch->setText("0 / 16");
}

void MainWindow::copy()
{
    if(!ui->outputText->toPlainText().isEmpty())
    {
        QApplication::clipboard()->setText(ui->outputText->toPlainText());
        QMessageBox doneBox(QMessageBox::Information, "Success", "Copied!", QMessageBox::NoButton, this);
        doneBox.exec();
    }
}

int MainWindow::defineGridSize(int sCount)
{
    for (int i = 2; i < 16; i++)
    {
        int j = i + 1;
        if (sCount <= i * 2 * 2 * i) return  i*2;
        if (sCount > i * 2 * 2 * i && sCount <= j * 2 * 2 * j) return j*2;
    }
    return 0;
}

void MainWindow::doCoding(int sCount)
{
    int sideSize = this->defineGridSize(sCount);
    if(kardanoManager.createKardano(sideSize))
    {
        kardanoManager.fillKardano(transmitter, sideSize);
        ui->outputText->setPlainText(kardanoManager.giveCodedString(sideSize));
    }
}

void MainWindow::doDecoding(int sCount)
{
    QString filePath = ui->kardanoLine->text();
    int sideSize = this->defineGridSize(sCount);
    if(kardanoManager.createKardano(sideSize))
    {
        kardanoManager.fillKardano(transmitter, sideSize, filePath);
        ui->outputText->setPlainText(kardanoManager.giveDecodedString(sideSize));
    }
}

QString MainWindow::fixString()
{
    QString toChoose = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    toChoose += "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    toChoose += "0123456789";
    transmitter = ui->inputText->toPlainText();
    if(ui->inputText->toPlainText().size() != rightValueStable)
        while(transmitter.size() != rightValueStable)
        {
           int index = QRandomGenerator::global()->bounded(toChoose.size());
           transmitter += toChoose.at(index);
        }
    ui->outputText->setPlainText(transmitter);
    return transmitter;
}

void MainWindow::refreshHowMuch()
{
    if(ui->inputText->toPlainText().size() > 1024)
    {
        ui->beginButton->setDisabled(true);
        QString newString = ui->inputText->toPlainText();
        newString.remove(newString.size()-1, 1);
        ui->inputText->setPlainText(newString);
        ui->beginButton->setEnabled(true);
    }
    int leftValue = ui->inputText->toPlainText().size(),
        rightValue = this->defineGridSize(leftValue)*this->defineGridSize(leftValue);
    ui->howMuch->setText(QString::number(leftValue) + " / " + QString::number(rightValue));
    rightValueStable = rightValue;
}

void MainWindow::changed()
{
    if(ui->decodingButton->isChecked())
    {
        ui->kardanoLine->setVisible(true);
        ui->label_3->setVisible(true);
    }
    else
    {
        ui->kardanoLine->setVisible(false);
        ui->label_3->setVisible(false);
    }
}

