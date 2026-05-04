#include "directorymanager.hpp"

#ifndef COMPONENTS_IS_ENABLED_QT

namespace Common {

bool DirectoryManager::init() {
    std::filesystem::create_directory(std::filesystem::current_path() / m_rootdir);
    auto curdir = std::filesystem::path(m_rootdir);
    if (    !std::filesystem::exists(curdir) ||
        !std::filesystem::is_directory(curdir) ||
        !isDirectoryWritable(curdir)) {
        std::cerr << "Root dir:       " << m_rootdir << std::endl;
        std::cerr << "Current dir:    " << std::filesystem::current_path() << std::endl;
        std::cerr << "Invalid directory (not exist or not readable)";
        return false;
    }

    for (auto& [dirtype, dirpath] : m_dirPaths) {
        if (std::filesystem::exists(dirpath.generic_string())) {
            std::cout << "[  OK  ] DirectoryManager check: Directory exist. Path: " << dirpath << std::endl;
            continue;
        }

        if (!std::filesystem::create_directory(dirpath)) {
            std::cerr << std::string(std::string("DirectoryManager: Error creating directory. Path: ") + dirpath.generic_string());
            return false;
        }
        std::cout << "[  OK  ] DirectoryManager check: created directory. Path: " << dirpath << std::endl;
    }
    return true;
}

bool DirectoryManager::isDirectoryWritable(const std::filesystem::path &p) const
{
    auto perms = std::filesystem::status(p).permissions();
    return (perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none; // TODO: Check work
}

}

#endif // COMPONENTS_IS_ENABLED_QT
