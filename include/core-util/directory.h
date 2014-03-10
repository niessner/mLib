#ifndef CORE_UTIL_DIRECTORY_H__
#define CORE_UTIL_DIRECTORY_H__

namespace ml
{

class Directory
{
public:
    Directory() {}
    Directory(const std::string &path);
    void load(const std::string &path);

    inline const std::string& path() const
    {
        return m_path;
    }
    inline const std::vector<std::string>& files() const
    {
        return m_files;
    }
    inline const std::vector<std::string>& directories() const
    {
        return m_directories;
    }
    std::vector<std::string> filesWithSuffix(const std::string &suffix) const;

private:
    std::string m_path;
    std::vector<std::string> m_files;
    std::vector<std::string> m_directories;
};

}  // namespace ml

#endif  // CORE_UTIL_DIRECTORY_H__