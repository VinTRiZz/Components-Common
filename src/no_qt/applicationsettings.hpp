#pragma once

#ifndef COMPONENTS_IS_ENABLED_QT

#include <boost/noncopyable.hpp>

#include <map>
#include <set>
#include <string>
#include <variant>
#include <optional>
#include <numeric>
#include <limits>
#include <memory>

class QSettings;

namespace Common {

using AppSettingValue_t = std::optional< std::variant<std::string, long long, double> >;

class AppSetting
{
public:
    AppSetting();

    void setName(const std::string& name);
    std::string getName() const;

    void setValue(const AppSettingValue_t& v);
    AppSettingValue_t getValue() const;
    std::string getValueString() const;

    void setAvailableValues(const std::map<std::string, AppSettingValue_t>& availableValues);

    // Для текста -- кол-во символов
    void setMin(long long minV);
    void setMax(long long maxV);

    void reset();

private:
    std::string     m_selfName;
    AppSettingValue_t    m_defaultValue;
    AppSettingValue_t    m_currentValue;

    std::map<std::string, AppSettingValue_t> m_availableValues;

    long long m_minV {0};
    long long m_maxV {std::numeric_limits<long long>::max()};
};

class ApplicationSettings : public boost::noncopyable {
    std::map<std::string, std::set<std::shared_ptr<AppSetting> > > m_settingSections;
    std::string m_currentConfigsPath {"default.ini"};

    std::map<std::string, AppSettingValue_t> m_arguments;

    ApplicationSettings();
public:
    ~ApplicationSettings();

    bool hasSetting(const std::string& section, const std::string& settingName);
    void addSetting(const std::string& section, const std::string& settingName);
    void addSetting(const std::string& section, const std::shared_ptr<AppSetting> &pSetting);
    std::shared_ptr<AppSetting> getSetting(const std::string& section, const std::string& settingName) const;

    // Работа с файлом настроек и классом
    static ApplicationSettings& getInstance();

    bool parseArguments(int argc, char* argv[]);
    AppSettingValue_t getArgument(const std::string& valName);

    // Работа с файлом настроек для внешних целей (загрузка профилей, например)
    void loadSettings(const std::string& configPath = {});
    void saveSettings(const std::string& configPath = {}) const;
};

}

#endif // COMPONENTS_IS_ENABLED_QT
