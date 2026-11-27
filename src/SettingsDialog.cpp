

// Own
#include <SettingsDialog.hpp>

// Qt
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>

namespace
{
// A slider with its current value next to it.
QWidget *createSlider(QWidget *pParent, int iMin, int iMax, int iValue, const QString &sSuffix, QSlider **ppSlider)
{
    QWidget *pRow = new QWidget(pParent);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pRow);
    QSlider *pSlider = new QSlider(Qt::Horizontal, pRow);
    QLabel *pValueLabel = new QLabel(QString::number(iValue) + sSuffix, pRow);

    pLayout->setContentsMargins(0, 0, 0, 0);
    pSlider->setRange(iMin, iMax);
    pSlider->setValue(iValue);
    pValueLabel->setMinimumWidth(56);
    pValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QObject::connect(pSlider, &QSlider::valueChanged, pValueLabel, [pValueLabel, sSuffix](int iNew)
                     { pValueLabel->setText(QString::number(iNew) + sSuffix); });

    pLayout->addWidget(pSlider, 1);
    pLayout->addWidget(pValueLabel);
    *ppSlider = pSlider;

    return pRow;
}
}

SettingsDialog::SettingsDialog(const SettingsValues &values, QWidget *pParent)
    : QDialog(pParent)
{
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QFormLayout *pForm = new QFormLayout();

    QComboBox *pThemeCombo = new QComboBox(this);
    pThemeCombo->addItems({"Dark", "Light", "Auto (follow the system)"});
    pThemeCombo->setCurrentIndex(values.theme);

    QCheckBox *pOnTopCheck = new QCheckBox("Keep this window above the others", this);
    pOnTopCheck->setChecked(values.alwaysOnTop);

    QCheckBox *pBandCheck = new QCheckBox("Show the reading band", this);
    pBandCheck->setChecked(values.bandVisible);

    QSlider *pBandSlider = nullptr;
    QWidget *pBandRow = createSlider(this, 15, 60, values.bandPercent, " %", &pBandSlider);
    pBandSlider->setEnabled(values.bandVisible);

    QSlider *pColumnSlider = nullptr;
    QWidget *pColumnRow = createSlider(this, 40, 100, values.columnChars, " ch", &pColumnSlider);

    QCheckBox *pBionicCheck = new QCheckBox("Bionic reading (bold the start of each word)", this);
    pBionicCheck->setChecked(values.bionic);

    QCheckBox *pTimerCheck = new QCheckBox("Start the timer when presenting", this);
    pTimerCheck->setChecked(values.timerOnPresent);

    QCheckBox *pVimCheck = new QCheckBox("Vim keys (h j k l) instead of WASD", this);
    pVimCheck->setChecked(values.vimKeys);

    pForm->setVerticalSpacing(12);
    pForm->addRow("Theme", pThemeCombo);
    pForm->addRow("Window", pOnTopCheck);
    pForm->addRow("Reading band", pBandCheck);
    pForm->addRow("Band position", pBandRow);
    pForm->addRow("Column width", pColumnRow);
    pForm->addRow("Text", pBionicCheck);
    pForm->addRow("Presentation", pTimerCheck);
    pForm->addRow("Keys", pVimCheck);

    QDialogButtonBox *pButtons = new QDialogButtonBox(QDialogButtonBox::Close, this);

    connect(pThemeCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(themeChanged(int)));
    connect(pOnTopCheck, SIGNAL(toggled(bool)), this, SIGNAL(alwaysOnTopChanged(bool)));
    connect(pBandCheck, SIGNAL(toggled(bool)), this, SIGNAL(bandVisibleChanged(bool)));
    connect(pBandCheck, SIGNAL(toggled(bool)), pBandSlider, SLOT(setEnabled(bool)));
    connect(pBandSlider, SIGNAL(valueChanged(int)), this, SIGNAL(bandPositionChanged(int)));
    connect(pColumnSlider, SIGNAL(valueChanged(int)), this, SIGNAL(columnCharsChanged(int)));
    connect(pBionicCheck, SIGNAL(toggled(bool)), this, SIGNAL(bionicChanged(bool)));
    connect(pTimerCheck, SIGNAL(toggled(bool)), this, SIGNAL(timerOnPresentChanged(bool)));
    connect(pVimCheck, SIGNAL(toggled(bool)), this, SIGNAL(vimKeysChanged(bool)));
    connect(pButtons, SIGNAL(rejected()), this, SLOT(reject()));

    pLayout->setSpacing(16);
    pLayout->addLayout(pForm);
    pLayout->addWidget(pButtons);

    setWindowTitle("Settings");
    setMinimumWidth(460);
}
