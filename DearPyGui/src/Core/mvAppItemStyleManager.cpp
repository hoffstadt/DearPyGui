#pragma once

#include "mvAppItemStyleManager.h"

namespace Marvel {

    mvAppItemStyleManagerScope::mvAppItemStyleManagerScope(mvAppItemStyleManager& manager)
        : m_manager(manager)
    {
       
    }

    mvAppItemStyleManagerScope::mvAppItemStyleManagerScope(mvAppItemStyleManagerScope&& other) noexcept
        : m_manager(other.m_manager), m_moved(true)
    {
        m_manager.pushColorStyles();
    }

    mvAppItemStyleManagerScope::~mvAppItemStyleManagerScope()
    {
        if(m_moved)
            m_manager.popColorStyles();
    }

    mvAppItemStyleManagerScope mvAppItemStyleManager::getScopedStyleManager()
    {
        return std::move(mvAppItemStyleManagerScope(*this));
    }

    void mvAppItemStyleManager::addColorStyle(ImGuiCol item, mvColor color)
    {
        m_colors.push_back({ item, color });
    }

    void mvAppItemStyleManager::pushColorStyles()
    {
        for (auto& item : m_colors)
            ImGui::PushStyleColor(item.idx, item.color);
    }

    void mvAppItemStyleManager::popColorStyles()
    {
        if (!m_colors.empty())
            ImGui::PopStyleColor((int)m_colors.size());
    }

    void mvAppItemStyleManager::clearColors()
    {
        m_colors.clear();
    }

}