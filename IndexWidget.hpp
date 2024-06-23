#ifndef INDEXWIDGET_HPP
#define INDEXWIDGET_HPP

#include <QWidget>

class IndexWidget: public QWidget
{
    Q_OBJECT

public:
    IndexWidget(QWidget *parent = nullptr);

signals:
    void openFile(const QString& path);
};

#endif
