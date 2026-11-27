

// own
#include <SettingsDialog.hpp>
#include <BionicWidget.hpp>

// qt
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>

namespace
{
    constexpr int MinWidth = 460;
    constexpr int ValueLabelWidth = 56;
    constexpr int FormSpacing = 12;
    constexpr int DialogSpacing = 16;

    // Un deslizador con su valor actual al lado.
    QWidget *createSlider(QWidget *parent, int min, int max, int value, const QString &suffix, QSlider **slider)
    {
        auto *row = new QWidget(parent);
        auto *layout = new QBoxLayout(QBoxLayout::LeftToRight, row);
        auto *valueLabel = new QLabel(QString::number(value) + suffix, row);

        *slider = new QSlider(Qt::Horizontal, row);
        (*slider)->setRange(min, max);
        (*slider)->setValue(value);

        layout->setContentsMargins(0, 0, 0, 0);
        valueLabel->setMinimumWidth(ValueLabelWidth);
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QObject::connect(*slider, &QSlider::valueChanged, valueLabel, [valueLabel, suffix](int current)
                         { valueLabel->setText(QString::number(current) + suffix); });

        layout->addWidget(*slider, 1);
        layout->addWidget(valueLabel);

        return row;
    }
} // namespace

SettingsDialog::SettingsDialog(const SettingsValues &values, QWidget *parent)
    : QDialog(parent)
{
    auto *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    auto *form = new QFormLayout;

    auto *themeCombo = new QComboBox(this);
    themeCombo->addItems({tr("Dark"), tr("Light"), tr("Auto (follow the system)")});
    themeCombo->setCurrentIndex(values.theme);

    auto *onTopCheck = new QCheckBox(tr("Keep this window above the others"), this);
    onTopCheck->setChecked(values.alwaysOnTop);

    auto *bandCheck = new QCheckBox(tr("Show the reading band"), this);
    bandCheck->setChecked(values.bandVisible);

    QSlider *bandSlider = nullptr;
    QWidget *bandRow = createSlider(this, BionicWidget::MinBandPercent, BionicWidget::MaxBandPercent, values.bandPercent, QStringLiteral(" %"), &bandSlider);
    bandSlider->setEnabled(values.bandVisible);

    QSlider *columnSlider = nullptr;
    QWidget *columnRow = createSlider(this, BionicWidget::MinColumnChars, BionicWidget::MaxColumnChars, values.columnChars, QStringLiteral(" ch"), &columnSlider);

    auto *bionicCheck = new QCheckBox(tr("Bionic reading (bold the start of each word)"), this);
    bionicCheck->setChecked(values.bionic);

    auto *timerCheck = new QCheckBox(tr("Start the timer when presenting"), this);
    timerCheck->setChecked(values.timerOnPresent);

    auto *vimCheck = new QCheckBox(tr("Vim keys (h j k l) instead of WASD"), this);
    vimCheck->setChecked(values.vimKeys);

    form->setVerticalSpacing(FormSpacing);
    form->addRow(tr("Theme"), themeCombo);
    form->addRow(tr("Window"), onTopCheck);
    form->addRow(tr("Reading band"), bandCheck);
    form->addRow(tr("Band position"), bandRow);
    form->addRow(tr("Column width"), columnRow);
    form->addRow(tr("Text"), bionicCheck);
    form->addRow(tr("Presentation"), timerCheck);
    form->addRow(tr("Keys"), vimCheck);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);

    connect(themeCombo, &QComboBox::currentIndexChanged, this, &SettingsDialog::themeChanged);
    connect(onTopCheck, &QCheckBox::toggled, this, &SettingsDialog::alwaysOnTopChanged);
    connect(bandCheck, &QCheckBox::toggled, this, &SettingsDialog::bandVisibleChanged);
    connect(bandCheck, &QCheckBox::toggled, bandSlider, &QSlider::setEnabled);
    connect(bandSlider, &QSlider::valueChanged, this, &SettingsDialog::bandPositionChanged);
    connect(columnSlider, &QSlider::valueChanged, this, &SettingsDialog::columnCharsChanged);
    connect(bionicCheck, &QCheckBox::toggled, this, &SettingsDialog::bionicChanged);
    connect(timerCheck, &QCheckBox::toggled, this, &SettingsDialog::timerOnPresentChanged);
    connect(vimCheck, &QCheckBox::toggled, this, &SettingsDialog::vimKeysChanged);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->setSpacing(DialogSpacing);
    layout->addLayout(form);
    layout->addWidget(buttons);

    setWindowTitle(tr("Settings"));
    setMinimumWidth(MinWidth);
}
