#pragma once

#include <boost/noncopyable.hpp>

#include <string>
#include <functional>

namespace Common {

class FileImportManager : public boost::noncopyable
{
    FileImportManager();
public:
    ~FileImportManager();

    enum class FileType : int {
        ErrorFile,
        RegularFile,
        ImageFile,
    };

    static FileImportManager& getInstance();

    void updateCache();
    void clearCache();

    void setRootDirectory(const std::string& rDir);
    void setFileCacheSize(unsigned cacheFileCount);

    void setFileTypeDetector(const std::function<FileType(const std::string&)>& ftypeDetector);

    /**
     * @brief pixmapFromPath    Считать файл
     * @param targetPath        Путь до файла
     * @param forceUpdate       При true принудительно обновить данные по файлу
     * @return                  SHA-256 хэш файла
     */
    std::string addToCache(const std::string& targetPath, bool forceUpdate = false);

    /**
     * @brief removeFromCache   Удалить файл из директории-кэша и из менеджера
     * @param targetPath
     */
    void removeFromCache(const std::string& targetPath) const;

private:
    unsigned m_fileCacheSize {10};
    unsigned m_maxFileCacheSizeMb {10}; // TODO: Использовать

    std::string m_rootDirectory;
    std::function<FileType(const std::string&)> m_fileTypeDetector;

    std::unordered_map<std::string, std::string> m_lastUsedFiles;
    mutable std::unordered_map<std::string, std::string> m_filePathCache;  // Словарь ПУТЬ --> ХЕШ
    mutable std::unordered_map<std::string, std::string> m_filesCache;  // Словарь ХЕШ --> данные файла
};

}
