
MultiStream Console::s_stream(std::cout);
std::ofstream Console::s_logFile;

void Console::log(const String &s)
{
	s_stream << s.ptr() << std::endl;
}