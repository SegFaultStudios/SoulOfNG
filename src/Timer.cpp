#include "Timer.hpp"

Timer::Timer() : m_elapsedTime(sf::Time::Zero), m_duration(sf::Time::Zero), m_isRunning(false)
{
}

void Timer::start(float durationSeconds)
{
    m_duration = sf::seconds(durationSeconds);
    m_elapsedTime = sf::Time::Zero;

    m_clock.restart();
    m_isRunning = true;
}

void Timer::update()
{
    if (!m_isRunning)
        return;

    float elapsed = getElapsedSeconds();

    if (elapsed >= m_duration.asSeconds())
    {
        timeout.emit(); 
        restart();
    }
}

void Timer::stop()
{
    if (m_isRunning)
    {
        m_elapsedTime += m_clock.getElapsedTime();
        m_isRunning = false;
    }
}

void Timer::reset()
{
    m_elapsedTime = sf::Time::Zero;
    m_isRunning = false;
}

void Timer::restart()
{
    m_elapsedTime = sf::Time::Zero;
    m_clock.restart();
    m_isRunning = true;
}

bool Timer::isRunning() const
{
    return m_isRunning;
}

bool Timer::isExpired() const
{
    if (!m_isRunning)
        return false;

    return getElapsedSeconds() >= m_duration.asSeconds();
}

float Timer::getElapsedSeconds() const
{
    if (m_isRunning)
        return (m_elapsedTime + m_clock.getElapsedTime()).asSeconds();

    return m_elapsedTime.asSeconds();
}

float Timer::getRemainingSeconds() const
{
    float remaining = m_duration.asSeconds() - getElapsedSeconds();
    return remaining < 0 ? 0 : remaining;
}