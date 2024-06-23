#include "MainWindow.hpp"
#include "IndexWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    IndexWidget* pIndexWidget = new IndexWidget(this);

    setCentralWidget(pIndexWidget);
}
