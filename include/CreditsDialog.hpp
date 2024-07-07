

#ifndef CREDITSDIALOG_HPP
#define CREDITSDIALOG_HPP

// Qt
#include <QDialog>

class QLabel;
class QTabWidget;
class QTextEdit;

class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    CreditsDialog(QWidget *pParent = 0);

    void addAuthor(const QString &sName, const QString &sTask, const QString &sEmail, const QString &sWebAddress = QString(""));

    QString appName() const;

    void setImage(const QString &sImagePath);

protected:
    QLabel *_pIconLabel;
    QLabel *_pAppInfoLabel;
    QLabel *_pDescriptionLabel;
    QLabel *_pAuthorsLabel;
    QTextEdit *_pLicenceTextEdit;
    QString _sAppName;
    QTabWidget *_pTabWidget;

private:
    void init();
};

inline QString CreditsDialog::appName() const
{
    return _sAppName;
}

#endif
