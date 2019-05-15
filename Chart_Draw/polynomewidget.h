#ifndef POLYNOMEWIDGET_H
#define POLYNOMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include "polynome.h"

// Виджет, рисующий графики функций
class PolynomeWidget: public QWidget
{
public:
    PolynomeWidget(QWidget * parent = 0);
    Polynome GetPolynome() { return pln; }
    void SetPolynome(Polynome value);

protected:
    void paintEvent(QPaintEvent *);

private:
    // Полином, который мы рисуем
    Polynome pln;
};

#endif // POLYNOMEWIDGET_H
