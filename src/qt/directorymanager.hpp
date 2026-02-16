#pragma once

#ifdef COMPONENTS_IS_ENABLED_QT

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
    enum DirectoryType : int {
        Config = 0,     // Конфигурации
        Logs,           // Логи приложения
        Plugins,        // Плагины приложения
        Backup,         // Бэкапы файлов
        Temporary,      // Временные файлы

        UserDefined = 100, // Любые добавленные типы должны быть после этого значения
    };
    QDir getDirectory(DirectoryType dtype) const;
    static QDir getDirectoryStatic(DirectoryType dtype);

    /**
     * @brief registerDirectory Зарегистрировать пользовательский тип директории
     * @param dtype             Тип директории
     * @param dirp              Путь до директории
     * @exception               Если dtype меньеш DirectoryType::UserDefined, исключение std::invalid_argument
     */
    void registerDirectory(int dtype, const QString& dirp);

private:
    void checkup();

    template<typename MapT>
    void createDirectories(const MapT& dirsMap, const QString& rootdir);

    QString m_rootDirectory;
    std::map<int, QString> m_directoryPaths;
};

}

#endif // COMPONENTS_IS_ENABLED_QT
