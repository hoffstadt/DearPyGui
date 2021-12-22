#include "mvKnob.h"
#include "mvKnobCustom.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

namespace Marvel {

    mvKnobFloat::mvKnobFloat(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvKnobFloat::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvKnobFloat*>(item);
        if (config.source != 0) _value = titem->_value;
        _disabled_value = titem->_disabled_value;
        _min = titem->_min;
        _max = titem->_max;
        _step = titem->_step;
    }

    void mvKnobFloat::draw(ImDrawList* drawlist, float x, float y)
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

        if (KnobFloat(config.specifiedLabel.c_str(), _value.get(), _min, _max, _step))
        {
            auto value = *_value;
            mvSubmitCallback([=]() {
                if(config.alias.empty())
                    mvAddCallback(getCallback(false), uuid, ToPyFloat(value), config.user_data);
                else
                    mvAddCallback(getCallback(false), config.alias, ToPyFloat(value), config.user_data);
                });
        }
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

    PyObject* mvKnobFloat::getPyValue()
    {
        return ToPyFloat(*_value);
    }

    void mvKnobFloat::setPyValue(PyObject* value)
    {
        *_value = ToFloat(value);
    }

    void mvKnobFloat::setDataSource(mvUUID dataSource)
    {
        if (dataSource == config.source) return;
        config.source = dataSource;

        mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
        if (!item)
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
                "Source item not found: " + std::to_string(dataSource), this);
            return;
        }
        if (GetEntityValueType(item->type) != GetEntityValueType(type))
        {
            mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
                "Values types do not match: " + std::to_string(dataSource), this);
            return;
        }
        _value = *static_cast<std::shared_ptr<float>*>(item->getValue());
    }

    void mvKnobFloat::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_value")) _min = ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_value")) _max = ToFloat(item);
    }

    void mvKnobFloat::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "min_scale", mvPyObject(ToPyFloat(_min)));
        PyDict_SetItemString(dict, "max_scale", mvPyObject(ToPyFloat(_max)));

    }

}