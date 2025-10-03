#include "applicationsettings.hpp"

#include <QDateTime>
#include <QFile>

#include <QSettings>

#include "directorymanager.hpp"

#include <Components/Logger/Logger.h>

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
    readSettings(settings);

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
    writeSettings(settings);
    settings.sync();

    LOG_OK("Setgings saved");
}

ApplicationSettings::ApplicationSettings() {}

ApplicationSettings::~ApplicationSettings() {}
