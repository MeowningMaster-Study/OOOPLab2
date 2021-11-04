#include "drawing.h"
#include <QRandomGenerator>
#include <QPixmap>
#include <QPainter>

Drawing::Drawing()
{
}

void Drawing::draw(drawInfo info) {
    auto rng = QRandomGenerator::global();

    QPixmap buffer(info.size);
    QPainter painter(&buffer);
    //painter.setRenderHint(QPainter::Antialiasing, true);

    QPoint p1(rng->bounded(500), rng->bounded(500)), p2;
    for (auto i = 0; i < 100; i++) {
        p2 = {rng->bounded(500), rng->bounded(500)};
        painter.drawLine(p1, p2);
        p1 = p2;
    }

    emit sendPixmap(buffer);
}

