#include "mvToolWindow.h"
#include "mvItemRegistry.h"

void mvToolWindow::draw()
{
    if (!m_show)
        return;

    if (m_dirtySize)
    {
        ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
        m_dirtySize = false;
    }

    if (m_dirtyPos)
    {
        ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
        m_dirtyPos = false;
    }

    if (!ImGui::Begin(getTitle(), &m_show, m_windowflags))
    {
        ImGui::End();
        return;
    }

    drawWidgets();

    if (ImGui::IsWindowFocused())
    {

        float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

        // update mouse
        ImVec2 mousePos = ImGui::GetMousePos();
        float x = mousePos.x - ImGui::GetWindowPos().x;
        float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
        GContext->input.mousePos.x = (int)x;
        GContext->input.mousePos.y = (int)y;

        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);

        if (GContext->itemRegistry->activeWindow != getUUID())
            GContext->itemRegistry->activeWindow = getUUID();

    }

    ImGui::End();
}

void mvToolWindow::setXPos(int pos)
{
    m_xpos = pos;
    m_dirtyPos = true;
}

void mvToolWindow::setYPos(int pos)
{

    m_ypos = pos;
    m_dirtyPos = true;
}

void mvToolWindow::setWidth(int width)
{
    m_width = width;
    m_dirtySize = true;
}

void mvToolWindow::setHeight(int height)
{
    m_height = height;
    m_dirtySize = true;
}