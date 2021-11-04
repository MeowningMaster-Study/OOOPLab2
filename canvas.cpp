#include "canvas.h"

#include <QPainter>

QPixmap* storedBuffer;

Canvas::Canvas(QWidget *parent)
    : QWidget(parent), storedBuffer(new QPixmap(0, 0))
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Canvas::updateBuffer(QPixmap* buffer)
{
    delete storedBuffer;
    storedBuffer = buffer;
    update();
}

void Canvas::paintEvent(QPaintEvent* /*event*/)
{   
    QPainter painter(this);
    painter.drawPixmap(0, 0, *storedBuffer);
}
