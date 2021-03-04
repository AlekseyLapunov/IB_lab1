#include "KardanoManager.h"

#include <QRandomGenerator>
#include <QDebug>
#include <fstream>

KardanoManager::KardanoManager()
{
    matrixPtr = nullptr;
}

KardanoManager::~KardanoManager()
{
}

bool KardanoManager::createKardano(int sideSize)
{
    bool state = false;
    this->doMalloc(sideSize);
    matrixPtr = fillMatrix(sideSize, matrixPtr);
    sendToFile(numbers, sideSize, matrixPtr);
    return state = true;
}

void KardanoManager::fillKardano(QString inputStr, int sideSize)
{
    // Количество выборок из малой решётки
    int k = sideSize*sideSize / 16;
    // Ставим метки в каждую малую решётку Кардано
    this->markThem(k, sideSize, matrixPtr);
    // По меткам расставляем буквы строки
    this->marksToSymbols(inputStr, sideSize, matrixPtr);
    // Отправляем в текстовый файл заполненную символами карту Кардано
    this->sendToFile(chars, sideSize, matrixPtr);
}

void KardanoManager::fillKardano(QString inputStr, int sideSize, QString fileName)
{
    // Последовательно расставляем символы строки в карту Кардано
    this->fromInputToKardano(inputStr, sideSize);
    // Отправляем в текстовый файл заполненную символами карту Кардано
    this->sendToFile(chars_d, sideSize, matrixPtr);
    // Ставим метки согласно приложенному ключу
    this->markThem(sideSize, fileName, matrixPtr);
}

QString KardanoManager::giveCodedString(int sideSize)
{
    QString codedAnswer = "";
    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
            codedAnswer += matrixPtr[i][j].symbol;
    clearDynArrays();
    return codedAnswer;
}

QString KardanoManager::giveDecodedString(int sideSize)
{
    QString decodedAnswer = "";
    for(int k = 0; k < 4; k++)
        for(int i = 0; i < sideSize; i++)
            for(int j = 0; j < sideSize; j++)
                if(matrixPtr[i][j].marked == k+1) decodedAnswer += matrixPtr[i][j].symbol;
    return decodedAnswer;
}

KardanoManager::kardanoGrid **KardanoManager::fillMatrix(int sz, kardanoGrid **input)
{
    // Четверти не координатные
    // I четверть
    int k = 1;
    for (int i = 0; i < sz/2; i++)
        for (int j = 0; j < sz/2; j++)
        {
            input[i][j].number = k;
            input[i][j].marked = 0;
            input[i][j].symbol = '?';
            k++;
        }
    // II четверть
    k = 1;
    for (int j = sz-1; j >= sz / 2; j--)
        for (int i = 0; i < sz / 2; i++)
        {
            input[i][j].number = k;
            input[i][j].marked = 0;
            input[i][j].symbol = '?';
            k++;
        }
    // III четверть
    k = 1;
    for (int j = 0; j < sz / 2; j++)
        for (int i = sz - 1; i >= sz / 2; i--)
        {
            input[i][j].number = k;
            input[i][j].marked = 0;
            input[i][j].symbol = '?';
            k++;
        }
    // IV четверть
    k = 1;
    for (int i = sz - 1; i >= sz / 2; i--)
        for (int j = sz - 1; j >= sz / 2; j--)
        {
            input[i][j].number = k;
            input[i][j].marked = 0;
            input[i][j].symbol = '?';
            k++;
        }

    return input;
}

void KardanoManager::sendToFile(int flag, int sideSize, KardanoManager::kardanoGrid **input)
{
    QDir here;
    if(!here.exists("kardanos"))
    here.mkdir("kardanos");
    here.mkdir("kardanos/debug_forms");

    QString definedByFlag = defineNameByFlag(flag);
    QFile output("kardanos/debug_forms/" + definedByFlag);
    fillByFlag(flag, sideSize, output, input);

}

QString KardanoManager::defineNameByFlag(int flag)
{
    if(flag == pattern) return "kardano_pattern.txt";
    if(flag == numbers) return "kardano_numbers.txt";
    if(flag == chars)   return "kardano_chars.txt";
    if(flag == rotate1) return "kardano_rotate1.txt";
    if(flag == rotate2) return "kardano_rotate2.txt";
    if(flag == rotate3) return "kardano_rotate3.txt";
    if(flag == chars_d) return "kardano_chars_d.txt";
    return "false_promise.txt";
}

