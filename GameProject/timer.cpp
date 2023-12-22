#include "timer.h"
#include <sgg/graphics.h>

Timer::Timer(float period, timer_type_t type)
	: m_period(period), m_type(type)
{
}

void Timer::start()
{
	m_time_start = graphics::getGlobalTime() / 1000.f;
	m_pause = false;
	m_running = true;
}

void Timer::stop()
{
	m_pause = false;
	m_running = false;
}

void Timer::pause(bool p)
{
	if (!m_running)
		return;

	m_pause = p;

	if (!m_pause)
	{
		switch (m_type)
		{
		case TIMER_ONCE:
		case TIMER_LOOPING:
			m_time_start = graphics::getGlobalTime() / 1000.f - m_val * m_period;
			break;
		case TIMER_PINGPONG:
			m_time_start = graphics::getGlobalTime() / 1000.f - m_period * (m_pingpong_descending ? (2.0f - m_val) : m_val);
		}
	}
}

Timer::operator float()
{
	if (m_running && !m_pause)
	{
		switch (m_type)
		{
		case TIMER_ONCE:
			m_val = std::min(1.0f, (graphics::getGlobalTime() / 1000.f - m_time_start) / m_period);
			if (m_val == 1.0f)
			{
				m_running = false;
				m_pause = false;
			}
			break;
		case TIMER_LOOPING:
			m_val = fmodf(graphics::getGlobalTime() / 1000.f - m_time_start, m_period) / m_period;
			break;
		case TIMER_PINGPONG:
			m_val = fmodf(graphics::getGlobalTime() / 1000.f - m_time_start, 2.0f * m_period) / m_period;
			m_pingpong_descending = m_val > 1.0f;
			m_val = (m_val <= 1.0f ? m_val : 2.0f - m_val);
			break;
		}
	}

	return m_val;
}
