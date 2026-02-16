#include "applicationsettings.hpp"

#ifndef COMPONENTS_IS_ENABLED_QT

#include <Components/Logger/Logger.h>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/tokenizer.hpp>

namespace Common {

namespace pt = boost::property_tree;

ApplicationSettings::ApplicationSettings() {}

ApplicationSettings::~ApplicationSettings() {}

bool ApplicationSettings::hasSetting(const std::string &section, const std::string &settingName)
{
    return (getSetting(section, settingName).use_count() != 0);
}

void ApplicationSettings::addSetting(const std::string &section, const std::string &settingName)
{
    auto pSett = std::make_shared<AppSetting>();
    pSett->setName(settingName);
    m_settingSections[section].insert(pSett);
}

void ApplicationSettings::addSetting(const std::string &section, const std::shared_ptr<AppSetting>& pSetting)
{
    m_settingSections[section].insert(pSetting);
}

std::shared_ptr<AppSetting> ApplicationSettings::getSetting(const std::string &section, const std::string &settingName) const
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

bool ApplicationSettings::parseArguments(int argc, char *argv[])
{
    std::string curargName;

    for (int i = 0; i < argc; ++i) {
        auto token = std::string(argv[i]);
        if (token[0] == '-') {
            curargName = token;
            curargName.erase(0, curargName.find_first_not_of('-'));
            continue;
        }
        if (!m_arguments[curargName].has_value()) {
            m_arguments[curargName] = token + " ";
            continue;
        }
        std::get<std::string>(m_arguments[curargName].value()) += token + " ";
    }

    for (auto& [argName, argValue] : m_arguments) {
        if (!argValue.has_value()) {
            continue;
        }
        try {
            argValue = std::stoll(std::get<std::string>(argValue.value()));
        } catch (std::invalid_argument& ex) {
            try {
                argValue = std::stod(std::get<std::string>(argValue.value()));
            } catch (std::invalid_argument&) {
                // Do nothing
            }
        }
    }

    // Help processing
    if (m_arguments.count("help") ||
        m_arguments.count("h")) {
        return false;
    }
    return true;
}

AppSettingValue_t ApplicationSettings::getArgument(const std::string &valName)
{
    auto arg = m_arguments.find(valName);
    if (arg == m_arguments.end()) {
        return std::nullopt;
    }
    return arg->second;
}

void ApplicationSettings::loadSettings(const std::string& configPath) {
    if (configPath.empty()) {
        return loadSettings(m_currentConfigsPath);
    }

    m_currentConfigsPath = configPath;
    COMPLOG_INFO("Loading settings from file:", configPath);

    pt::ptree tree;
    try {
        pt::ini_parser::read_ini(configPath, tree);
        COMPLOG_OK("Settings loaded");
    } catch (const pt::ini_parser_error& e) {
        COMPLOG_ERROR("Failed to parse settings:", e.what());
    }

    for (auto& [groupName, group] : tree) {
        for (auto& [valueName, value] : group) {
            if (auto pSett = getSetting(groupName, valueName); pSett) {
                pSett->setValue(value.data());
                continue;
            }
            addSetting(groupName, valueName);
            getSetting(groupName, valueName)->setValue(value.data());
        }
    }

    COMPLOG_OK("Settings loaded");
}

void ApplicationSettings::saveSettings(const std::string& configPath) const {
    if (configPath.empty()) {
        return saveSettings(m_currentConfigsPath);
    }

    COMPLOG_INFO("Saving settings to file:", configPath);

    pt::ptree tree;
    for (auto& [settGroup, setts] : m_settingSections) {
        auto& subgroup = tree.put(settGroup, "");
        for (auto& pSett : setts) {
            subgroup.put(pSett->getName(), pSett->getValueString());
        }
    }

    try {
        pt::ini_parser::write_ini(configPath, tree);
        std::cout << "Configuration saved" << std::endl;
    } catch (const pt::ini_parser_error& e) {
        std::cerr << "Error saving: " << e.what() << std::endl;
    }

    COMPLOG_OK("Settings saved");
}

AppSetting::AppSetting()
{

}

void AppSetting::setName(const std::string &name)
{
    m_selfName = name;
}

std::string AppSetting::getName() const
{
    return m_selfName;
}

void AppSetting::reset()
{
    m_currentValue = m_defaultValue;
}


void AppSetting::setValue(const AppSettingValue_t &value)
{
    m_currentValue = value;
}

AppSettingValue_t AppSetting::getValue() const
{
    return m_currentValue;
}

std::string AppSetting::getValueString() const
{
    if (!m_currentValue.has_value()) {
        return {};
    }

    return std::visit([](auto& v) -> std::string {
        if constexpr (std::is_arithmetic_v<std::decay_t<decltype(v)> >) {
            return std::to_string(v);
        } else {
            return v;
        }
    }, m_currentValue.value());
}

void AppSetting::setAvailableValues(const std::map<std::string, AppSettingValue_t> &availableValues)
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
