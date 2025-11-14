#ifndef UI_TABLE_WIDGET_HPP
#define UI_TABLE_WIDGET_HPP

#include "UI/UIWidget.hpp"
#include "UI/UIText.hpp"

#include <vector>
#include <functional>

class UITableWidget : public UIWidget
{
public:
    UITableWidget(const std::string& name);

    void setColumnCount(std::size_t count);
    void setRowHeight(float height);
    void setColumnWidth(std::size_t index, float width);
    void setHeaderLabels(const std::vector<std::string>& labels);

    std::size_t addRow(const std::vector<std::string>& cells);
    void clearRows();

    void setOnRowClick(const std::function<void(std::size_t)>& callback);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(float dt) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
private:
    struct Cell 
    {
        UIText text;
        sf::Vector2f finalPos;
    };

    struct Row 
    {
        std::vector<Cell> cells;
        sf::FloatRect boundingBox;
    };

    std::vector<float> m_colWidths;
    std::vector<Row> m_rows;

    float m_rowHeight{30.f};
    float m_headerHeight{30.f};

    std::function<void(std::size_t)> m_onRowClick;
    std::vector<UIText> m_headers;

    sf::RectangleShape m_background;
};

#endif //UI_TABLE_WIDGET_HPP