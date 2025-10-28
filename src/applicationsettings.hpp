#pragma once

#include <boost/noncopyable.hpp>

#include <map>
#include <set>

#include <QString>
#include <QVariant>

#include <QWidget>

class QSettings;

namespace Common {

class AppSetting
{
    QString m_settingName;
public:
    AppSetting(const QString& settingName);

    QString getName() const;
    void reset();

    void readSettings(QSettings& settingsFile);
    void writeSettings(QSettings& settingsFile) const;

    virtual QWidget *createEditor(QWidget* parent) const;

    void setValueName(int valueEnum, const QString &name);
    QString getValueName(int enumValue) const;
    std::set<int> getAllEnums() const;

    void setValue(const QString& valueName, const QVariant& value);
    void setValue(int valueEnum, const QVariant& value);

    QVariant valueByEnum(int enumValue) const;
    QVariant valueByKey(const QString& valueName) const;

private:
    std::map<QString, QVariant> m_propertiesMap;
    std::map<int, QString> m_registeredProperties;
};

class ApplicationSettings : public boost::noncopyable {

    const QString APPLICATION_SETTINGS_FILE_PATH{"default.ini"};
    std::set<std::shared_ptr<AppSetting> > m_settings;

    ApplicationSettings();
public:
    ~ApplicationSettings();

    void addSetting(const QString& settingName);
    void addSetting(const std::shared_ptr<AppSetting> &pSetting);
    std::shared_ptr<AppSetting> getSetting(const QString& settingName) const;

    // Работа с файлом настроек и классом
    static ApplicationSettings& getInstance();

    // Работа с файлом настроек для внешних целей (загрузка профилей, например)
    void loadSettings(const QString& configPath);
    void saveSettings(const QString& configPath) const;
};

}
