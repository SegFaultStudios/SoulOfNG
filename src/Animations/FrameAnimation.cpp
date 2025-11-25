#include "Animations/FrameAnimation.hpp"

#include "AssetsManager.hpp"
#include "Logger.hpp"
FrameAnimation::FrameAnimation(const std::string& firstFrame)
{
    addFrame(firstFrame);

    m_sprite = new sf::Sprite(m_frames[0]);
    m_sprite->setTexture(m_frames[0], true);

    m_sprite->setPosition({-60, -50});
}

bool FrameAnimation::addFrame(const std::string& filename) 
{
    sf::Texture texture;

    if (texture.loadFromFile(filename))
    {
        m_frames.push_back(texture);
        return true;
    }
    else
    {
        LOG_ERROR("Failed to load frame");
        return false;
    }
}

void FrameAnimation::update(float deltaTime) 
{
    if (m_frames.empty() || !m_isPlaying) return;
    
    m_elapsedTime += deltaTime;

    if (m_elapsedTime >= m_frameTime) 
    {
        m_currentFrame++;
        
        if (m_currentFrame >= m_frames.size())
        {
            m_currentLoop++;
            
            if (!m_isLooping || (m_loopCount > 0 && m_currentLoop >= m_loopCount)) 
            {
                m_currentFrame = m_frames.size() - 1;
                m_isPlaying = false;
            } else {
                m_currentFrame = 0;
            }
        }
        
        m_sprite->setTexture(m_frames[m_currentFrame], true);
        m_elapsedTime = 0.0f;
    }
}

void FrameAnimation::draw(sf::RenderWindow& window) {
    window.draw(*m_sprite);
}

void FrameAnimation::setLooping(bool looping) { m_isLooping = looping; }
void FrameAnimation::setLoopCount(int count) { m_loopCount = count; }
void FrameAnimation::play() { m_isPlaying = true; }
void FrameAnimation::pause() { m_isPlaying = false; }
void FrameAnimation::stop() { 
    m_isPlaying = false; 
    m_currentFrame = 0;
    m_currentLoop = 0;
    if (!m_frames.empty()) {
        m_sprite->setTexture(m_frames[0]);
    }
}

void FrameAnimation::setFrameTime(float time) { m_frameTime = time; }
bool FrameAnimation::isFinished() const { return !m_isPlaying && m_currentLoop >= m_loopCount && m_loopCount > 0; }
