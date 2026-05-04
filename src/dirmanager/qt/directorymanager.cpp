#include "directorymanager.hpp"

#ifdef COMPONENTS_IS_ENABLED_QT

#include <QDebug>
#include <QDir>
#include <QFileInfo>

namespace Common {

bool DirectoryManager::init() {
    QDir::current().mkdir(m_rootdir.c_str());
    auto curdir = QDir(m_rootdir.c_str());
    if (    !curdir.exists() ||
        !curdir.isReadable()) {
        qCritical() << "Root dir:       " << m_rootdir.c_str();
        qCritical() << "Current dir:    " << QDir::currentPath();
        qCritical() << "Invalid directory (not exist or not readable)";
        return false;
    }

    for (auto& [dirtype, dirpath] : m_dirPaths) {
        if (QDir(dirpath).exists()) {
            qInfo()
            << "[  OK  ] DirectoryManager check: Directory exist. Path:" << dirpath;
            continue;
        }

        if (!curdir.mkdir(dirpath)) {
            qCritical() << "DirectoryManager: Error creating directory. Path: " <<  dirpath;
            return false;
        }
        qInfo() << "[  OK  ] DirectoryManager check: created directory. Path:" << dirpath;
    }
    return true;
}

}

#endif // COMPONENTS_IS_ENABLED_QT
