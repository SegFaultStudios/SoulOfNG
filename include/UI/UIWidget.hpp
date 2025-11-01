#ifndef UI_WIDGET_HPP
#define UI_WIDGET_HPP

#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>

enum class CustomEvent
{
    HOVERED,
    LEAVE,
    CLICKED
};

class UIWidget : public sf::Drawable
{
public:
    using SharedPtr = std::shared_ptr<UIWidget>;
    using UniquePtr = std::unique_ptr<UIWidget>;
    using EventCallback = std::function<void()>;

    UIWidget(const std::string& name);
    virtual ~UIWidget();
    
    void setOnClick(const EventCallback& callback);
    void setOnHover(const EventCallback& callback);
    void setOnLeave(const EventCallback& callback);

    virtual void setPosition(const sf::Vector2f& position);
    void setVisible(bool isVisible);
    virtual void setSize(const sf::Vector2f& size);
    void setName(const std::string& name);

    const std::string& getName() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getSize() const;
    bool isVisible() const;

    void hide();
    void show();

    //Called for inherited widgets by UIWidget
    virtual void handleCustomEvent(CustomEvent event) {}
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void update(float deltaTime);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    virtual sf::FloatRect getBoundingBox() const = 0;

private:
    bool m_isVisible{true};
    bool m_isHovered{false};

    sf::Vector2f m_position{0.0f, 0.0f};
    sf::Vector2f m_size{2.0f, 2.0f};

    std::string m_name;

    EventCallback m_onClick{nullptr};
    EventCallback m_onHover{nullptr};
    EventCallback m_onLeave{nullptr};
};

#endif //UI_WIDGET_HPP