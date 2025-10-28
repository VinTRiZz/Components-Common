#include "applicationsettings.hpp"

#include <QDateTime>
#include <QFile>

#include <QSettings>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "directorymanager.hpp"

#include <Components/Logger/Logger.h>

namespace Common {

ApplicationSettings::ApplicationSettings() {}

ApplicationSettings::~ApplicationSettings() {}

void ApplicationSettings::addSetting(const QString &settingName)
{
    m_settings.insert(std::make_shared<AppSetting>(settingName));
}

void ApplicationSettings::addSetting(const std::shared_ptr<AppSetting>& pSetting)
{
    m_settings.insert(pSetting);
}

std::shared_ptr<AppSetting> ApplicationSettings::getSetting(const QString &settingName) const
{
    for (const auto& pSetting : m_settings) {
        if (pSetting->getName() == settingName) {
            return pSetting;
        }
    }
    return nullptr;
}

ApplicationSettings& ApplicationSettings::getInstance() {
    static ApplicationSettings inst;
    return inst;
}

void ApplicationSettings::loadSettings(const QString& configPath) {
    if (configPath.isNull()) {
        return loadSettings(DirectoryManager::getInstance().getDirectory(DirectoryManager::DirectoryType::Config).absolutePath() + QDir::separator() +
                                 APPLICATION_SETTINGS_FILE_PATH);
    }
    LOG_INFO("Loading settings from file:", configPath);

    QSettings settings(configPath, QSettings::IniFormat);
    for (auto& sett : m_settings) {
        sett->readSettings(settings);
    }

    LOG_OK("Settings loaded");
}

void ApplicationSettings::saveSettings(const QString& configPath) const {
    if (configPath.isNull()) {
        return saveSettings(DirectoryManager::getInstance().getDirectory(DirectoryManager::DirectoryType::Config).absolutePath() + QDir::separator() +
                                 APPLICATION_SETTINGS_FILE_PATH);
    }

    LOG_INFO("Saving settings to file:", configPath);

    QSettings settings(configPath, QSettings::IniFormat);
    settings.clear();
    for (auto& sett : m_settings) {
        sett->writeSettings(settings);
    }
    settings.sync();

    LOG_OK("Setgings saved");
}

AppSetting::AppSetting(const QString &settingName) :
    m_settingName {settingName}
{

}

QString AppSetting::getName() const
{
    return m_settingName;
}

void AppSetting::reset()
{
    for (auto v : getAllEnums()) {
        m_propertiesMap.emplace(getValueName(v), QVariant());
    }
}

void AppSetting::readSettings(QSettings &settingsFile)
{
    if (!m_registeredProperties.empty()) {
        for (auto& [key, v] : m_registeredProperties) {
            m_propertiesMap.emplace(v, QVariant());
        }
    }

    for (auto& [propName, propValue] : m_propertiesMap) {
        propValue = settingsFile.value(propName, {});
    }
}

void AppSetting::writeSettings(QSettings &settingsFile) const
{
    for (auto& [propName, propValue] : m_propertiesMap) {
        settingsFile.setValue(propName, propValue);
    }
}

QWidget* AppSetting::createEditor(QWidget *parent) const
{
    auto pRes = new QWidget(parent);
    auto pLayout = new QVBoxLayout;
    pRes->setLayout(pLayout);

    for (auto& [propName, propValue] : m_propertiesMap) {
        auto pSubLayout = new QHBoxLayout();

        auto pLabel = new QLabel(propName, pRes);
        auto pEditor = new QLineEdit(propValue.toString(), pRes);

        pSubLayout->addWidget(pLabel);
        pSubLayout->addWidget(pEditor);
        pLayout->addLayout(pSubLayout);
    }

    return pRes;
}

void AppSetting::setValueName(int valueEnum, const QString &name)
{
    m_registeredProperties[valueEnum] = name;
}

void AppSetting::setValue(const QString &valueName, const QVariant &value)
{
    m_propertiesMap[valueName] = value;
}

void AppSetting::setValue(int valueEnum, const QVariant &value)
{
    m_propertiesMap[getValueName(valueEnum)] = value;
}

QVariant AppSetting::valueByEnum(int enumValue) const
{
    return m_propertiesMap.at(getValueName(enumValue));
}

QVariant AppSetting::valueByKey(const QString &valueName) const
{
    return m_propertiesMap.at(valueName);
}

QString AppSetting::getValueName(int enumValue) const
{
    if (m_registeredProperties.count(enumValue)) {
        return m_registeredProperties.at(enumValue);
    }
    return {};
}

std::set<int> AppSetting::getAllEnums() const
{
    std::set<int> res;
    for (auto& [key, val] : m_registeredProperties) {
        res.insert(key);
    }
    return res;
}

}
