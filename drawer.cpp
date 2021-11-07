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
        qreal scale = storedScale;
        qreal startPosX = (storedOffset.x() - halfWidth) * scale, posX;
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

        auto formulas = QVector<QString>(storedFormulas);
        for (QString& formula : formulas) {
            parser.compile(formula.toStdString(), expression);
            posX = startPosX;
            QPainterPath path;
            for (auto x = 0; x <= width; x++) {
                qreal posY = expression.value();
                int y = normalizedOffsetY - qFloor(posY / scale);
                QPoint p(x, y);

                if (x == 0) {
                    path.moveTo(p);
                } else {
                    path.lineTo(p);
                }
                posX += scale;
            }
            painter.drawPath(path);
        }

        emit send(buffer);
    }
}
