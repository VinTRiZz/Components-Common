#include "appsetting.hpp"

namespace Common {

void AppSettingBase::setName(const std::string &name)
{
    m_name = name;
}

std::string_view AppSettingBase::getName() const
{
    return m_name;
}

void AppSettingBase::setDescription(const std::string &descr)
{
    m_description = descr;
}

std::string_view AppSettingBase::getDescription() const
{
    return m_description;
}

bool AppSettingBase::setValue(const AppSettingValue_t &v)
{
    m_value = v;
    return true;
}

bool AppSettingBase::isSet() const
{
    return !std::holds_alternative<std::monostate>(m_value);
}

std::string AppSettingBase::getValueString() const
{
    return valueToString(m_value);
}

} // namespace Common
