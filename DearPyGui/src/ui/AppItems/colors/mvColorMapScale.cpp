#include "mvColorMapScale.h"
#include <utility>
#include "mvContext.h"
#include "dearpygui.h"
#include <string>
#include "mvItemRegistry.h"
#include <implot.h>
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    mvColorMapScale::mvColorMapScale(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvColorMapScale::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvColorMapScale*>(item);
        _scale_min = titem->_scale_min;
        _scale_max = titem->_scale_max;
        _colormap = titem->_colormap;
    }

    void mvColorMapScale::setColorMap(ImPlotColormap colormap)
    {
        _colormap = colormap;
    }

    void mvColorMapScale::draw(ImDrawList* drawlist, float x, float y)
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

            ImPlot::ColormapScale(_internalLabel.c_str(), _scale_min, _scale_max, ImVec2((float)_width, (float)_height), _colormap);
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

    void mvColorMapScale::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "min_scale")) _scale_min = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "max_scale")) _scale_max = (double)ToFloat(item);
        if (PyObject* item = PyDict_GetItemString(dict, "colormap"))
        {
            _colormap= (ImPlotColormap)GetIDFromPyObject(item);
            if (_colormap > 10)
            {
                auto asource = GetItem(*GContext->itemRegistry, _colormap);
                if (asource == nullptr)
                {
                    mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_colormap",
                        "Source Item not found: " + std::to_string(_colormap), nullptr);
                    _colormap = 0;
                }

                else if (asource->_type == mvAppItemType::mvColorMap)
                {
                    mvColorMap* colormap = static_cast<mvColorMap*>(asource);
                    _colormap = colormap->getColorMap();
                }
            }
        }
    }

    void mvColorMapScale::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "min_scale", mvPyObject(ToPyFloat((float)_scale_min)));
        PyDict_SetItemString(dict, "max_scale", mvPyObject(ToPyFloat((float)_scale_max)));

    }

}
