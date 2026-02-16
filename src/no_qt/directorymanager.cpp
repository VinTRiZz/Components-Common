#include "directorymanager.hpp"

#include <iostream>

#ifndef COMPONENTS_IS_ENABLED_QT

namespace Common {

DirectoryManager::DirectoryManager() {

}

DirectoryManager& DirectoryManager::getInstance() {
    static DirectoryManager inst;
    return inst;
}

void DirectoryManager::setRootPath(const std::string &rootPath)
{
    if (!rootPath.empty()) {
        m_rootDirectory = std::filesystem::absolute(rootPath);
    } else {
        m_rootDirectory = std::string();
    }
    checkup();
}

template<typename MapT>
void DirectoryManager::createDirectories(const MapT& dirsMap,
                                         const std::string& rootdir) {
    std::filesystem::create_directory(std::filesystem::current_path() / rootdir); // Не понимаю прикола с оператором "/", но пусть так и будет
    auto curdir = std::filesystem::path(rootdir);
    if (    !std::filesystem::exists(curdir) ||
            !std::filesystem::is_directory(curdir) ||
            !isDirectoryWritable(curdir)) {
        std::cerr << "Root dir:       " << rootdir << std::endl;
        std::cerr << "Current dir:    " << std::filesystem::current_path() << std::endl;
        throw std::invalid_argument("Invalid directory (not exist or not readable)");
    }

    for (auto& [dirtype, dirpath] : dirsMap) {
        if (std::filesystem::exists(rootdir + std::filesystem::path::preferred_separator + dirpath.generic_string())) {
            std::cout << "[  OK  ] DirectoryManager check: Directory exist. Path:" << dirpath << std::endl;
            continue;
        }

        if (!std::filesystem::create_directory(curdir / dirpath)) {
            throw std::domain_error(
                std::string(std::string("DirectoryManager: Error creating directory. Path: ") + dirpath.generic_string()));
        }
        std::cout << "[  OK  ] DirectoryManager check: created directory. Path:" << dirpath << std::endl;
    }
}

std::filesystem::path DirectoryManager::getDirectory(DirectoryType dtype) const
{
    return std::filesystem::path(m_rootDirectory.generic_string() + std::filesystem::path::preferred_separator) / m_directoryPaths.at(dtype);
}

std::filesystem::path DirectoryManager::getDirectoryStatic(DirectoryType dtype)
{
    auto& inst = getInstance();
    return inst.getDirectory(dtype);
}

void DirectoryManager::registerDirectory(int dtype, const std::string &dirp)
{
    if (dtype < int(DirectoryType::UserDefined)) {
        throw std::invalid_argument("DirectoryManager: Invalid directory type");
    }
    m_directoryPaths[dtype] = dirp;
}

void DirectoryManager::checkup() {
    if (m_rootDirectory.empty()) {
        m_rootDirectory = "Approot";
    }

    m_directoryPaths[DirectoryType::Config]       = "config";
    m_directoryPaths[DirectoryType::Data]         = "data";
    m_directoryPaths[DirectoryType::Logs]         = "log";
    m_directoryPaths[DirectoryType::Plugins]      = "plugins";
    m_directoryPaths[DirectoryType::Backup]       = "backup";
    m_directoryPaths[DirectoryType::Temporary]    = "tmp";

    createDirectories(m_directoryPaths, m_rootDirectory);
}

bool DirectoryManager::isDirectoryWritable(const std::filesystem::path &p) const
{
    auto perms = std::filesystem::status(p).permissions();
#warning "Wrong permission comparing. Need fix in case if self != owner"
    return bool(perms & std::filesystem::perms::owner_write); // TODO: Добавить сравнение владельца директории с правами доступа
}

}

#endif // COMPONENTS_IS_ENABLED_QT
