#pragma once

#include <boost/noncopyable.hpp>

#include <QString>

class QSettings;

class ApplicationSettings : public boost::noncopyable {
public:
    const QString APPLICATION_SETTINGS_FILE_PATH{"default.ini"};

    // Работа с файлом настроек и классом
    static ApplicationSettings& getInstance();

    // Работа с файлом настроек для внешних целей (загрузка профилей, например)
    void loadSettings(const QString& configPath);
    void saveSettings(const QString& configPath) const;

    ~ApplicationSettings();

protected:
    ApplicationSettings();

    // Для дочерних классов
    virtual void readSettings(QSettings& settingsFile);
    virtual void writeSettings(QSettings& settingsFile) const;
};
