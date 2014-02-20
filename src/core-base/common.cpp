
void MLIB_WARNING(const char *description)
{
	Console::log() << description << std::endl;
	DEBUG_BREAK;
}

void MLIB_ERROR(const char *description)
{
	Console::log() << description << std::endl;
	DEBUG_BREAK;
}

void MLIB_ASSERT(bool statement, const char *description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
		DEBUG_BREAK;
	}
}

void MLIB_WARNING(const String &description)
{
	Console::log() << description << std::endl;
	DEBUG_BREAK;
}

void MLIB_ERROR(const String &description)
{
	Console::log() << description << std::endl;
	DEBUG_BREAK;
}

void MLIB_ASSERT(bool statement, const String &description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
		DEBUG_BREAK;
	}
}