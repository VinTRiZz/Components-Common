#include "fileimportmanager.hpp"

#include <filesystem>

namespace Common {

FileImportManager::FileImportManager()
{
    m_fileTypeDetector = [](const std::string&) -> FileType {
        return FileType::RegularFile;
    };
}

FileImportManager::~FileImportManager()
{

}

FileImportManager &FileImportManager::getInstance()
{
    static FileImportManager inst;
    return inst;

}

void FileImportManager::updateCache()
{
    for (auto& dirEntr : std::filesystem::directory_iterator(m_rootDirectory)) {
        addToCache(std::filesystem::path(dirEntr).generic_string());
    }
}

void FileImportManager::clearCache()
{
    // TODO: Remove files

    m_lastUsedFiles.clear();
    m_filePathCache.clear();
    m_filesCache.clear();
}

void FileImportManager::setRootDirectory(const std::string &rDir)
{
    m_rootDirectory = rDir;
}

void FileImportManager::setFileCacheSize(unsigned int cacheFileCount)
{

}

void FileImportManager::setFileTypeDetector(const std::function<FileType (const std::string &)> &ftypeDetector)
{

}

std::string FileImportManager::addToCache(const std::string &targetPath, bool forceUpdate)
{
    if (!forceUpdate) [[likely]] {
        if (m_filePathCache.count(targetPath)) {
            return m_filePathCache.at(targetPath);
        }
    }
}

void FileImportManager::removeFromCache(const std::string &targetPath) const
{

}

}
