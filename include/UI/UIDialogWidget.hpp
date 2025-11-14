#ifndef UI_DIALOG_WIDGET_HPP
#define UI_DIALOG_WIDGET_HPP

#include "UI/UIWidget.hpp"
#include "UI/UIText.hpp"

class UIDialogWidget : public UIWidget
{
public:
    enum class DialogResult
    {
        None,
        Accepted,
        Rejected
    };

    UIDialogWidget(const std::string& name);

    void open();
    void close();
    bool isOpen() const;

    //Blocks thread until inheried class calls close()
    DialogResult exec(sf::RenderWindow& window);

    DialogResult getResult() const;

    void setTitle(const std::string& title);
    void setSize(const sf::Vector2f& size) override;
    void setPosition(const sf::Vector2f& position) override;

    void update(float deltaTime) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

protected:
    virtual void onAccept() {}
    virtual void onReject() {}
    void setResult(DialogResult result);

private:
    void beginDrag(const sf::Vector2f& mouse);
    void updateDrag(const sf::Vector2f& mouse);

private:
    sf::RectangleShape m_background;
    sf::RectangleShape m_titleBar;
    UIText m_titleText;

    bool m_open = false;
    bool m_dragging = false;
    sf::Vector2f m_dragOffset;

    DialogResult m_result = DialogResult::None;
};

#endif //UI_DIALOG_WIDGET_HPP