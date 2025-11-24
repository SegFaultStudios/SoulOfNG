#ifndef TIMER_HPP
#define TIMER_HPP

#include <SFML/System.hpp>

#include "Signal.hpp"

#include <functional>

class Timer
{
public:
    Signal<> timeout;

    Timer();
    
    void start(float durationSeconds);
    
    void stop();
    
    void reset();
    
    void restart();
    
    float getElapsedSeconds() const;
    
    float getRemainingSeconds() const;
    
    bool isRunning() const;
    bool isExpired() const;
    
    void update();
private:
    sf::Clock m_clock;
    sf::Time m_elapsedTime;
    sf::Time m_duration;
    bool m_isRunning;
};

#endif //TIMER_HPP