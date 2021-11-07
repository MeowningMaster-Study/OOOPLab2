#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawer.h"
#include <QTimer>
#include <QCloseEvent>

QStringList formulas;
qreal scale;
QPoint offset;
QThread* drawerThread;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    drawerThread = new QThread;
    Drawer* drawer = new Drawer();
    drawer->moveToThread(drawerThread);
    connect(drawer, SIGNAL(send(QPixmap)), this, SLOT(updateBuffer(QPixmap)));
    connect(this, SIGNAL(start()), drawer, SLOT(draw()));
    connect(this, SIGNAL(updateSize(QSize)), drawer, SLOT(updateSize(QSize)), Qt::DirectConnection);
    connect(this, SIGNAL(updateFormulas(QStringList)), drawer, SLOT(updateFormulas(QStringList)), Qt::DirectConnection);
    //connect(this, SIGNAL(updatePosition(QPoint, qreal)), drawer, SLOT(updatePosition(QPoint, qreal)), Qt::DirectConnection);
    drawerThread->start();
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
    formulas.append(formula);
    emit updateFormulas(formulas.toList());
}

void MainWindow::updateBuffer(QPixmap pixmap) {
    QPixmap* copy = new QPixmap(pixmap);
    ui->canvas->updateBuffer(copy);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    drawerThread->terminate();
    event->accept();
}
