/*
Directory.h
Written by Matthew Fisher
*/

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
    inline const Vector<std::string>& files() const
    {
        return m_files;
    }
    inline const Vector<std::string>& directories() const
    {
        return m_directories;
    }
    Vector<std::string> filesWithSuffix(const std::string &suffix) const;

private:
    std::string m_path;
    Vector<std::string> m_files;
    Vector<std::string> m_directories;
};

