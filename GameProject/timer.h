#pragma once

/** Implements a timer for use in your game. You set the period and the timer mode
*   and the timer instance reports a value in [0.0,1.0] corresponding
*   to the normalized period.
*/

class Timer
{
public:
	enum timer_type_t {
		TIMER_ONCE,			/// Run once and stop
		TIMER_LOOPING,      /// Continuously loop from 0 to 1
		TIMER_PINGPONG		/// Continuously loop from 0 to 1 and back to 0
	};

private:
	float m_val = 0.0f;
	float m_period;
	float m_time_start = 0.0f;
	timer_type_t m_type;
	bool  m_running = false;
	bool  m_pause = false;
	bool  m_pingpong_descending = false;
public:
	/** Ctor.
	*	\param period is the time interval to map to the normalized output range, in seconds
	*	\param type is the timer mode.
	*/
	Timer(float period = 1.0f, timer_type_t type = TIMER_ONCE);

	/** Starts the timer.
	*/
	void start();

	/** Stops the timer.
	*/
	void stop();

	/** Pauses thetimer.
	*	\param p: if true, the timer is paused, or otherwise un-paused.
	*/
	void pause(bool p);

	/**	Checks if the timer is counting.
	*	\return true if the timer has not finished counting or is not stopped, false otherwise.
	*/
	bool isRunning() { return m_running; }

	/** Casting operator. Updates and reports the current normalized interval value.
	*   \return the current normalized (output) value of the timer.
	*/
	operator float();
};
