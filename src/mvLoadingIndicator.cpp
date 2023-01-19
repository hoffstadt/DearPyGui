#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"
#include "mvFontItems.h"
#include "mvThemes.h"
#include "mvContainers.h"
#include "mvItemHandlers.h"

void mvLoadingIndicator::draw(ImDrawList* drawlist, float x, float y)
{

    //-----------------------------------------------------------------------------
    // pre draw
    //-----------------------------------------------------------------------------

    // show/hide
    if (!config.show)
        return;

    // focusing
    if (info.focusNextFrame)
    {
        ImGui::SetKeyboardFocusHere();
        info.focusNextFrame = false;
    }

    // cache old cursor position
    ImVec2 previousCursorPos = ImGui::GetCursorPos();

    // set cursor position if user set
    if (info.dirtyPos)
        ImGui::SetCursorPos(state.pos);

    // update widget's position state
    state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

    // set item width
    if (config.width != 0)
        ImGui::SetNextItemWidth((float)config.width);

    // set indent
    if (config.indent > 0.0f)
        ImGui::Indent(config.indent);

    // push font if a font object is attached
    if (font)
    {
        ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
        ImGui::PushFont(fontptr);
    }

    // themes
    apply_local_theming(this);

    //-----------------------------------------------------------------------------
    // draw
    //-----------------------------------------------------------------------------
    {
        ScopedID id(uuid);

        if (_style == 0)
            LoadingIndicatorCircle(config.specifiedLabel.c_str(), _radius, _mainColor, _optionalColor, _circleCount, _speed);
        else
            LoadingIndicatorCircle2(config.specifiedLabel.c_str(), _radius, _thickness, _mainColor);
    }

    //-----------------------------------------------------------------------------
    // update state
    //-----------------------------------------------------------------------------
    UpdateAppItemState(state);

    //-----------------------------------------------------------------------------
    // post draw
    //-----------------------------------------------------------------------------

    // set cursor position to cached position
    if (info.dirtyPos)
        ImGui::SetCursorPos(previousCursorPos);

    if (config.indent > 0.0f)
        ImGui::Unindent(config.indent);

    // pop font off stack
    if (font)
        ImGui::PopFont();

    // handle popping themes
    cleanup_local_theming(this);

    if (handlerRegistry)
        handlerRegistry->checkEvents(&state);

    // handle drag & drop if used
    apply_drag_drop(this);
}

void mvLoadingIndicator::handleSpecificKeywordArgs(PyObject* dict)
{
    if (dict == nullptr)
        return;

    if (PyObject* item = PyDict_GetItemString(dict, "style")) _style = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "circle_count")) _circleCount = ToInt(item);
    if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
    if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
    if (PyObject* item = PyDict_GetItemString(dict, "speed")) _speed = ToFloat(item);
    if (PyObject* item = PyDict_GetItemString(dict, "color")) _mainColor = ToColor(item);
    if (PyObject* item = PyDict_GetItemString(dict, "secondary_color")) _optionalColor = ToColor(item);
}

void mvLoadingIndicator::getSpecificConfiguration(PyObject* dict)
{
    if (dict == nullptr)
        return;

    PyDict_SetItemString(dict, "style", mvPyObject(ToPyInt(_style)));
    PyDict_SetItemString(dict, "circle_count", mvPyObject(ToPyInt(_circleCount)));
    PyDict_SetItemString(dict, "radius", mvPyObject(ToPyFloat(_radius)));
    PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
    PyDict_SetItemString(dict, "speed", mvPyObject(ToPyFloat(_speed)));
    PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_mainColor)));
    PyDict_SetItemString(dict, "secondary_color", mvPyObject(ToPyColor(_optionalColor)));

}