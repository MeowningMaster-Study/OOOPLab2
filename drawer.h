#ifndef DRAWER_H
#define DRAWER_H

#include <QObject>
#include <QPixmap>

class Drawer: public QObject
{
    Q_OBJECT
public:
    Drawer();
public slots:
    void draw();
    void updateSize(QSize);
    void updateFormulas(QStringList);
    void updatePosition(QPoint, qreal);
signals:
    void send(QPixmap);
private:
    QSize storedSize;
    QStringList storedFormulas;
    QPointF storedOffset;
    qreal storedScale;
};

#endif // DRAWER_H
