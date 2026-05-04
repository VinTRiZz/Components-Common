#pragma once

#ifdef COMPONENTS_IS_ENABLED_QT

#include <QString>

#include "../directorymanagerbase.hpp"

namespace Common {

/**
 * @brief The DirectoryManager class Менеджер директорий, к которым приложение имеет доступ
 */
class DirectoryManager : public DirectoryManagerBase<DirectoryManager, QString>
{
public:
    bool init() override;

private:
};

}

#endif // COMPONENTS_IS_ENABLED_QT
