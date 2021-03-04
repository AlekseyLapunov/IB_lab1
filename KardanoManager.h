#ifndef KARDANOMANAGER_H
#define KARDANOMANAGER_H

#include <QChar>
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QFile>

class KardanoManager
{
private:
    int **markedTempo;
    //! Структура ячейки решётки Кардано. Содержит номер, пометку и символ для ячейки
    struct kardanoGrid
    {
        int number;
        int marked;
        QChar symbol;
    };
    //! Квадратная матрица, элементы которой - ячейки решётки Кардано
    kardanoGrid **matrixPtr;

public:
    KardanoManager();
    ~KardanoManager();
    //! Метод создаёт решётку Кардано. Возвращает true в результате успеха
    bool createKardano(int sideSize);
    //! Метод заполняет решётку Кардано символами
    void fillKardano(QString inputStr, int sideSize);
    //! Метод заполняет решётку Кардано символами
    void fillKardano(QString inputStr, int sideSize, QString fileName);

    QString giveCodedString(int sideSize);

    QString giveDecodedString(int sideSize);

private:
    enum {pattern, numbers, chars, rotate1, rotate2, rotate3, chars_d};

    //! Метод выделяет память под решётку Кардано
    void doMalloc(int sideSize);
    //! Метод заполняет параметр ячеек "номер" числами от 0 до (sz/2)^2
    kardanoGrid **fillMatrix(int sz, kardanoGrid **input);
    //! Метод отправляет решётку Кардано в текстовый файл
    void sendToFile(int flag, int sideSize, kardanoGrid **input);
    //! Метод определяет имя файла по флагу
    QString defineNameByFlag(int flag);
    //! Метод заполняет текстовый файл согласно флагу
    void fillByFlag(int flag, int sideSize, QFile &, kardanoGrid **input);
    //! Метод расставляет метки в малые решётки Кардано
    void markThem(int k, int sideSize, kardanoGrid **input);
    //! Метод ищет выбранное число в массиве заблокированных чисел
    bool foundInBanned(int number, int *array, int sideSize);
    //! Метод выбирает случайное число, проверяя его на доступность
    int pickNumber(int *array, int sideSize, int currentBanIndex);
    //! Метод находит нужную клетку в карте Кардано и помечает её
    void findAndMark(int number, int sideSize, int small, KardanoManager::kardanoGrid **input);
    //! Метод сопоставляет меткам в решётке символы из входящей строки
    void marksToSymbols(QString inputStr, int sideSize, KardanoManager::kardanoGrid **input);
    //! Метод ставит метки, аналогичные повороту
    void doRotate(int sideSize, int **input);

    void fromInputToKardano(QString inputStr, int sideSize);
    //! Метод расставляет метки в малые решётки Кардано из файла fileName
    void markThem(int sideSize, QString fileName, KardanoManager::kardanoGrid **input);

    void sumMatrix(int sideSize);

    void findAndIncrement(int exodus, int sideSize);

    void clearDynArrays();

    void dropKey(int sideSize);
};

#endif // KARDANOMANAGER_H
