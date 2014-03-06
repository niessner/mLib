
#ifdef _WIN32

class UIConnection
{
public:
	void init(const std::string &executableFile, const std::string &pipeBaseName);
	void readMessages();
	void sendMessage(const std::string &message);

	inline Vector<std::string>& messages()
	{
		return m_messages;
	}

private:
	

	Vector<std::string> m_messages;
	Pipe m_writeToUIPipe;
	Pipe m_readFromUIPipe;
};

#endif