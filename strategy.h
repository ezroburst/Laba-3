#ifndef STRATEGY_H
#define STRATEGY_H
#pragma once
#include <QDir>
#include <QFileSystemModel>
#include <QStringList>
#include <QVector>


//структура для хранения названия(имя папки/файла или тип файла) и размера
struct Element
{
    QStringList elementName;
    QVector<double> elementSize;
    QVector<double> elementPro;
   // QVector<int> elemrntProzent;
};


//////////////////////////////////////////////////////
///// Иерархия классов, определяющая алгоритмы
class Strateg
{
public:
    virtual ~Strateg(){}
    virtual void UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element) = 0;
};

// Класс для использования
class strategy
{
private:
    Strateg* strateg;
    double size=0;
public:
    strategy(Strateg *strat): strateg(strat){}
    ~strategy(){delete strateg;}
  //  int sizeFolder ( QString path );
    void UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element)
    {
        strateg->UseStrategy(index, model, element);
    }
};

class folderSrtategy : public Strateg
{
public:
    void UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element&);
};

class fileTypeStrategy : public Strateg
{
public:
    void UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element&);

};

#endif // STRATEGY_H