void KardanoManager::fillByFlag(int flag, int sideSize, QFile &file, KardanoManager::kardanoGrid **input)
{
    if (file.open(QIODevice::WriteOnly))
    {
            QTextStream textStream(&file);
            for(int i = 0; i < sideSize; i++)
            {
                for(int j = 0; j < sideSize; j++)
                {
                    if(flag == pattern || flag == rotate1 || flag == rotate2 || flag == rotate3)
                    {
                        textStream << input[i][j].marked;
                    }
                    if(flag == numbers)
                    {
                        textStream << input[i][j].number;
                    }
                    if(flag == chars || flag == chars_d)
                    {
                        textStream << input[i][j].symbol;
                    }
                    if(j != sideSize - 1) textStream << " ";
                }
                if(i != sideSize - 1) textStream << Qt::endl;
            }
            file.close();
    }
}

void KardanoManager::markThem(int k, int sideSize, KardanoManager::kardanoGrid **input)
{
    markedTempo = new int*[sideSize];
    for(int i = 0; i < sideSize; i++)
        markedTempo[i] = new int[sideSize];

    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
            markedTempo[i][j] = 0;

    int *banned = new int[(sideSize/2)*(sideSize/2)], currentBanIndex = -1;
    for(int i = 0; i < (sideSize/2)*(sideSize/2); i++)
        banned[i] = 0;

    for(int small = 0; small < 4; small++)
    {
        for(int i = 0; i < k; i++)
        {
            currentBanIndex++;
            int picked = this->pickNumber(banned, sideSize, currentBanIndex);
            this->findAndMark(picked, sideSize, small, input);
            if((((sideSize/2)*(sideSize/2))%2 != 0) && (i == k - 1) && (small == 3))
            {
                currentBanIndex++;
                int picked = this->pickNumber(banned, sideSize, currentBanIndex);
                this->findAndMark(picked, sideSize, small, input);

            }
            /// qDebug() << banned[currentBanIndex] << " ";
        }
    }
    // Отправляем в текстовый файл первую помеченную форму
    this->sendToFile(pattern, sideSize, matrixPtr);
    // Сбрасываем ключ шифрования
    this->dropKey(sideSize);
    doRotate(sideSize, markedTempo);
    findAndIncrement(1, sideSize);
    sumMatrix(sideSize);
    // Отправляем в текстовый файл форму после первого поворота
    this->sendToFile(rotate1, sideSize, matrixPtr);
    doRotate(sideSize, markedTempo);
    findAndIncrement(2, sideSize);
    sumMatrix(sideSize);
    // Отправляем в текстовый файл форму после второго поворота
    this->sendToFile(rotate2, sideSize, matrixPtr);
    doRotate(sideSize, markedTempo);
    findAndIncrement(3, sideSize);
    sumMatrix(sideSize);
    // Отправляем в текстовый файл форму после третьего поворота
    this->sendToFile(rotate3, sideSize, matrixPtr);
}

bool KardanoManager::foundInBanned(int number, int *array, int sideSize)
{
    for(int i = 0; i < (sideSize/2)*(sideSize/2); i++)
        if(array[i] == number)
            return true;
    return false;
}

int KardanoManager::pickNumber(int *array, int sideSize, int currentBanIndex)
{
    int chosen = 0;
    do
    {
        chosen = QRandomGenerator::global()->bounded((sideSize/2)*(sideSize/2))+1;
    } while(this->foundInBanned(chosen, array, sideSize));
    array[currentBanIndex] = chosen;
    return chosen;
}

void KardanoManager::findAndMark(int number, int sideSize, int small, KardanoManager::kardanoGrid **input)
{
    int toSmallIndex = -1;
    for(int i = 0; i < sideSize; i++)
    {
        for(int j = 0; j < sideSize; j++)
        {
            if(input[i][j].number == number) toSmallIndex++;
            if(toSmallIndex == small)
            {
                input[i][j].marked = 1;
                markedTempo[i][j] = 1;
                return;
            }
        }
    }
}

