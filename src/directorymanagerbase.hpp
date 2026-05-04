#pragma once

#include <Components/Logger/Logger.h>

#include <map>
#include <filesystem>

namespace Common {

/**
 * @brief The DirectoryType enum Directory types to get from manager
 */
enum DirectoryType : int
{
    Config = 0,     // .ini, .conf, etc.
    Data,           // Databases, resources, etc.
    Logs,           // App logs
    Plugins,        // App plugins
    Backup,         // Data, config backups, if required
    Temporary,      // Cache

    UserDefined = 100, // User-defined directories to register
};

/**
 * @brief The DirectoryManagerBase class Base of directory manager
 */
template <typename DerivedT, typename PathT>
class DirectoryManagerBase : public boost::noncopyable
{
protected:

public:
    static DerivedT& getInstance();

    void setRootPath(const std::string& rootPath);
    std::string getRootPath() const;

    /**
     * @brief init  Create subdirectories if not exist
     * @return      true if no error
     */
    virtual bool init() = 0;

    /**
     * @brief getDirectory  Get directory, registered in manager
     * @param dtype         @ref DirectoryType enum or custom value
     * @return              Directory handle
     */
    PathT getDirectory(int dtype) const;

    /**
     * @brief getDirectoryStatic    Static equ of @ref getDirectory()
     * @param dtype
     * @return
     */
    static PathT getDirectoryStatic(int dtype);

    /**
     * @brief registerDirectory Зарегистрировать пользовательский тип директории
     * @param dtype             Тип директории
     * @param dirp              Путь до директории
     * @exception               Если dtype меньеш DirectoryType::UserDefined, исключение std::invalid_argument
     */
    void registerDirectory(int dtype, const PathT& dirp);

protected:
    std::string m_rootdir;
    std::map<int, PathT> m_dirPaths;
};

template<typename DerivedT, typename PathT>
inline DerivedT &DirectoryManagerBase<DerivedT, PathT>::getInstance() {
    static DerivedT inst;
    return inst;
}

template<typename DerivedT, typename PathT>
inline void DirectoryManagerBase<DerivedT, PathT>::setRootPath(const std::string &rootPath) {
    m_rootdir = rootPath;
    auto rootBase = m_rootdir + std::filesystem::path::preferred_separator;
    registerDirectory(DirectoryType::Config,    (rootBase + "config").c_str());
    registerDirectory(DirectoryType::Data,      (rootBase + "data").c_str());
    registerDirectory(DirectoryType::Logs,      (rootBase + "log").c_str());
    registerDirectory(DirectoryType::Plugins,   (rootBase + "plugins").c_str());
    registerDirectory(DirectoryType::Backup,    (rootBase + "backup").c_str());
    registerDirectory(DirectoryType::Temporary, (rootBase + "tmp").c_str());
    init();
}

template<typename DerivedT, typename PathT>
inline PathT DirectoryManagerBase<DerivedT, PathT>::getDirectory(int dtype) const {
    auto targetIt = m_dirPaths.find(dtype);
    if (targetIt == m_dirPaths.end()) {
        return {};
    }
    return targetIt->second;
}

template<typename DerivedT, typename PathT>
inline PathT DirectoryManagerBase<DerivedT, PathT>::getDirectoryStatic(int dtype) {
    auto& inst = getInstance();
    return inst.getDirectory(dtype);
}

template<typename DerivedT, typename PathT>
inline void DirectoryManagerBase<DerivedT, PathT>::registerDirectory(int dtype, const PathT &dirp) {
    m_dirPaths[dtype] = dirp;
}

} // namespace Common
