#include "MainWindow.hpp"
#include "TextReaderWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    TextReaderWidget* pTextReaderWidget = new TextReaderWidget(this);

    setCentralWidget(pTextReaderWidget);
}
