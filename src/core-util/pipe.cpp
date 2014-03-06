
#ifdef _WIN32

#include <AccCtrl.h>
#include <Aclapi.h>

Pipe::Pipe()
{
    m_handle = NULL;
}

Pipe::~Pipe()
{
    closePipe();
}

void Pipe::closePipe()
{
    if(m_handle != NULL)
    {
        FlushFileBuffers(m_handle);
        DisconnectNamedPipe(m_handle);
        CloseHandle(m_handle);
        m_handle = NULL;
    }
}

void Pipe::createPipe(const std::string &pipeName, bool block)
{
	Console::log() << "creating pipe " << pipeName << std::endl;

    closePipe();
    const UINT PipeBufferSize = 100000;

    DWORD dwRes;
    PSID pEveryoneSID = NULL, pAdminSID = NULL;
    PACL pACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS ea[1];
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    SECURITY_ATTRIBUTES attributes;
    HKEY hkSub = NULL;

    // Create a well-known SID for the Everyone group.
    BOOL success = AllocateAndInitializeSid(&SIDAuthWorld, 1,
                                            SECURITY_WORLD_RID,
                                            0, 0, 0, 0, 0, 0, 0,
                                            &pEveryoneSID);
    MLIB_ASSERT_STR(success != FALSE, "AllocateAndInitializeSid failed in Pipe::CreatePipe");

    // Initialize an EXPLICIT_ACCESS structure for an ACE.
    // The ACE will allow Everyone read access to the key.
    ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
    ea[0].grfAccessPermissions = FILE_ALL_ACCESS;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance= NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

    // Create a new ACL that contains the new ACEs.
    dwRes = SetEntriesInAcl(1, ea, NULL, &pACL);
    MLIB_ASSERT_STR(dwRes == ERROR_SUCCESS, "SetEntriesInAcl failed in Pipe::CreatePipe");

    // Initialize a security descriptor.  
    pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    MLIB_ASSERT_STR(pSD != NULL, "LocalAlloc failed in Pipe::CreatePipe");
    
    success = InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
    MLIB_ASSERT_STR(success != FALSE, "InitializeSecurityDescriptor failed in Pipe::CreatePipe");
    
    // Add the ACL to the security descriptor. 
    success = SetSecurityDescriptorDacl(pSD, 
                TRUE,     // bDaclPresent flag
                pACL, 
                FALSE);
    MLIB_ASSERT_STR(success != FALSE, "SetSecurityDescriptorDacl failed in Pipe::CreatePipe");
    
    // Initialize a security attributes structure.
    attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    attributes.lpSecurityDescriptor = pSD;
    attributes.bInheritHandle = FALSE;

    std::string fullPipeName = std::string("\\\\.\\pipe\\") + pipeName;
    m_handle = CreateNamedPipeA( 
		fullPipeName.c_str(),		// pipe name
		PIPE_ACCESS_DUPLEX,         // read/write access
        PIPE_TYPE_MESSAGE |         // message type pipe 
        PIPE_READMODE_MESSAGE |     // message-read mode 
        PIPE_WAIT,                  // blocking mode 
        PIPE_UNLIMITED_INSTANCES,   // max. instances  
        PipeBufferSize,             // output buffer size 
        PipeBufferSize,             // input buffer size 
        NMPWAIT_USE_DEFAULT_WAIT,   // client time-out 
        &attributes);               // default security attribute
    MLIB_ASSERT_STR(m_handle != INVALID_HANDLE_VALUE, "CreateNamedPipe failed in Pipe::CreatePipe");

    //
    // Block until a connection comes in
    //
	
    if(block)
    {
		Console::log("Pipe created, waiting for connection");
        BOOL Connected = (ConnectNamedPipe(m_handle, NULL) != 0);
        MLIB_ASSERT_STR(Connected != FALSE, "ConnectNamedPipe failed in Pipe::CreatePipe");
		Console::log("Connected");
    }
	else
	{
		//cout << "Not blocking for connection to complete" << endl;
	}
}

void Pipe::connectToLocalPipe(const std::string &pipeName)
{
    connectToPipe(std::string("\\\\.\\pipe\\") + pipeName);
}

