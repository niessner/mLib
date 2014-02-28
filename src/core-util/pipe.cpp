/*#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <aclapi.h>

#include "pipe.h"*/

Pipe::Pipe()
{
    m_handle = NULL;
}

Pipe::~Pipe()
{
    ClosePipe();
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

void Pipe::createPipe(const string &pipeName, bool block)
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
    Assert(success != FALSE, "AllocateAndInitializeSid failed in Pipe::CreatePipe");

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
    Assert(dwRes == ERROR_SUCCESS, "SetEntriesInAcl failed in Pipe::CreatePipe");

    // Initialize a security descriptor.  
    pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    Assert(pSD != NULL, "LocalAlloc failed in Pipe::CreatePipe");
    
    success = InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
    Assert(success != FALSE, "InitializeSecurityDescriptor failed in Pipe::CreatePipe");
    
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

    string fullPipeName = string("\\\\.\\pipe\\") + pipeName;
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
    Assert(m_handle != INVALID_HANDLE_VALUE, "CreateNamedPipe failed in Pipe::CreatePipe");

    //
    // Block until a connection comes in
    //
	
    if(block)
    {
		cout << "Pipe created, waiting for connection" << endl;
        BOOL Connected = (ConnectNamedPipe(m_handle, NULL) != 0);
        Assert(Connected != FALSE, "ConnectNamedPipe failed in Pipe::CreatePipe");
		cout << "Connected" << endl;
    }
	else
	{
		//cout << "Not blocking for connection to complete" << endl;
	}
}

void Pipe::ConnectToLocalPipe(const string &pipeName)
{
    ConnectToPipe(string("\\\\.\\pipe\\") + pipeName);
}

void Pipe::ConnectToPipe(const string &pipeName)
{
	cout << "Connecting to " << pipeName << endl;
    ClosePipe();
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
    Assert(success != FALSE, "SetNamedPipeHandleState failed in Pipe::ConnectToPipe");
}

bool Pipe::MessagePresent()
{
	Assert(m_handle != NULL, "Pipe invalid in Pipe::MessagePresent");
    DWORD BytesReady  = 0;
	DWORD BytesLeft   = 0;
    BOOL success = PeekNamedPipe(
        m_handle,
        NULL,
        0,
        NULL,
        &BytesReady,
        &BytesLeft);
    //Assert(success != FALSE, "PeekNamedPipe failed in Pipe::MessagePresent");
    return (BytesReady > 0);
}

bool Pipe::ReadMessage(vector<BYTE> &Message)
{
    Assert(m_handle != NULL, "Pipe invalid in Pipe::ReadMessage");
    DWORD BytesReady  = 0;
    BOOL success = PeekNamedPipe(
        m_handle,
        NULL,
        0,
        NULL,
        &BytesReady,
        NULL);
    Assert(success != FALSE, "PeekNamedPipe failed in Pipe::ReadMessage");
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
    Assert(success != FALSE && BytesRead > 0, "ReadFile failed in Pipe::ReadMessage");
    return true;
}

void Pipe::SendMessage(const vector<BYTE> &Message)
{
    SendMessage(&Message[0], (UINT)Message.size());
}

void Pipe::SendMessage(const string &message)
{
	SendMessage((const BYTE *)message.c_str(), (UINT)message.size());

	string endLine;
	endLine.push_back('\n');
	SendMessage((const BYTE *)endLine.c_str(), 1);
}

void Pipe::SendMessage(const BYTE *Message, UINT MessageLength)
{
	if(Message == NULL || MessageLength == 0) return;
    Assert(m_handle != NULL, "Pipe invalid in Pipe::SendMessage");

    DWORD BytesWritten;
    BOOL success = WriteFile( 
        m_handle,               // pipe handle
        Message,               // message
        MessageLength,         // message length
        &BytesWritten,         // bytes written
        NULL);                 // not overlapped
    Assert(success != FALSE, "WriteFile failed in Pipe::ReadMessage");
    Assert(BytesWritten == MessageLength, "WriteFile failed to send entire message in Pipe::ReadMessage");
}

UINT Pipe::ActiveInstances()
{
    Assert(m_handle != NULL, "Pipe invalid in Pipe::ActiveInstances");
    DWORD Instances;
    BOOL success = GetNamedPipeHandleState(
        m_handle,
        NULL,
        &Instances,
        NULL,
        NULL,
        NULL,
        0);
    Assert(success != FALSE, "GetNamedPipeHandleState failed in Pipe::ActiveInstances");
    return Instances;
}

string Pipe::UserName()
{
    Assert(m_handle != NULL, "Pipe invalid in Pipe::UserName");
    char buffer[512];
    BOOL success = GetNamedPipeHandleStateA(
        m_handle,
        NULL,
        NULL,
        NULL,
        NULL,
        buffer,
        512);
    Assert(success != FALSE, "GetNamedPipeHandleState failed in Pipe::UserName");
    return string(buffer);
}
