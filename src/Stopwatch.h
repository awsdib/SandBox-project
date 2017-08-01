#pragma once
#ifndef Foundation_Stopwatch_INCLUDED
#define Foundation_Stopwatch_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"


namespace Poco {

	// This class is used to measure the time needed for a flame particle to reach the house
	class Foundation_API Stopwatch
		/// A simple facility to measure time intervals
		/// with microsecond resolution.
		///
		/// Note that Stopwatch is based on the Timestamp
		/// class. Therefore, if during a Stopwatch run,
		/// the system time is changed, the measured time
		/// will not be correct.
	{
	public:
		Stopwatch();
		~Stopwatch();

		void start();
		/// Starts (or restarts) the stopwatch.

		void stop();
		/// Stops or pauses the stopwatch.

		void reset();
		/// Resets the stopwatch.

		void restart();
		/// Resets and starts the stopwatch.

		Timestamp::TimeDiff elapsed() const;
		/// Returns the elapsed time in microseconds
		/// since the stopwatch started.

		int elapsedSeconds() const;
		/// Returns the number of seconds elapsed
		/// since the stopwatch started.

		static Timestamp::TimeVal resolution();
		/// Returns the resolution of the stopwatch.

	private:
		Stopwatch(const Stopwatch&);
		Stopwatch& operator = (const Stopwatch&);

		Timestamp           _start;
		Timestamp::TimeDiff _elapsed;
		bool                _running;
	};


	//
	// inlines
	//
	inline void Stopwatch::start()
	{
		if (!_running)
		{
			_start.utcTime();
			_running = true;
		}
	}


	inline void Stopwatch::stop()
	{
		if (_running)
		{

			_elapsed = elapsed();
			cout << ": " << elapsed();
			_running = false;
		}
	}



	inline int Stopwatch::elapsedSeconds() const
	{
		Timestamp current;
		return int(elapsed() / resolution());

	}


	inline Timestamp::TimeVal Stopwatch::resolution()
	{
		return Timestamp::resolution();
	}


}


#endif 