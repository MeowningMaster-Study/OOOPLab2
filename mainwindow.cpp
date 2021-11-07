#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawer.h"
#include <QTimer>
#include <QCloseEvent>

QStringList formulas;
qreal scale = 0.05;
bool drag = false;
QPoint dragStart, dragOffset, offset(0, 0);
QThread* drawerThread;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
    ui->canvas->installEventFilter(this);

    drawerThread = new QThread;
    Drawer* drawer = new Drawer();
    drawer->moveToThread(drawerThread);
    connect(drawer, SIGNAL(send(QPixmap)), this, SLOT(updateBuffer(QPixmap)));
    connect(this, SIGNAL(start()), drawer, SLOT(draw()));
    connect(this, SIGNAL(updateSize(QSize)), drawer, SLOT(updateSize(QSize)), Qt::DirectConnection);
    connect(this, SIGNAL(updateFormulas(QStringList)), drawer, SLOT(updateFormulas(QStringList)), Qt::DirectConnection);
    connect(this, SIGNAL(updatePosition(QPoint, qreal)), drawer, SLOT(updatePosition(QPoint, qreal)), Qt::DirectConnection);
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

void MainWindow::onAddButtonClicked()
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
    drawerThread->exit();
    event->accept();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj != ui->canvas) {
        return false;
    }

    auto type = event->type();

    if (type == QEvent::Wheel) {
        auto ev = static_cast<QWheelEvent*>(event);
        scale = scale * (1. + ev->angleDelta().y()/120 * 0.05);
        emit updatePosition(offset, scale);
        return true;
    }

    if (type == QEvent::MouseButtonPress) {
        auto ev = static_cast<QMouseEvent*>(event);
        if (!drag) {
            drag = true;
            dragStart = ev->pos();
            dragOffset = {0, 0};
        }
        return true;
    }

    if (type == QEvent::MouseMove) {
        auto ev = static_cast<QMouseEvent*>(event);
        if (drag) {
            auto dragDiff = ev->pos() - dragStart;
            offset += dragDiff - dragOffset;
            dragOffset = dragDiff;
            emit updatePosition(offset, scale);
        }
        return true;
    }

    if (type == QEvent::MouseButtonRelease) {
        if (drag) {
            drag = false;
        }
        return true;
    }

    return false;
}