void KardanoManager::marksToSymbols(QString inputStr, int sideSize, KardanoManager::kardanoGrid **input)
{
    int strIndex = 0;
    for(int i = 0; i < sideSize; i++)
    {
        for(int j = 0; j < sideSize; j++)
        {
            if(input[i][j].marked == 1)
            {
                input[i][j].symbol = inputStr[strIndex];
                strIndex++;
            }
        }
    }
    for(int i = 0; i < sideSize; i++)
    {
        for(int j = 0; j < sideSize; j++)
        {
            if(input[i][j].marked == 2)
            {
                input[i][j].symbol = inputStr[strIndex];
                strIndex++;
            }
        }
    }
    for(int i = 0; i < sideSize; i++)
    {
        for(int j = 0; j < sideSize; j++)
        {
            if(input[i][j].marked == 3)
            {
                input[i][j].symbol = inputStr[strIndex];
                strIndex++;
            }
        }
    }
    for(int i = 0; i < sideSize; i++)
    {
        for(int j = 0; j < sideSize; j++)
        {
            if(input[i][j].marked == 4)
            {
                input[i][j].symbol = inputStr[strIndex];
                strIndex++;
            }
        }
    }
}

void KardanoManager::doRotate(int sideSize, int **input)
{
    for (int i = 0; i < sideSize / 2; i++)
    {
        for (int j = i; j < sideSize - i - 1; j++)
        {
            int temp = input[i][j];
            input[i][j] = input[sideSize - 1 - j][i];
            input[sideSize - 1 - j][i] = input[sideSize - 1 - i][sideSize - 1 - j];
            input[sideSize - 1 - i][sideSize - 1 - j] = input[j][sideSize - 1 - i];
            input[j][sideSize - 1 - i] = temp;
        }
    }
}

void KardanoManager::fromInputToKardano(QString inputStr, int sideSize)
{
    int k = 0;
    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
        {
            matrixPtr[i][j].symbol = inputStr[k];
            k++;
        }
}

void KardanoManager::markThem(int sideSize, QString fileName, KardanoManager::kardanoGrid **input)
{
    markedTempo = new int*[sideSize];
    for(int i = 0; i < sideSize; i++)
        markedTempo[i] = new int[sideSize];

    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
            markedTempo[i][j] = 0;


    QFile from(fileName);
    if(from.open(QIODevice::ReadOnly))
    {
        QTextStream fromStream(&from);
        QString gotString = from.readLine();
        if(gotString.size() == sideSize*sideSize)
        {
            int temp = 0;
            for(int i = 0; i < sideSize; i++)
                for(int j = 0; j < sideSize; j++)
                {
                    int value = gotString.at(temp).digitValue();
                    input[i][j].marked = value;
                    markedTempo[i][j] = value;
                    temp++;
                }
        }
        from.close();

        // Отправляем в текстовый файл первую помеченную форму
        this->sendToFile(pattern, sideSize, input);

        doRotate(sideSize, markedTempo);
        findAndIncrement(1, sideSize);
        sumMatrix(sideSize);
        // Отправляем в текстовый файл форму после первого поворота
        this->sendToFile(rotate1, sideSize, matrixPtr);
        doRotate(sideSize, markedTempo);
        findAndIncrement(2, sideSize);
        sumMatrix(sideSize);
        // Отправляем в текстовый файл форму после второго поворота
        this->sendToFile(rotate2, sideSize, matrixPtr);
        doRotate(sideSize, markedTempo);
        findAndIncrement(3, sideSize);
        sumMatrix(sideSize);
        // Отправляем в текстовый файл форму после третьего поворота
        this->sendToFile(rotate3, sideSize, matrixPtr);
    }
}

void KardanoManager::sumMatrix(int sideSize)
{
    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
            matrixPtr[i][j].marked += markedTempo[i][j];
}

void KardanoManager::findAndIncrement(int exodus, int sideSize)
{
    for(int i = 0; i < sideSize; i++)
        for(int j = 0; j < sideSize; j++)
            if(markedTempo[i][j] == exodus) markedTempo[i][j] += 1;
}

void KardanoManager::clearDynArrays()
{
    delete[] matrixPtr;
    matrixPtr = nullptr;

    delete[] markedTempo;
    markedTempo = nullptr;
}

void KardanoManager::dropKey(int sideSize)
{
    QDir here;
    if(!here.exists("kardanos"))
    here.mkdir("kardanos");

    QFile temp("kardanos/key.txt");
    if (temp.open(QIODevice::WriteOnly))
    {
            QTextStream textStream(&temp);
            for(int i = 0; i < sideSize; i++)
            {
                for(int j = 0; j < sideSize; j++)
                {
                    textStream << matrixPtr[i][j].marked;
                }
            }
            temp.close();
    }
}

void KardanoManager::doMalloc(int sideSize)
{
    matrixPtr = new kardanoGrid*[sideSize];
    for(int i = 0; i < sideSize; i++)
        matrixPtr[i] = new kardanoGrid[sideSize];
}
