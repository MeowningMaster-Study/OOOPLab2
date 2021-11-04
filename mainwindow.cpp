#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawer.h"
#include <QPainter>
#include <QTimer>

QPainter* painter;
QStringList formulas;
qreal scale;
QPointF offset;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    QThread* thread = new QThread;
    Drawer* drawer = new Drawer();
    drawer->moveToThread(thread);
    connect(drawer, SIGNAL(send(QPixmap)), this, SLOT(updateBuffer(QPixmap)));
    connect(this, SIGNAL(start()), drawer, SLOT(draw()));
    connect(this, SIGNAL(updateSize(QSize)), drawer, SLOT(updateSize(QSize)), Qt::DirectConnection);
    thread->start();
    QTimer::singleShot(0, this, SLOT(start()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    emit updateSize(ui->canvas->size());
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    emit updateSize(ui->canvas->size());
}

void MainWindow::on_addButton_clicked()
{
    auto formula = ui->formulaEdit->text();
    ui->formulasListWidget->addItem(formula);
}

void MainWindow::updateBuffer(QPixmap pixmap) {
    QPixmap* copy = new QPixmap(pixmap);
    ui->canvas->updateBuffer(copy);
}
