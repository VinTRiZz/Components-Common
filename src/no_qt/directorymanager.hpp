#pragma once

#ifndef COMPONENTS_IS_ENABLED_QT

#include <boost/noncopyable.hpp>
#include <map>
#include <filesystem>
#include <string>

namespace Common {

/**
 * @brief The DirectoryManager class Менеджер директорий, к которым приложение имеет доступ
 */
class DirectoryManager : public boost::noncopyable {
    DirectoryManager();

public:
    static DirectoryManager& getInstance();

    void setRootPath(const std::string& rootPath);

    /**
     * @brief The DirectoryType enum  Необходимые для работы приложения
     * директории
     */
    enum DirectoryType : int {
        Config = 0,     // Конфигурации
        Data,           // Данные приложения
        Logs,           // Логи приложения
        Plugins,        // Плагины приложения
        Backup,         // Бэкапы файлов
        Temporary,      // Временные файлы

        UserDefined = 100, // Любые добавленные типы должны быть после этого значения
    };
    std::filesystem::path getDirectory(DirectoryType dtype) const;
    static std::filesystem::path getDirectoryStatic(DirectoryType dtype);

    /**
     * @brief registerDirectory Зарегистрировать пользовательский тип директории
     * @param dtype             Тип директории
     * @param dirp              Путь до директории
     * @exception               Если dtype меньеш DirectoryType::UserDefined, исключение std::invalid_argument
     */
    void registerDirectory(int dtype, const std::string& dirp);

private:
    void checkup();

    template<typename MapT>
    void createDirectories(const MapT& dirsMap, const std::string& rootdir);

    std::filesystem::path m_rootDirectory;
    std::map<int, std::filesystem::path> m_directoryPaths;

    bool isDirectoryWritable(const std::filesystem::path& p) const;
};

}

#endif // COMPONENTS_IS_ENABLED_QT
