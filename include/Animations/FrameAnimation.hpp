#ifndef FRAME_ANIMATION_HPP
#define FRAME_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <string>
//Test shit
class FrameAnimation
{
public:
    explicit FrameAnimation(const std::string& firstFrame);
    bool addFrame(const std::string& filename);
    void update(float deltaTime);    
    void draw(sf::RenderWindow& window);
    void setLooping(bool looping);
    void setLoopCount(int count);
    void play();
    void pause();
    void stop();
    void setFrameTime(float time);
    bool isFinished() const;
private:
    std::vector<sf::Texture> m_frames;
    sf::Sprite* m_sprite;
    size_t m_currentFrame{0};
    float m_frameTime{0.1f};
    float m_elapsedTime{0.0f};
    bool m_isLooping{true};
    bool m_isPlaying{true};
    int m_loopCount{0}; // 0 = infinite
    int m_currentLoop{0};
};

#endif //FRAME_ANIMATION_HPP