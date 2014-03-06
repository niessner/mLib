
class Pipe
{
public:
    Pipe();
    ~Pipe();
    
    //
    // Connection
    //
    void closePipe();
    void createPipe(const std::string &pipeName, bool block);
    void connectToLocalPipe(const std::string &pipeName);
    void connectToPipe(const std::string &pipeName);

    //
    // Messaging
    //
    bool messagePresent();
    bool readMessage(Vector<BYTE> &message);
    void sendMessage(const BYTE *message, UINT messageLength);
    void sendMessage(const Vector<BYTE> &message);
	void sendMessage(const std::string &message);

    //
    // Query
    //
    UINT activeInstances();
    std::string userName();
    __forceinline bool valid()
    {
        return (m_handle != NULL);
    }
    

private:

#ifdef _WIN32
    HANDLE m_handle;
#endif
};
