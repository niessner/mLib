
#ifdef _WIN32

void UIConnection::init(const std::string &executableFile, const std::string &pipeBaseName)
{
	if(Utility::runCommand(executableFile, "", false) != 0)
	{
		Console::log("Failed to launch UI");
		return;
	}
	m_readFromUIPipe.createPipe(pipeBaseName + "ReadFromUI", false);
	m_writeToUIPipe.connectToLocalPipe(pipeBaseName + "WriteToUI");
    Console::log("UI pipes created");
}

void UIConnection::readMessages()
{
	while(m_readFromUIPipe.messagePresent())
	{
		Vector<BYTE> message;
		m_readFromUIPipe.readMessage(message);
		message.pushBack(0);
		std::string s = std::string((const char *)&message[0]);
		if(s.size() > 0)
		{
			m_messages.pushBack(s);
		}
	}
}

void UIConnection::sendMessage(const std::string &message)
{
	m_writeToUIPipe.sendMessage(message);
}

#endif