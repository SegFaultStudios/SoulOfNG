#ifndef UI_LAYOUT_HPP
#define UI_LAYOUT_HPP

#include <vector>

class UIWidget;

class UILayout
{
private:
    UILayout(UIWidget* parent = nullptr) : m_parent(parent) 
    {

    }

    void addUIWidget(UIWidget* child)
    {
        m_children.push_back(child);
    }

    virtual ~UILayout() = default;

    bool isParentValid() const
    {
        return m_parent != nullptr;
    }

    UIWidget* getParent() const
    {
        return m_parent;
    }

private:
    UIWidget* m_parent{nullptr};
    std::vector<UIWidget*> m_children;

    float m_spacing{5.0f};
    float m_margin{5.0f};
};

#endif //UI_LAYOUT_HPP