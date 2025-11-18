#ifndef UI_WIDGET_HPP
#define UI_WIDGET_HPP

#include <SFML/Graphics.hpp>

#include "Signal.hpp"

#include <memory>
#include <functional>

class UIWidget : public sf::Drawable
{
public:
    using SharedPtr = std::shared_ptr<UIWidget>;
    using UniquePtr = std::unique_ptr<UIWidget>;

    Signal<> clicked;
    Signal<> hovered;
    Signal<> left;

    explicit UIWidget(const std::string& name, UIWidget* parent = nullptr);
    virtual ~UIWidget();

    void setVisible(bool isVisible);
    void setName(const std::string& name);

    virtual void setPosition(const sf::Vector2f& position);
    virtual void setSize(const sf::Vector2f& size);

    const std::string& getName() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getSize() const;
    bool isVisible() const;

    void hide();
    void show();

    virtual sf::FloatRect getBoundingBox() const;
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void update(float deltaTime);
protected:
    enum class CustomEvent
    {
        HOVERED,
        LEAVE,
        CLICKED,
        LEAVE_CLICK,
    };

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    //Called for inherited widgets by UIWidget
    virtual void handleCustomEvent(CustomEvent event) {}
private:
    UIWidget* m_parent{nullptr};
    std::vector<UIWidget*> m_children;

    bool m_isVisible{true};
    bool m_isHovered{false};
    bool m_wasHoveredBeforeClick{false};

    sf::Vector2f m_position{0.0f, 0.0f};
    sf::Vector2f m_size{10.0f, 10.0f};

    std::string m_name;
};

#endif //UI_WIDGET_HPP