#include "drawer.h"
#include "exprtk.h"
#include <QThread>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>

Drawer::Drawer(){}

void Drawer::updateSize(QSize size) {
    storedSize = size;
    storedOffset = {0, 0};
    storedScale = 0.05;
}

void Drawer::updateFormulas(QStringList formulas) {
    storedFormulas = formulas;
}

void Drawer::updatePosition(QPoint offset, qreal scale) {
    storedOffset = offset;
    storedScale = scale;
}

void Drawer::draw(){
    while(true)
    {
        int width = storedSize.width(), height = storedSize.height();
        int halfWidth = width / 2;
        int halfHeight = height / 2;
        qreal scale = storedScale;
        qreal startPosX = (-storedOffset.x() - halfWidth) * scale, posX;
        int normalizedOffsetY = height / 2 + storedOffset.y();
        QPixmap buffer(storedSize);
        QPainter painter(&buffer);
        // painter.setRenderHint(QPainter::Antialiasing, true);

        exprtk::symbol_table<qreal> symbolTable;
        symbolTable.add_variable("x", posX);
        symbolTable.add_constants();
        exprtk::expression<qreal> expression;
        expression.register_symbol_table(symbolTable);
        exprtk::parser<qreal> parser;

        // draw axes
        QPoint center(halfWidth + storedOffset.x(), halfHeight + storedOffset.y());
        painter.drawLine(0, center.y(), width, center.y()); // x
        painter.drawLine(center.x(), 0, center.x(), height); // y

        // draw marks
        int tickHalf = 5;
        int distance = 20;
        for (int i = center.y() - distance; i >= 0; i -= distance) {
            painter.drawLine(center.x() - tickHalf, i, center.x() + tickHalf, i);
        }
        for (int i = center.y() + distance; i < height; i += distance) {
            painter.drawLine(center.x() - tickHalf, i, center.x() + tickHalf, i);
        }
        for (int i = center.x() - distance; i >= 0; i -= distance) {
            painter.drawLine(i, center.y() - tickHalf, i, center.y() + tickHalf);
        }
        for (int i = center.x() + distance; i < width; i += distance) {
            painter.drawLine(i, center.y() - tickHalf, i, center.y() + tickHalf);
        }

        // draw scale
        painter.drawText(QPoint(20, height - 20), QString::number(distance * scale));

        // draw graphs
        auto formulas = QStringList(storedFormulas);
        for (QString& formula : formulas) {
            parser.compile(formula.toStdString(), expression);
            posX = startPosX;
            QPainterPath path;
            qreal prevY = Q_QNAN;
            for (auto x = 0; x <= width; x++) {
                qreal posY = expression.value();
                if (!qIsNaN(posY)) {
                    int y = normalizedOffsetY - qFloor(posY / scale);
                    QPoint p(x, y);

                    if (qIsNaN(prevY)) {
                        path.moveTo(p);
                    } else {
                        path.lineTo(p);
                    }
                }
                posX += scale;
                prevY = posY;
            }
            painter.drawPath(path);
        }

        emit send(buffer);
    }
}
