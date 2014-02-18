
#ifdef WIN32
Clock::Clock()
{
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency( &ticksPerSecond );
	m_ticksPerSecond = ticksPerSecond.QuadPart;
	start();
}

void Clock::start()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	m_startTime = time.QuadPart;
}

double Clock::elapsed()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	return double(time.QuadPart - m_startTime) / double(m_ticksPerSecond);
}
#endif

#ifdef LINUX
Clock::Clock()
{
	
}

void Clock::start()
{
	
}

double Clock::elapsed()
{
	return 0.0;
}
#endif