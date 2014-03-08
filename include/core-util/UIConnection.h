#ifndef INCLUDE_CORE_UTIL_UICONNECTION_H_
#define INCLUDE_CORE_UTIL_UICONNECTION_H_

#ifdef _WIN32

#include <string>

class UIConnection
{
 public:
    void init(const std::string &executableFile, const std::string &pipeBaseName);
    void readMessages();
    void sendMessage(const std::string &message);

    inline std::vector<std::string>& messages()
    {
        return m_messages;
    }
 private:
    std::vector<std::string> m_messages;
    Pipe m_writeToUIPipe;
    Pipe m_readFromUIPipe;
};

#endif

#endif  // INCLUDE_CORE_UTIL_UICONNECTION_H_
