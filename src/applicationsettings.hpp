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
public:
    AppSetting();

    void setName(const QString& name);
    QString getName() const;

    void setValue(const QVariant& v);
    QVariant getValue() const;

    void setAvailableValues(const std::map<QString, QVariant>& availableValues);

    // Для текста -- кол-во символов
    void setMin(long long minV);
    void setMax(long long maxV);

    void reset();

    QWidget* createTextEditor(QWidget* parent);
    QWidget* createListEditor(QWidget* parent);
    QWidget* createColorEditor(QWidget* parent);
    QWidget* createIntSpinBoxEditor(QWidget* parent, int step = 1);
    QWidget* createDoubleSpinBoxEditor(QWidget* parent, double step = 0.1);

private:
    QString     m_selfName;
    QVariant    m_defaultValue;
    QVariant    m_currentValue;

    std::map<QString, QVariant> m_availableValues;

    long long m_minV {0};
    long long m_maxV {std::numeric_limits<long long>::max()};
};

class ApplicationSettings : public boost::noncopyable {

    const QString APPLICATION_SETTINGS_FILE_PATH{"default.ini"};
    std::map<QString, std::set<std::shared_ptr<AppSetting> > > m_settingSections;

    ApplicationSettings();
public:
    ~ApplicationSettings();

    bool hasSetting(const QString& section, const QString& settingName);
    void addSetting(const QString& section, const QString& settingName);
    void addSetting(const QString& section, const std::shared_ptr<AppSetting> &pSetting);
    std::shared_ptr<AppSetting> getSetting(const QString& section, const QString& settingName) const;

    // Работа с файлом настроек и классом
    static ApplicationSettings& getInstance();

    // Работа с файлом настроек для внешних целей (загрузка профилей, например)
    void loadSettings(const QString& configPath);
    void saveSettings(const QString& configPath) const;
};

}
