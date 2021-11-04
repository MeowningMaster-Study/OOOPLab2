#ifndef DRAWING_H
#define DRAWING_H

#include <QObject>
#include <QSize>
#include <QPixmap>
#include <QPointF>

struct drawInfo {
    QStringList formulas;
    QSize size;
    qreal scale;
    QPointF offset;
};

class Drawing : public QObject
{
Q_OBJECT
public:
    explicit Drawing();

public slots:
    void draw(drawInfo);

signals:
    void sendPixmap(QPixmap);
};

#endif // DRAWING_H
