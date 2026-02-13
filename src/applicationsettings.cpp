#include "applicationsettings.hpp"

#ifdef COMPONENTS_IS_ENABLED_QT

#include <QDateTime>
#include <QFile>

#include <QSettings>

#include <QVariant>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QColorDialog>

#include "directorymanager.hpp"

#include <Components/Logger/Logger.h>

namespace Common {

ApplicationSettings::ApplicationSettings() {}

ApplicationSettings::~ApplicationSettings() {}

bool ApplicationSettings::hasSetting(const QString &section, const QString &settingName)
{
    return (getSetting(section, settingName).use_count() != 0);
}

void ApplicationSettings::addSetting(const QString &section, const QString &settingName)
{
    auto pSett = std::make_shared<AppSetting>();
    pSett->setName(settingName);
    m_settingSections[section].insert(pSett);
}

void ApplicationSettings::addSetting(const QString &section, const std::shared_ptr<AppSetting>& pSetting)
{
    m_settingSections[section].insert(pSetting);
}

std::shared_ptr<AppSetting> ApplicationSettings::getSetting(const QString &section, const QString &settingName) const
{
    if (!m_settingSections.count(section)) {
        return {};
    }

    for (const auto& pSetting : m_settingSections.at(section)) {
        if (pSetting->getName() == settingName) {
            return pSetting;
        }
    }
    return {};
}

ApplicationSettings& ApplicationSettings::getInstance() {
    static ApplicationSettings inst;
    return inst;
}

void ApplicationSettings::loadSettings(const QString& configPath) {
    if (configPath.isNull()) {
        return loadSettings(m_currentConfigsPath);
    }

    m_currentConfigsPath = configPath;
    COMPLOG_INFO("Loading settings from file:", configPath);

    QSettings settings(configPath, QSettings::IniFormat);

    for (auto& groupName : settings.childGroups()) {
        settings.beginGroup(groupName);
        for (auto& settName : settings.childKeys()) {
            if (auto pSett = getSetting(groupName, settName); pSett) {
                pSett->setValue(settings.value(settName));
                continue;
            }
            addSetting(groupName, settName);
            getSetting(groupName, settName)->setValue(settings.value(settName));
        }
        settings.endGroup();
    }

    COMPLOG_OK("Settings loaded");
}

void ApplicationSettings::saveSettings(const QString& configPath) const {
    if (configPath.isNull()) {
        return saveSettings(m_currentConfigsPath);
    }

    COMPLOG_INFO("Saving settings to file:", configPath);

    QSettings settings(configPath, QSettings::IniFormat);
    settings.clear();
    for (auto& [settGroup, setts] : m_settingSections) {
        settings.beginGroup(settGroup);
        for (auto& pSett : setts) {
            settings.setValue(pSett->getName(), pSett->getValue().toString());
        }
        settings.endGroup();
    }
    settings.sync();

    COMPLOG_OK("Settings saved");
}

AppSetting::AppSetting()
{

}

void AppSetting::setName(const QString &name)
{
    m_selfName = name;
}

QString AppSetting::getName() const
{
    return m_selfName;
}

void AppSetting::reset()
{
    m_currentValue = m_defaultValue;
}


QWidget* AppSetting::createTextEditor(QWidget* parent)
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(m_currentValue.toString());

    // Соединение сигнала изменения текста с установкой значения
    QObject::connect(editor, &QLineEdit::textChanged, [this, editor](const QString& text) {
        setValue(text);
    });

    return editor;
}

QWidget* AppSetting::createListEditor(QWidget* parent)
{
    QComboBox* editor = new QComboBox(parent);

    // Заполнение комбобокса зарегистрированными значениями
    for (const auto& [valueName, value] : m_availableValues) {
        editor->addItem(valueName, value);
    }

    // Установка текущего значения
    int index = editor->findData(m_currentValue);
    if (index >= 0) {
        editor->setCurrentIndex(index);
    }

    // Соединение сигнала изменения выбора с установкой значения
    QObject::connect(editor, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [this, editor](int index) {
        if (index >= 0) {
            QVariant data = editor->itemData(index);
            setValue(data);
        }
    });

    return editor;
}

QWidget* AppSetting::createColorEditor(QWidget* parent)
{
    QWidget* container = new QWidget(parent);
    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel* colorLabel = new QLabel(container);
    colorLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    colorLabel->setAutoFillBackground(true);
    colorLabel->setMinimumSize(30, 20);

    QPushButton* colorButton = new QPushButton("Выбрать цвет", container);

    layout->addWidget(colorLabel);
    layout->addWidget(colorButton);

    // Функция для обновления отображения цвета
    auto updateColorDisplay = [colorLabel](const QColor& color) {
        QPalette palette = colorLabel->palette();
        palette.setColor(QPalette::Window, color);
        colorLabel->setPalette(palette);
        colorLabel->setToolTip(color.name());
    };

    // Установка начального цвета
    QColor currentColor = m_currentValue.isNull() ? Qt::white : m_currentValue.value<QColor>();
    updateColorDisplay(currentColor);

    // Соединение кнопки с диалогом выбора цвета
    QObject::connect(colorButton, &QPushButton::clicked,
                     [this, updateColorDisplay, container]() {
        QColor currentColor = m_currentValue.isNull() ? Qt::white : m_currentValue.value<QColor>();

        QColor color = QColorDialog::getColor(currentColor, container, "Выберите цвет");
        if (color.isValid()) {
            setValue(color);
            updateColorDisplay(color);
        }
    });

    return container;
}

QWidget* AppSetting::createIntSpinBoxEditor(QWidget* parent, int step)
{
    QSpinBox* editor = new QSpinBox(parent);
    editor->setRange(m_minV, m_maxV);
    editor->setSingleStep(step);

    // Установка текущего значения
    editor->setValue(m_currentValue.toInt());

    // Соединение сигнала изменения значения
    QObject::connect(editor, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this](int value) {
        setValue(value);
    });

    return editor;
}

QWidget* AppSetting::createDoubleSpinBoxEditor(QWidget* parent, double step)
{
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setRange(m_minV, m_maxV);
    editor->setSingleStep(step);
    editor->setDecimals(3); // 3 знака после запятой по умолчанию

    // Установка текущего значения
    editor->setValue(m_currentValue.toDouble());

    // Соединение сигнала изменения значения
    QObject::connect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     [this](double value) {
        setValue(value);
    });

    return editor;
}

void AppSetting::setValue(const QVariant &value)
{
    m_currentValue = value;
}

QVariant AppSetting::getValue() const
{
    return m_currentValue;
}

void AppSetting::setAvailableValues(const std::map<QString, QVariant> &availableValues)
{
    m_availableValues = availableValues;
}

void AppSetting::setMin(long long minV)
{
    m_minV = minV;
}

void AppSetting::setMax(long long maxV)
{
    m_maxV = maxV;
}

}

#endif // COMPONENTS_IS_ENABLED_QT
