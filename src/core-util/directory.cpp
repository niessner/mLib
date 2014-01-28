
Directory::Directory(const String &path)
{
    load(path);
}

void Directory::load(const String &path)
{
    m_path = path + "\\";
    m_files.deleteMemory();
    m_directories.deleteMemory();

    WIN32_FIND_DATAA findResult;
    
    HANDLE hFind = FindFirstFileA((path + String("\\*")).ptr(), &findResult);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do
    {
        if (findResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            String directoryName(findResult.cFileName);
            if(!directoryName.startsWith("."))
            {
                m_directories.pushBack(directoryName);
            }
        }
        else
        {
            //FileSize.LowPart = findResult.nFileSizeLow;
            //FileSize.HighPart = findResult.nFileSizeHigh;
            m_files.pushBack(String(findResult.cFileName));
        }
    }
    while (FindNextFileA(hFind, &findResult) != 0);

    FindClose(hFind);
}

Vector<String> Directory::filesWithSuffix(const String &suffix) const
{
    Vector<String> result;
    for(UINT fileIndex = 0; fileIndex < m_files.size(); fileIndex++)
    {
        const String &filename = m_files[fileIndex];
        if(filename.endsWith(suffix))
        {
            result.pushBack(filename);
        }
    }
    return result;
}
