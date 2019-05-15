#include "polynomewidget.h"

PolynomeWidget::PolynomeWidget(QWidget * parent) : QWidget(parent)
{
}

// Метод отрисовки полинома
void PolynomeWidget::paintEvent(QPaintEvent *) {
    if (pln.size() == 0) // Если полином пустой
        return;

    QPainter p(this); // Создаём новый объект рисовальщика
    p.setPen(QPen(Qt::black,1,Qt::SolidLine)); // Рисовать будем чёрной линией в 1 пиксель
    p.setBrush(Qt::white);           // Заливку будем делать сплошную белую
    p.drawRect(0,0,width() - 1,height() - 1); // Рисование линии

    bool has_min = false; // Есть ли отрицательные степени - при их наличии определяем границы по-другому

    // Ищем отрицательные степени
    for (size_t i = 0; i < pln.size(); i++)
    {
        if (pln[i].power < 0)
        {
            has_min = true;
            break;
        }
    }

    // Ищем максимальный модуль значения полинома
    double max_y = 0.0;
    for (double x = -10; x < 10; x += 0.1)
    {
        double y = fabs(pln(x));

        if (y > max_y)
            max_y = y;
    }

    // Если есть отрицательные степени, и значение получилось очень больше - ограничиваем
    if (has_min && (isnan(max_y) || max_y > 1000))
        max_y = 100;

    // Рисуем оси
    p.drawLine(5, height() / 2, width() - 5, height() / 2);  // OX
    p.drawLine(width() / 2, 5, width() / 2, height() - 5);   // OY
    p.drawLine(width() - 5, height() / 2, width() - 12, height() / 2 - 2); // стрелочки
    p.drawLine(width() - 5, height() / 2 + 1, width() - 12, height() / 2 + 3);
    p.drawLine(width() / 2, 5,  width() / 2 - 2, 12);
    p.drawLine(width() / 2 + 1, 5,  width() / 2 + 3, 12);
    p.drawText(width() - 12, height() / 2 - 12, "X");
    p.drawText(width() / 2 + 12, 12, "Y");
    p.drawText(width() / 2 - 12, height() / 2 + 12, "O");

    if (max_y == 0) // Для функции y = 0 больше ничего не рисуем
        return;

    // Коэффициенты пересчёта математических координат в экранные
    double kx = (width() / 20.0 - 0.5);
    double bx = width() * 0.5;
    double ky = (5 - height() * 0.5) / max_y;
    double by = height() * 0.5;

    for (int x = -9; x <= 9; x++)
        p.drawLine(x * kx + bx, height() / 2 + 5, x * kx + bx, height() / 2 - 5);

    p.drawText(5 * kx + bx, height() / 2 + 20, "5");
    p.drawText(-5 * kx + bx, height() / 2 + 20, "-5");

    int lg = (int)log10(max_y / 2);

    bool center = false;

    for (double y = 0; y < max_y; y += pow(10, lg))
    {
        p.drawLine(width() / 2 - 5, ky * y + by, width() / 2 + 5, ky * y + by);
        p.drawLine(width() / 2 - 5, -ky * y + by, width() / 2 + 5, -ky * y + by);

        if (!center && y >= max_y / 2)
        {
            p.drawText(width() / 2 + 12, ky * y + by - 5, QString::number(y));
            p.drawText(width() / 2 + 12, -ky * y + by - 5, QString::number(-y));
            center = true;
        }
    }

    p.setPen(QPen(Qt::red,2,Qt::SolidLine));

    double last_x = -10;
    double last_y = pln(-10);

    for (double x = -10.02; x < 10; x += 0.02)
    {
        double y = pln(x);
        if (fabs(y) <= max_y && fabs(last_y) <= max_y)
            p.drawLine(last_x * kx + bx, last_y * ky + by, x * kx + bx, y * ky + by);

        last_y = y;
        last_x = x;
    }
}

void PolynomeWidget::SetPolynome(Polynome value)
{
    pln = value;
    this->repaint(); // После смены полинома надо перерисоваться
}
