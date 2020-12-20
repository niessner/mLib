
#ifdef _WIN32

#include <core-util/UIConnection.h>
#include <iostream>
#include <core-util/utility.h>

namespace ml {

void UIConnection::init(const std::string &executableFile, const std::string &pipeBaseName)
{
    if (!util::fileExists(executableFile))
    {
        std::cout << "File not found: " << executableFile << std::endl;
        std::cout << "Working directory: " << util::getWorkingDirectory() << std::endl;
        std::cin.get();
        return;
    }
    if (executableFile.size() > 0 && util::runCommand(executableFile, "", false) != 0)
	{
		std::cout << "Failed to launch UI" << std::endl;
		return;
	}
	m_readFromUIPipe.createPipe(pipeBaseName + "ReadFromUI", false);
	m_writeToUIPipe.connectToLocalPipe(pipeBaseName + "WriteToUI");
	std::cout << "UI pipes created" << std::endl;
}

void UIConnection::readMessages()
{
	while(m_readFromUIPipe.messagePresent())
	{
		std::vector<BYTE> message;
		m_readFromUIPipe.readMessage(message);
		message.push_back(0);
		std::string s = std::string((const char *)&message[0]);
		if(s.size() > 0)
		{
			m_messages.push_back(s);
		}
	}
}

void UIConnection::sendMessage(const std::string &message)
{
	m_writeToUIPipe.sendMessage(message);
}

}  // namespace ml

#endif  // _WIN32
