#include "directorymanager.hpp"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

namespace Common {

DirectoryManager::DirectoryManager() {

}

DirectoryManager& DirectoryManager::getInstance() {
    static DirectoryManager inst;
    return inst;
}

void DirectoryManager::setRootPath(const QString &rootPath)
{
    if (!rootPath.isNull()) {
        m_rootDirectory = QDir(rootPath).absolutePath();
    } else {
        m_rootDirectory = QString();
    }
    checkup();
}

template<typename MapT>
void DirectoryManager::createDirectories(const MapT& dirsMap,
                                         const QString& rootdir) {
    QDir::current().mkdir(rootdir);
    auto curdir = QDir(rootdir);
    if (    !curdir.exists() ||
            !curdir.isReadable()) {
        qCritical() << "Root dir:       " << rootdir;
        qCritical() << "Current dir:    " << QDir::currentPath();
        throw std::invalid_argument(
            "Invalid directory (not exist or not readable)");
    }

    for (auto& [dirtype, dirpath] : dirsMap) {
        if (QDir(rootdir + QDir::separator() + dirpath).exists()) {
            qDebug()
                << "[  OK  ] DirectoryManager check: Directory exist. Path:" << dirpath;
            continue;
        }

        if (!curdir.mkdir(dirpath)) {
            throw std::domain_error(
                std::string(
                    "DirectoryManager: Error creating directory. Path: " + dirpath.toStdString()));
        }
        qDebug() << "[  OK  ] DirectoryManager check: created directory. Path:" << dirpath;
    }
}

QDir DirectoryManager::getDirectory(DirectoryType dtype) const
{
    return QDir(m_rootDirectory + QDir::separator() + m_systemDirectoryPaths.at(dtype));
}

QDir DirectoryManager::getDirectoryStatic(DirectoryType dtype)
{
    auto& inst = getInstance();
    return inst.getDirectory(dtype);
}

void DirectoryManager::checkup() {
    if (m_rootDirectory.isNull()) {
        m_rootDirectory = "Approot";
    }

    m_systemDirectoryPaths[DirectoryType::Config]       = "config";
    m_systemDirectoryPaths[DirectoryType::Logs]         = "log";
    m_systemDirectoryPaths[DirectoryType::Plugins]      = "plugins";
    m_systemDirectoryPaths[DirectoryType::Backup]       = "backup";
    m_systemDirectoryPaths[DirectoryType::Temporary]    = "tmp";

    createDirectories(m_systemDirectoryPaths, m_rootDirectory);
}

}
