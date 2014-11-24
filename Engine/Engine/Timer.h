#include "Advanced2D.h"

#ifndef _TIMER_H
#define _TIMER_H 1

class Timer
{
private:
	DWORD timer_start;
	DWORD stopwatch_start;

public:
	Timer(void);
	~Timer(void);
	DWORD getTimer();
	DWORD getStartTimeMillis();

	void sleep(int ms);
	void reset();
	bool stopwatch(int ms);
};

#endif
