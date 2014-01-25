/*
Directory.h
Written by Matthew Fisher
*/

class Directory
{
public:
    Directory() {}
    Directory(const String &path);
    void load(const String &path);

    inline const String& path() const
    {
        return m_path;
    }
    inline const Vector<String>& files() const
    {
        return m_files;
    }
    inline const Vector<String>& directories() const
    {
        return m_directories;
    }
    Vector<String> filesWithSuffix(const String &suffix) const;

private:
    String m_path;
    Vector<String> m_files;
    Vector<String> m_directories;
};

