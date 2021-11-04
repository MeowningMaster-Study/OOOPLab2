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
signals:
    void send(QPixmap);
private:
    QSize storedSize;
};

#endif // DRAWER_H
