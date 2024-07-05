

// Own
#include <MainWindow.hpp>
#include <TextReaderWidget.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    setCentralWidget(new TextReaderWidget(this));
}
