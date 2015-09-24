#ifndef ESOTERIC_ATime_h
#define ESOTERIC_ATime_h

#include "numbers.h"
#include <OS.h>

//=======================================================
// class: ATime
//
//	This is the base class for any measure of time.  The
//  base representation of time is seconds.  Thus whenever
//	one time is constructed from another, the data is 
//	converted to seconds when the base() method is called.
//
//=======================================================
class ATime
{
public:
			ATime();
			ATime(const double aValue);
			ATime(const ATime& other);
	virtual	~ATime();
	
		ATime&		operator= (const ATime& other);
		ATime&		operator- (const ATime& other);
	
		void			SetSeconds(const double secs);
		const double	GetSeconds() const;

	double	fSeconds;

protected:	
	
private:
};

//=======================================================
//
//=======================================================

class seconds : public ATime
{
public:
		seconds(const double aValue=0):ATime(aValue){};
		seconds(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds; };

protected:

private:
};


//=======================================================
//
//=======================================================
class milliseconds : public ATime
{
public:
		milliseconds(const double aValue=0):ATime(aValue/kkilo){};
		milliseconds(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds*kkilo; };

protected:

private:
};


//=======================================================
//
//=======================================================
class microseconds : public ATime
{
public:
		microseconds(const double aValue=0):ATime(aValue/kmega){};
		microseconds(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds*kmega; };

protected:

private:
};

//=======================================================
//
//=======================================================
class nanoseconds : public ATime
{
public:
		nanoseconds(const double aValue=0):ATime(aValue/kgiga){};
		nanoseconds(const ATime& other) {fSeconds = other.fSeconds;};

		operator double() { return fSeconds*kgiga; };
	
protected:

private:
};

//=======================================================
//
//=======================================================
class picoseconds : public ATime
{
public:
		picoseconds(const double aValue=0):ATime(aValue/ktera){};
		picoseconds(const ATime& other) {fSeconds = other.fSeconds;};

		operator double() { return fSeconds*ktera; };

protected:

private:
};

//=======================================================
//
//=======================================================
class minutes : public ATime
{
public:
		minutes(const double aValue=0):ATime(aValue*60){};
		minutes(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds/60; };

protected:

private:
};

//=======================================================
//
//=======================================================
class hours : public ATime
{
public:
		hours(const double aValue=0):ATime(aValue*3600){};
		hours(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds/3600; };

protected:

private:
};

//=======================================================
//
//=======================================================
class days : public ATime
{
public:
		days(const double aValue=0):ATime(aValue*86400){};
		days(const ATime& other) {fSeconds = other.fSeconds;};
	
		operator double() { return fSeconds/86400; };

protected:

private:
};


//=======================================================
// Clocks
//
// Clocks know what time it is.
//=======================================================
class AClock
{
public:
	virtual	void	Now(ATime&)=0;
	virtual void	DelayUntil(ATime&)=0;
	virtual	void	DelayFor(ATime&)=0;
};

class ASystemClock: public AClock
{
public:
	virtual	void	Now(ATime&);
	virtual void	DelayUntil(ATime&);
	virtual	void	DelayFor(ATime&);
	
};

//=======================================================
// Class: AStopWatch
//
//	Provides simple timing of events.
//=======================================================
class AStopWatch
{
public:
				AStopWatch();
				AStopWatch(AClock *aClock);
	virtual		~AStopWatch();
				
		void	Start();
		void	Stop();
		void	Reset();
		void	GetElapsedTime(ATime&);
		void	GetElapsedTimeAndReset(ATime&);

protected:
	AClock			*fClock;
	int				fOwnsClock;
	microseconds	fStartTime;
	microseconds	fStopTime;
	microseconds	fElapsedTime;
};

//=======================================================
// Class: ATicker
//
//	A thread that provides a tick event on a timed basis
//=======================================================

// Tick rates for SMPTE time base
const double kSMPTE24fps = 24.0;
const double kSMPTE25fps = 25.0;
const double kSMPTE30fpsdrop = 29.97002617;
const double kSMPTE30fps = 30.0;


// MIDI tick rate
const double kMIDIRate = kmega;

class ATicker
{
public:
			ATicker(ATime &tickInterval);		// Interval between ticks
			ATicker(const double tickRate);		// Number of times to tick a second
	virtual	~ATicker();
	
	virtual	void	Tick(const double tickCount);
	
			void	GetInterval(ATime &interval);
			

protected:
	microseconds	fInterval;

private:
			// Don't allow default construction?
			ATicker();
	
	thread_id	fThreadID;
};

#endif
