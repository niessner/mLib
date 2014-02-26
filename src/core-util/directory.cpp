
Directory::Directory(const std::string &path)
{
    load(path);
}

Vector<std::string> Directory::filesWithSuffix(const std::string &suffix) const
{
    Vector<std::string> result;
    for(UINT fileIndex = 0; fileIndex < m_files.size(); fileIndex++)
    {
        const std::string &filename = m_files[fileIndex];
        if(StringUtil::endsWith(filename, suffix))
        {
            result.pushBack(filename);
        }
    }
    return result;
}

#ifdef WIN32
void Directory::load(const std::string &path)
{
	m_path = path + "\\";
	m_files.clear();
	m_directories.clear();

	WIN32_FIND_DATAA findResult;

	HANDLE hFind = FindFirstFileA((path + std::string("\\*")).c_str(), &findResult);

	if (hFind == INVALID_HANDLE_VALUE) return;

	do
	{
		if (findResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string directoryName(findResult.cFileName);
			if(!StringUtil::startsWith(directoryName, "."))
			{
				m_directories.pushBack(directoryName);
			}
		}
		else
		{
			//FileSize.LowPart = findResult.nFileSizeLow;
			//FileSize.HighPart = findResult.nFileSizeHigh;
			m_files.pushBack(std::string(findResult.cFileName));
		}
	}
	while (FindNextFileA(hFind, &findResult) != 0);

	FindClose(hFind);
}
#endif

#ifdef LINUX
void Directory::load(const std::string &path)
{
	
}
#endif