void Pipe::connectToPipe(const std::string &pipeName)
{
	Console::log("Connecting to " + pipeName);
    closePipe();
    bool done = false;
    while(!done)
    {
		m_handle = CreateFileA( 
            pipeName.c_str(),             // pipe name 
            GENERIC_READ |                // read and write access 
            GENERIC_WRITE, 
            0,                            // no sharing 
            NULL,                         // default security attributes
            OPEN_EXISTING,                // opens existing pipe 
            0,                            // default attributes 
            NULL);                        // no template file
        if(m_handle != INVALID_HANDLE_VALUE)
        {
            done = true;
        }
        Sleep(100);
    }
	//cout << "Connected" << endl;

    //DWORD mode = PIPE_READMODE_MESSAGE;
	DWORD mode = PIPE_READMODE_BYTE;
    BOOL success = SetNamedPipeHandleState( 
        m_handle,  // pipe handle 
        &mode,    // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    MLIB_ASSERT_STR(success != FALSE, "SetNamedPipeHandleState failed in Pipe::ConnectToPipe");
}

bool Pipe::messagePresent()
{
	MLIB_ASSERT_STR(m_handle != NULL, "Pipe invalid in Pipe::MessagePresent");
    DWORD BytesReady  = 0;
	DWORD BytesLeft   = 0;
    BOOL success = PeekNamedPipe(
        m_handle,
        NULL,
        0,
        NULL,
        &BytesReady,
        &BytesLeft);
    //MLIB_ASSERT_STR(success != FALSE, "PeekNamedPipe failed in Pipe::MessagePresent");
    return (BytesReady > 0);
}

bool Pipe::readMessage(Vector<BYTE> &Message)
{
    MLIB_ASSERT_STR(m_handle != NULL, "Pipe invalid in Pipe::ReadMessage");
    DWORD BytesReady  = 0;
    BOOL success = PeekNamedPipe(
        m_handle,
        NULL,
        0,
        NULL,
        &BytesReady,
        NULL);
    MLIB_ASSERT_STR(success != FALSE, "PeekNamedPipe failed in Pipe::ReadMessage");
    Message.resize(BytesReady);
    if(BytesReady == 0)
    {
        return false;
    }

    DWORD BytesRead;
    success = ReadFile( 
        m_handle,                // handle to pipe 
        &Message[0],            // buffer to receive data 
        (DWORD)Message.size(),  // size of buffer 
        &BytesRead,             // number of bytes read 
        NULL);                  // not overlapped I/O 
    MLIB_ASSERT_STR(success != FALSE && BytesRead > 0, "ReadFile failed in Pipe::ReadMessage");
    return true;
}

void Pipe::sendMessage(const Vector<BYTE> &Message)
{
    sendMessage(&Message[0], (UINT)Message.size());
}

void Pipe::sendMessage(const std::string &message)
{
	sendMessage((const BYTE *)message.c_str(), (UINT)message.size());

	std::string endLine;
	endLine.push_back('\n');
	sendMessage((const BYTE *)endLine.c_str(), 1);
}

void Pipe::sendMessage(const BYTE *Message, UINT MessageLength)
{
	if(Message == NULL || MessageLength == 0) return;
    MLIB_ASSERT_STR(m_handle != NULL, "Pipe invalid in Pipe::SendMessage");

    DWORD BytesWritten;
    BOOL success = WriteFile( 
        m_handle,               // pipe handle
        Message,               // message
        MessageLength,         // message length
        &BytesWritten,         // bytes written
        NULL);                 // not overlapped
    MLIB_ASSERT_STR(success != FALSE, "WriteFile failed in Pipe::ReadMessage");
    MLIB_ASSERT_STR(BytesWritten == MessageLength, "WriteFile failed to send entire message in Pipe::ReadMessage");
}

UINT Pipe::activeInstances()
{
    MLIB_ASSERT_STR(m_handle != NULL, "Pipe invalid in Pipe::ActiveInstances");
    DWORD Instances;
    BOOL success = GetNamedPipeHandleState(
        m_handle,
        NULL,
        &Instances,
        NULL,
        NULL,
        NULL,
        0);
    MLIB_ASSERT_STR(success != FALSE, "GetNamedPipeHandleState failed in Pipe::ActiveInstances");
    return Instances;
}

std::string Pipe::userName()
{
    MLIB_ASSERT_STR(m_handle != NULL, "Pipe invalid in Pipe::UserName");
    char buffer[512];
    BOOL success = GetNamedPipeHandleStateA(
        m_handle,
        NULL,
        NULL,
        NULL,
        NULL,
        buffer,
        512);
    MLIB_ASSERT_STR(success != FALSE, "GetNamedPipeHandleState failed in Pipe::UserName");
    return std::string(buffer);
}

#endif