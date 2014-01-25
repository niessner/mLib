/*
Directory.cpp
Written by Matthew Fisher
*/

Directory::Directory(const String &path)
{
    Load(path);
}

void Directory::Load(const String &path)
{
    m_path = path + "\\";
    m_files.deleteMemory();
    m_directories.deleteMemory();

    WIN32_FIND_DATA FindResult;
    //LARGE_INTEGER FileSize;
    
    HANDLE hFind = FindFirstFile((path + String("\\*")).CString(), &FindResult);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    } 

    do
    {
        if (FindResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            String DirectoryName(FindResult.cFileName);
            if(DirectoryName != String(".") &&
               DirectoryName != String("..") &&
               DirectoryName != String(".svn"))
            {
                m_directories.PushEnd(DirectoryName);
            }
        }
        else
        {
            //FileSize.LowPart = FindResult.nFileSizeLow;
            //FileSize.HighPart = FindResult.nFileSizeHigh;
            m_files.PushEnd(String(FindResult.cFileName));
        }
    }
    while (FindNextFile(hFind, &FindResult) != 0);

    FindClose(hFind);
}

Vector<String> Directory::FilesWithSuffix(const String &suffix) const
{
    Vector<String> result;
    for(UINT fileIndex = 0; fileIndex < m_files.Length(); fileIndex++)
    {
        const String &curFilename = m_files[fileIndex];
        if(curFilename.EndsWith(suffix))
        {
            result.PushEnd(curFilename);
        }
    }
    return result;
}
