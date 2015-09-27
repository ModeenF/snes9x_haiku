#include "atime.h"
#include <OS.h>
#include <stdio.h>

ATime::ATime()
	: fSeconds(0.0)
{
}

ATime::ATime(const double aValue)
	: fSeconds(aValue)
{
}

ATime::ATime(const ATime& other)
	: fSeconds(other.fSeconds)
{
}

ATime::~ATime()
{}

ATime&
ATime::operator=(const ATime& other)
{
	fSeconds = other.fSeconds;
	return *this;
}

ATime&
ATime::operator-(const ATime& other)
{
	fSeconds -= other.fSeconds;
	return *this;
}

void
ATime::SetSeconds(const double secs)
{
	fSeconds = secs;
}

const double
ATime::GetSeconds() const
{
	return fSeconds;
}

//=======================================================
// Clocks
//
// Clocks know what time it is.
//=======================================================
void 
ASystemClock::Now(ATime& time)
{
	time = microseconds(system_time());
}

void
ASystemClock::DelayUntil(ATime& time)
{
	// Get the current time
	microseconds currentTime(system_time());
	
	// Subtract the passed in time
	microseconds diffTime = seconds(time.GetSeconds() - currentTime.GetSeconds());
	
	// if the value is > 0, then snooze
	// for the difference
	if ((double)diffTime > 0.0)
		snooze((bigtime_t)diffTime);
}

void
ASystemClock::DelayFor(ATime& time)
{
	// Add the amount of time passed in
	microseconds delayAmount(time);
	
	if ((double)delayAmount > 0)
		//snooze((double)delayAmount);
		snooze((bigtime_t)delayAmount);
}


//=======================================================
// AStopWatch
//
// A Simple Stop Watch
//=======================================================
//AStopWatch::AStopWatch()
//	: fStartTime(0.0),
//	fStopTime(0.0),
//	fOwnsClock(0),
//	fClock(0)
AStopWatch::AStopWatch()
	: fClock(0),
	fOwnsClock(0),
	fStartTime(0.0),
	fStopTime(0.0)
{
	fClock = new ASystemClock();
	fOwnsClock = 1;
}

//AStopWatch::AStopWatch(AClock *aClock)
//	: fStartTime(0.0),
//	fStopTime(0.0),
//	fOwnsClock(0),
//	fClock(0)
AStopWatch::AStopWatch(AClock *aClock)
	: fClock(0),
	fOwnsClock(0),
	fStartTime(0.0),
	fStopTime(0.0)
{
	fClock = aClock;
}

AStopWatch::~AStopWatch()
{
	if (fOwnsClock)
		delete fClock;
}

void	
AStopWatch::Start()
{
	// Get the current system time 
	fClock->Now(fStartTime);
}

void	
AStopWatch::Stop()
{
	fClock->Now(fStopTime);
}

void
AStopWatch::Reset()
{
	// Reset does an implied stop
	fStartTime = 0.0;
	fStopTime = fStartTime;
}

void
AStopWatch::GetElapsedTime(ATime& time)
{
	time = fStopTime - fStartTime;
}

void
AStopWatch::GetElapsedTimeAndReset(ATime& time)
{
	time = fStopTime - fStartTime;
	Reset();
}




//=======================================================
// Class: ATicker
//
//	A thread that provides a tick event on a timed basis
//=======================================================
static long
ATicker_thread(void *data)
{
	ATicker *ticker = (ATicker*)data;
	double nextTime;
	double snoozeTime;
	double baseTime;
	double tickCount = 0;
	microseconds tickInterval;
	
	ticker->GetInterval(tickInterval);
	
	baseTime = system_time();
	
	// Run continuously.  The only thing that stops this
	// is the death of the thread.
	while(1)
	{
		nextTime = baseTime + tickInterval;
		snoozeTime = (nextTime - system_time());
		
		while (snoozeTime >= 1.0)
		{
			snooze((bigtime_t)snoozeTime);
			
			snoozeTime = nextTime - system_time();
		}
		
		tickCount++;
		ticker->Tick(tickCount);
		
		baseTime = nextTime;
	}
	
	return 0;
}

ATicker::ATicker(ATime &tickInterval)
	: fInterval(tickInterval)
{
	fThreadID = spawn_thread(ATicker_thread,
		"ticker",B_NORMAL_PRIORITY,this);
	resume_thread(fThreadID);
}

ATicker::ATicker(const double tickRate)
{
	fInterval = seconds(1.0 / tickRate);
	
	// Number of times to tick a second
	fThreadID = spawn_thread(ATicker_thread,
		"ticker",B_NORMAL_PRIORITY,this);
	resume_thread(fThreadID);
}			

ATicker::~ATicker()
{
	// kill the thread
	kill_thread(fThreadID);
}

			
void	
ATicker::Tick(const double tickCount)
{
	printf("ATicker::Tick: %f\n", tickCount);
}

	
void
ATicker::GetInterval(ATime &interval)
{
	interval = fInterval;
}
