#ifndef CANVAS_H
#define CANVAS_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    void updateBuffer(QPixmap* buffer);

// public slots:

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap* storedBuffer;
};

#endif // CANVAS_H
