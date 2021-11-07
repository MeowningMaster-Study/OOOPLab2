#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddButtonClicked();
    void updateBuffer(QPixmap);
signals:
    void updateSize(QSize);
    void updateFormulas(QStringList);
    void updatePosition(QPoint offset, qreal scale);
    void start();
private:
    Ui::MainWindow *ui;
    void showEvent(QShowEvent*);
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent*);
protected:
    bool eventFilter(QObject*, QEvent*);
};
#endif // MAINWINDOW_H
