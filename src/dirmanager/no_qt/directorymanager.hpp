#pragma once

#ifndef COMPONENTS_IS_ENABLED_QT

#include <filesystem>

#include "../directorymanagerbase.hpp"

namespace Common {

/**
 * @brief The DirectoryManager class Менеджер директорий, к которым приложение имеет доступ
 */
class DirectoryManager : public DirectoryManagerBase<DirectoryManager, std::filesystem::path>
{
public:
    bool init() override;

private:
    bool isDirectoryWritable(const std::filesystem::path& p) const;
};

}

#endif // COMPONENTS_IS_ENABLED_QT
