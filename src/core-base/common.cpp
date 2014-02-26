
void warningFunctionMLIB(const char *description)
{
	Console::log() << description << std::endl;
}

void errorFunctionMLIB(const char *description)
{
	Console::log() << description << std::endl;
}

void assertFunctionMLIB(bool statement, const char *description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
		DEBUG_BREAK;
	}
}

void warningFunctionMLIB(const String &description)
{
	Console::log() << description << std::endl;
}

void errorFunctionMLIB(const String &description)
{
	Console::log() << description << std::endl;
}

void assertFunctionMLIB(bool statement, const String &description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
		DEBUG_BREAK;
	}
}