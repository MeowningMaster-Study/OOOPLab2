#include "drawer.h"
#include <QThread>
#include <QRandomGenerator>
#include <QPainter>

QSize storedSize(50, 50);

Drawer::Drawer(){}

void Drawer::updateSize(QSize size) {
    storedSize = size;
}

void Drawer::draw(){
    auto rng = QRandomGenerator::global();

    while(true)
    {
        QPixmap buffer(storedSize);
        QPainter painter(&buffer);
        //painter.setRenderHint(QPainter::Antialiasing, true);
        QPoint p1(rng->bounded(500), rng->bounded(500)), p2;
        for (auto i = 0; i < 100; i++) {
            p2 = {rng->bounded(500), rng->bounded(500)};
            painter.drawLine(p1, p2);
            p1 = p2;
        }
        emit send(buffer);
    }
}
