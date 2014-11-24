#include "Advanced2D.h"

Timer::Timer(void)
{
	timer_start = timeGetTime();
	reset();
}

Timer::~Timer(void){}

DWORD Timer::getTimer()
{
	return (DWORD) (timeGetTime());
}

DWORD Timer::getStartTimeMillis()
{
	return (DWORD) (timeGetTime() - timer_start);
}


void Timer::sleep(int ms)
{
	DWORD start = getTimer();
	while (start + ms > getTimer());
}

void Timer::reset()
{
	stopwatch_start = getTimer();
}

bool Timer::stopwatch(int ms)
{
	if ( timeGetTime() > stopwatch_start + ms )
	{
		stopwatch_start = getTimer();
		return true;
	}
	else return false;
}

