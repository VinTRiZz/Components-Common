#pragma once

#include <QDir>
#include <QString>
#include <boost/noncopyable.hpp>
#include <map>

namespace Common {

class DirectoryManager : public boost::noncopyable {
    DirectoryManager();

public:
    static DirectoryManager& getInstance();

    void setRootPath(const QString& rootPath);

    /**
     * @brief The DirectoryType enum  Необходимые для работы приложения
     * директории
     */
    enum class DirectoryType : int {
        Config,         // Конфигурации
        Logs,           // Логи приложения
        Plugins,        // Плагины приложения
        Backup,         // Бэкапы файлов
        Temporary,      // Временные файлы
    };
    QDir getDirectory(DirectoryType dtype) const;
    static QDir getDirectoryStatic(DirectoryType dtype);

private:
    void checkup();

    template<typename MapT>
    void createDirectories(const MapT& dirsMap, const QString& rootdir);

    QString m_rootDirectory;
    std::map<DirectoryType, QString> m_directoryPaths;
};

}
