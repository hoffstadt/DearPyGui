#include "mvLoadingIndicator.h"
#include "mvLoadingIndicatorCustom.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    mvLoadingIndicator::mvLoadingIndicator(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvLoadingIndicator::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvLoadingIndicator*>(item);
        _style = titem->_style;
        _circleCount = titem->_circleCount;
        _radius = titem->_radius;
        _speed = titem->_speed;
        _thickness = titem->_thickness;
        _mainColor = titem->_mainColor;
        _optionalColor = titem->_optionalColor;
    }

    void mvLoadingIndicator::draw(ImDrawList* drawlist, float x, float y)
    {

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------

        // show/hide
        if (!_show)
            return;

        // focusing
        if (_focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            _focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.pos);

        // update widget's position state
        _state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (_width != 0)
            ImGui::SetNextItemWidth((float)_width);

        // set indent
        if (_indent > 0.0f)
            ImGui::Indent(_indent);

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(_uuid);

            if (_style == 0)
                LoadingIndicatorCircle(_specifiedLabel.c_str(), _radius, _mainColor, _optionalColor, _circleCount, _speed);
            else
                LoadingIndicatorCircle2(_specifiedLabel.c_str(), _radius, _thickness, _mainColor);
        }

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------
        UpdateAppItemState(_state);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (_indent > 0.0f)
            ImGui::Unindent(_indent);

        // pop font off stack
        if (_font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

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

}