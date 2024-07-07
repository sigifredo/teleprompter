

#ifndef CHRONOMETERWIDGET_HPP
#define CHRONOMETERWIDGET_HPP

// Qt
#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;

class ChronometerWidget : public QWidget
{
    Q_OBJECT

public:
    ChronometerWidget(QWidget *pParent = nullptr);

public slots:
    void start();
    void stop();

private:
    QPushButton *_pStartButton;
    QLabel *_pTimeLabel;
    QTimer *_pTimer;
    int _iTimeElapsed;

    void setTimerActiveStyle(const bool &active);

private slots:
    void updateTime();
};

#endif
