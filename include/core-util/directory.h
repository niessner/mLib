#ifndef CORE_UTIL_DIRECTORY_H_
#define CORE_UTIL_DIRECTORY_H_

namespace ml
{

class Directory
{
public:
    Directory() {}
    Directory(const std::string &path);
    void load(const std::string &path);

    inline const std::string& getPath() const
    {
        return m_path;
    }
    inline const std::vector<std::string>& getFiles() const
    {
        return m_files;
    }
    inline const std::vector<std::string>& getDirectories() const
    {
        return m_directories;
    }
    std::vector<std::string> getFilesWithSuffix(const std::string& suffix) const;
	std::vector<std::string> getFilesWithPrefix(const std::string& prefix) const;
	std::vector<std::string> getFilesContaining(const std::string& str) const;

    static std::vector<std::string> enumerateDirectories(const std::string &path)
    {
        Directory dir(path);
        return dir.getDirectories();
    }

    static std::vector<std::string> enumerateFiles(const std::string &path)
    {
        Directory dir(path);
        return dir.getFiles();
    }

    static std::vector<std::string> enumerateFiles(const std::string &path, const std::string &suffix)
    {
        Directory dir(path);
        return dir.getFilesWithSuffix(suffix);
    }

    static std::vector<std::string> enumerateFilesWithPath(const std::string &path)
    {
        std::string ending = "";
        if (path.back() != '/' && path.back() != '\\')
            ending = "/";

        auto files = enumerateFiles(path);
        for (auto &f : files)
            f = path + ending + f;
        return files;
    }

    static std::vector<std::string> enumerateFilesWithPath(const std::string &path, const std::string &suffix)
    {
        std::string ending = "";
        if (path.back() != '/' && path.back() != '\\')
            ending = "/";

        auto files = enumerateFiles(path, suffix);
        for (auto &f : files)
            f = path + ending + f;
        return files;
    }

private:
    std::string m_path;
    std::vector<std::string> m_files;
    std::vector<std::string> m_directories;
};

}  // namespace ml

#endif  // CORE_UTIL_DIRECTORY_H_