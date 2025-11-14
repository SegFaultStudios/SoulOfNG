#ifndef LAYER_HPP
#define LAYER_HPP

#include <SFML/Graphics.hpp>
#include <memory>

class Layer
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void handleEvent(sf::Event& event) = 0;
    virtual void onStart() = 0;
    virtual void onEnd() = 0;
    virtual std::unique_ptr<Layer> getNextLayer() const {return nullptr;}

    bool isOver() const
    {
        return m_isOver;
    }

    virtual ~Layer() = default;
protected:
    void over()
    {
        m_isOver = true;
    };

private:
    //Yeah we need to re-make this shit
    bool m_isOver{false};
};

#endif //LAYER_HPP