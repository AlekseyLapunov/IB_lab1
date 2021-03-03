#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "KardanoManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString transmitter; int rightValueStable;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //! Метод начинает выполнение кодирования или декодирования
    void begin();
    //! Метод осуществляет корректное завершение программы
    void quit();
    //! Метод отвечает за отображение информации о приложении
    void showAbout();
    //! Метод очищает текстовые поля
    void clear();
    //! Метод отвечает за копирование текста из поля в буфер обмена
    void copy();
    /*!
     Метод обновляет число, отображающее количество введённых
     символов, и число символов, необходимое для полного заполнения сетки Кардано.
     Дополнительно, метод контроллирует ограничение на ввод символов в 1024 символ.
    */
    void refreshHowMuch();
    //! Метод отвечает за включение / выключение текстового поля для ввода пути файла с решёткой Кардано
    void changed();

private:
    //! Метод определяет размер одной стороны решётки Кардано для заданного числа символов sCount
    int defineGridSize(int sCount);
    //! Метод отвечает за начало кодирования (вызывается методом begin())
    void doCoding(int sCount = 0);
    //! Метод отвечает за начало расшифровки (вызывается методом begin())
    void doDecoding(int sCount = 0);
    //! Метод дополняет случайными символами неполную строку
    QString fixString();

    KardanoManager kardanoManager;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
