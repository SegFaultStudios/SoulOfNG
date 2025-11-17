#include "UI/UITableWidget.hpp"

UITableWidget::UITableWidget(const std::string& name) : UIWidget(name)
{
    m_background.setFillColor(sf::Color(40, 40, 40, 180));
}

void UITableWidget::setColumnCount(std::size_t count)
{
    m_colWidths.assign(count, 100.f);
}

void UITableWidget::setRowHeight(float height)
{
    m_rowHeight = height;
}

void UITableWidget::setColumnWidth(std::size_t index, float width)
{
    if (index < m_colWidths.size())
        m_colWidths[index] = width;
}

std::size_t UITableWidget::addRow(const std::vector<std::string>& cells)
{
    Row& row = m_rows.emplace_back();
    
    row.cells.resize(m_colWidths.size());

    for (std::size_t i = 0; i < m_colWidths.size() && i < cells.size(); ++i)
    {
        row.cells[i].text.setText(cells[i]);
        row.cells[i].text.setCharacterSize(16);
    }

    return m_rows.size() - 1;
}

void UITableWidget::clearRows()
{
    m_rows.clear();
}

void UITableWidget::setHeaderLabels(const std::vector<std::string>& labels)
{
    m_headers.clear();
    m_headers.resize(labels.size());

    for (std::size_t i = 0; i < labels.size(); ++i)
    {
        m_headers[i].setText(labels[i]);
        m_headers[i].setCharacterSize(18);
    }
}

void UITableWidget::setOnRowClick(const std::function<void(std::size_t)>& callback)
{
    m_onRowClick = callback;
}

void UITableWidget::update(float dt)
{
    UIWidget::update(dt);

    m_background.setPosition(getPosition());
    m_background.setSize(getSize());

    float xStart = getPosition().x;
    float yStart = getPosition().y;

    float x = xStart;
    for (std::size_t i = 0; i < m_headers.size(); ++i)
    {
        m_headers[i].setPosition({x + 8.f, yStart + 5.f});
        x += m_colWidths[i];
    }

    float rowY = yStart + m_headerHeight;

    for (auto& row : m_rows)
    {
        row.boundingBox = sf::FloatRect({xStart, rowY}, {getSize().x, m_rowHeight});

        float cellX = xStart;
        for (std::size_t i = 0; i < row.cells.size(); ++i)
        {
            row.cells[i].finalPos = {cellX + 8.f, rowY + 5.f};
            cellX += m_colWidths[i];
        }

        rowY += m_rowHeight;
    }


    for (auto& row : m_rows)
    {
        for (std::size_t i = 0; i < row.cells.size(); ++i)
        {
            row.cells[i].text.setPosition(row.cells[i].finalPos);
        }
    }
}

void UITableWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    UIWidget::handleEvent(event, window);

    if (auto click = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (click->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(
                sf::Mouse::getPosition(window), window.getView()
            );

            for (std::size_t i = 0; i < m_rows.size(); ++i)
            {
                if (m_rows[i].boundingBox.contains(mouse))
                {
                    if (m_onRowClick) 
                        m_onRowClick(i);
                    break;
                }
            }
        }
    }
}

void UITableWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!isVisible())
        return;
    
    target.draw(m_background, states);

    float x = getPosition().x;

    sf::RectangleShape headerBg;
    headerBg.setFillColor(sf::Color(80, 80, 80));
    headerBg.setSize({0, m_headerHeight});

    for (std::size_t i = 0; i < m_headers.size(); ++i)
    {
        headerBg.setSize({m_colWidths[i], m_headerHeight});
        headerBg.setPosition({x, getPosition().y});
        target.draw(headerBg, states);

        m_headers[i].draw(target, states);

        x += m_colWidths[i];
    }

    // Draw rows
    for (auto& row : m_rows)
    {
        for (std::size_t i = 0; i < row.cells.size(); ++i)
        {
            // row.cells[i].text.setPosition(row.cells[i].finalPos);
            row.cells[i].text.draw(target, states);
        }
    }
}