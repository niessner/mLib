
class CommandLineReader
{
public:
	CommandLineReader(const String &usage, int argc, char* argv[])
	{
		m_commandLine = "";
		for(int arg = 1; arg < argc; arg++)
			m_commandLine += argv[arg];
		m_args = m_commandLine.split(" ");
		m_usage = usage;
	}

	CommandLineReader(const String &usage, const String &commandLine)
	{
		m_commandLine = commandLine;
		m_args = commandLine.split(" ");
		m_usage = usage;
	}

	const Vector<String>& args() const
	{
		return m_args;
	}

	const String arg(UINT argIndex) const
	{
		if(argIndex >= m_args.size())
		{
			Console::log("insufficient number of arguments: " + m_commandLine);
			Console::log("usage: " + m_usage);
			MLIB_ERROR("aborting");
			exit(1);
		}
		return m_args[argIndex];
	}

private:
	Vector<String> m_args;
	String m_commandLine;
	String m_usage;
};