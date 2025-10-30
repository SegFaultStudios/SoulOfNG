#include "Editor.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <cstring>

#include "Wall.hpp"

#include <iostream>
#include <stdexcept>

Editor::Editor(sf::RenderWindow& window, Scene& scene) : m_window(window), m_scene(scene)
{
    if(!ImGui::SFML::Init(m_window, true))
        throw std::runtime_error("failed to init sfml imgui");
}

void Editor::processEvents(sf::Event& event)
{
    if (const auto& keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyEvent->code == sf::Keyboard::Key::S && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
        {
            m_scene.saveToFile("./resources/maps/default_map.json");
            std::cout << "Scene saved\n";
        }
    }

    ImGui::SFML::ProcessEvent(m_window, event);
}

void Editor::update(sf::Time deltaTime)
{
    ImGui::SFML::Update(m_window, deltaTime);
}

void Editor::draw()
{
    drawHierarchy();
    drawSelectedEntityDetails();
    drawAddManager();

    ImGui::SFML::Render(m_window);
}

void Editor::drawAddManager()
{
    ImGui::Begin("Add");

    if(ImGui::Button("Add wall"))
    {
        m_selectedEntityId = m_scene.addEntity<Wall>("Wall");
    }

    ImGui::End();
}

void Editor::drawHierarchy()
{
    ImGui::Begin("Hierarchy");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));

    for(auto& [id, entity] : m_scene.getEntities())
    {
        ImGui::PushID(id);

        auto entityName = entity->getName().c_str();
        
        bool selected = (id == m_selectedEntityId);

        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

        if (selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = ImGui::TreeNodeEx(entityName, nodeFlags);

        if (ImGui::IsItemClicked())
            m_selectedEntityId = id;

        if(nodeOpen)
            ImGui::TreePop();

        ImGui::PopID();
    }

    ImGui::PopStyleVar(2);

    ImGui::End();

}

void Editor::drawSelectedEntityDetails()
{
    ImGui::Begin("Details");

    auto selectedEntity = m_scene.getEntity(m_selectedEntityId);

    if(!selectedEntity)
    {
        ImGui::End();
        return;
    }

    char buffer[128];
    std::strncpy(buffer, selectedEntity->getName().c_str(), sizeof(buffer));
    if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
        selectedEntity->setName(std::string(buffer));

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginTable("TransformTable", 2, ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Position");

            ImGui::TableSetColumnIndex(1);
            ImGui::PushID("Position");

            auto position = selectedEntity->getPosition();

            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(100,100,100,255));

            if (ImGui::Button("R"))
                position = sf::Vector2f(0, 0);
            
            ImGui::PopStyleColor();
            ImGui::SameLine();

            ImGui::DragFloat2("##Position", &position.x, 0.5f);

            selectedEntity->setPosition(position);

            ImGui::PopID();

            // ImGui::TableNextRow();
            // ImGui::TableSetColumnIndex(0);
            // ImGui::Text("Rotation");
            // ImGui::TableSetColumnIndex(1);
            // auto euler = selectedEntity->getRotation();
            // if (ImGui::DragFloat3("##Rotation", &euler.x, 0.5f))
            //     selectedEntity->setRotation(euler);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Scale");
            ImGui::TableSetColumnIndex(1);
            auto scale = selectedEntity->getScale();
            if (ImGui::DragFloat2("##Scale", &scale.x, 0.1f))
                selectedEntity->setScale(scale);

            ImGui::EndTable();
        }
    }

    ImGui::End();    
}