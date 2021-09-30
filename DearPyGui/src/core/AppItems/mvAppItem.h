/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all DearPygui widgets.
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvAppItemState.h"
#include "mvCallbackRegistry.h"
#include "mvPythonTranslator.h"
#include "cpp.hint"
#include "mvDefaultTheme.h"
#include <implot_internal.h>

namespace Marvel {

    // forward declarations
    class mvThemeManager;

    //-----------------------------------------------------------------------------
    // mvAppItemType
    //-----------------------------------------------------------------------------
    enum class mvAppItemType
    {
        None = -1, All, mvInputText, mvButton,
        mvRadioButton, mvTabBar, mvTab, mvImage, mvMenuBar, mvViewportMenuBar,
        mvMenu, mvMenuItem, mvChildWindow, mvGroup,
        mvSliderFloat, mvSliderInt, mvFilterSet,
        mvDragFloat, mvDragInt, mvInputFloat,
        mvInputInt, mvColorEdit, mvClipper,
        mvColorPicker, mvTooltip, mvCollapsingHeader,
        mvSeparator, mvCheckbox, mvListbox, mvText, mvCombo,
        mvPlot, mvSimplePlot, mvDrawlist, mvWindowAppItem,
        mvSelectable, mvTreeNode, mvProgressBar, mvSpacer,
        mvImageButton, mvTimePicker, mvDatePicker, mvColorButton,
        mvFileDialog, mvTabButton,
        mvNodeEditor, mvNode, mvNodeAttribute,
        mvTable, mvTableColumn, mvTableRow,
        mvDrawLine, mvDrawArrow, mvDrawTriangle,
        mvDrawCircle, mvDrawEllipse, mvDrawBezierCubic, mvDrawBezierQuadratic,
        mvDrawQuad, mvDrawRect, mvDrawText, mvDrawPolygon, mvDrawPolyline,
        mvDrawImage, mvDragFloatMulti, mvDragIntMulti, mvSliderFloatMulti,
        mvSliderIntMulti, mvInputIntMulti, mvInputFloatMulti,
        mvDragPoint, mvDragLine, mvAnnotation, mvLineSeries,
        mvScatterSeries, mvStemSeries, mvStairSeries, mvBarSeries,
        mvErrorSeries, mvVLineSeries, mvHLineSeries, mvHeatSeries,
        mvImageSeries, mvPieSeries, mvShadeSeries, mvLabelSeries,
        mvHistogramSeries, mv2dHistogramSeries,
        mvCandleSeries, mvAreaSeries, mvColorMapScale, mvSlider3D,
        mvKnobFloat, mvLoadingIndicator, mvNodeLink, 
        mvTextureRegistry, mvStaticTexture, mvDynamicTexture,
        mvStage, mvDrawLayer, mvViewportDrawlist,
        mvFileExtension, mvPlotLegend, mvPlotAxis,
        mvHandlerRegistry, mvKeyDownHandler, mvKeyPressHandler,
        mvKeyReleaseHandler, mvMouseMoveHandler, mvMouseWheelHandler,
        mvMouseClickHandler, mvMouseDoubleClickHandler, mvMouseDownHandler,
        mvMouseReleaseHandler, mvMouseDragHandler, 
        mvHoverHandler, mvActiveHandler, mvFocusHandler, mvVisibleHandler,
        mvEditedHandler, mvActivatedHandler, mvDeactivatedHandler, mvDeactivatedAfterEditHandler,
        mvToggledOpenHandler, mvClickedHandler, mvDragPayload, mvResizeHandler,
        mvFont, mvFontRegistry, mvTheme, mvThemeColor, mvThemeStyle, mvThemeComponent,
        mvFontRangeHint, mvFontRange, mvFontChars, mvCharRemap,
        mvValueRegistry, mvIntValue, mvFloatValue, mvFloat4Value,
        mvInt4Value, mvBoolValue, mvStringValue, mvDoubleValue, mvDouble4Value,
        mvColorValue, mvFloatVectValue, mvSeriesValue, mvRawTexture, mvSubPlots,
        mvColorMap, mvColorMapRegistry, mvColorMapButton, mvColorMapSlider,
        mvItemPool, mvItemSet, mvTemplateRegistry, mvTableCell, mvItemHandlerRegistry,
        ItemTypeCount
    };

    enum class StorageValueTypes
    {
        None = 0,
        Int, Int4, Float, Float4, FloatVect, 
        Double, Double4, DoubleVect,
        Series, Bool, String, UUID,
        Time, Color, Texture
    };

    enum ItemDescriptionFlags
    {
        MV_ITEM_DESC_DEFAULT     = 0,
        MV_ITEM_DESC_ROOT        = 1 << 1,
        MV_ITEM_DESC_CONTAINER   = 1 << 2,
        MV_ITEM_DESC_HANDLER     = 1 << 3 // todo: rename descriptively
    };

    enum class mvLibType {
        MV_IMGUI = 0,
        MV_IMPLOT = 1,
        MV_IMNODES = 2
    };

    template<int item_type> 
    struct mvItemTypeMap {};

    template<typename T> 
    struct mvItemTypeReverseMap{};

    struct ScopedID
    {
        ScopedID(void* id) { ImGui::PushID(id); }
        ScopedID(mvUUID id) { ImGui::PushID((int)id); }
        ~ScopedID() { ImGui::PopID(); }
    };

    //-----------------------------------------------------------------------------
    // mvAppItem
    //-----------------------------------------------------------------------------
    class mvAppItem
    {

    public:

        mvAppItem(mvUUID uuid);
        mvAppItem(const mvAppItem& other) = delete; // copy constructor
        mvAppItem(mvAppItem&& other)      = delete; // move constructor

        virtual ~mvAppItem();

        //-----------------------------------------------------------------------------
        // These methods are overridden through the use of:
        //   - MV_REGISTER_WIDGET
        //   - MV_APPLY_WIDGET_REGISTRATION
        //-----------------------------------------------------------------------------
        [[nodiscard]] virtual mvRef<mvAppItem>  getClassThemeComponent() const = 0;
        [[nodiscard]] virtual mvAppItemType     getType      () const { return mvAppItemType::None; } // should be pure, see #1071
        [[nodiscard]] virtual int               getDescFlags () const = 0;
        [[nodiscard]] virtual int               getTarget    () const = 0; // which child slot
        [[nodiscard]] virtual StorageValueTypes getValueType () const = 0;
        [[nodiscard]] virtual const char*       getCommand   () const = 0;
        [[nodiscard]] virtual const char*       getTypeString() const = 0;
        [[nodiscard]] virtual int               getApplicableState() const = 0;
        [[nodiscard]] virtual const std::vector<std::pair<std::string, int>>& getAllowableParents() const = 0;
        [[nodiscard]] virtual const std::vector<std::pair<std::string, int>>& getAllowableChildren() const = 0;

        // actual immediate mode drawing instructions
        virtual void draw(ImDrawList* drawlist, float x, float y) = 0;

        //-----------------------------------------------------------------------------
        // usually used for iterating through items and performing an action
        // other than drawing.
        //-----------------------------------------------------------------------------
        virtual void customAction(void* data = nullptr) {};
        virtual void alternativeCustomAction(void* data = nullptr) {};

        //-----------------------------------------------------------------------------
        // shows information in either the debug window or a separate window
        //-----------------------------------------------------------------------------
        virtual void renderSpecificDebugInfo() {} // for the debug tool

        //-----------------------------------------------------------------------------
        // These methods handle setting the widget's value using PyObject*'s or
        // returning the actual value. These are mostly overridden by the
        // mvTypeBase classes
        //-----------------------------------------------------------------------------
        virtual void*     getValue() { return nullptr; }
        virtual PyObject* getPyValue() { return GetPyNone(); }
        virtual void      setPyValue(PyObject* value) { }

        // used to check arguments, get/set configurations
        void handleKeywordArgs(PyObject* dict, std::string parser);  // python dictionary acts as an out parameter 
        void applyTemplate    (mvAppItem* item);

        // used by derived items to register their arguments
        virtual void handleSpecificRequiredArgs  (PyObject* args)  {}
        virtual void handleSpecificPositionalArgs(PyObject* args)  {}
        virtual void handleSpecificKeywordArgs   (PyObject* dict)  {} // called by handleKeywordArgs
        virtual void getSpecificConfiguration    (PyObject* dict)  {}
        virtual void applySpecificTemplate       (mvAppItem* item) {}

        //-----------------------------------------------------------------------------
        // These methods can be optionally overridden if your widget needs to be
        // notified when children are added/removed (i.e. tables, node editor)
        //-----------------------------------------------------------------------------
        virtual void onChildAdd    (mvRef<mvAppItem> item) {}
        virtual void onChildRemoved(mvRef<mvAppItem> item) {}
        virtual void onChildrenRemoved() {}
        virtual void onBind(mvAppItem* item) {}

        //-----------------------------------------------------------------------------
        // callbacks
        //-----------------------------------------------------------------------------
        [[nodiscard]] PyObject* getCallback(bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
       
        //-----------------------------------------------------------------------------
        // config setters
        //-----------------------------------------------------------------------------
        virtual void setDataSource(mvUUID value);

        mvAppItem*       getChild(mvUUID uuid);      // will return nullptr if not found
        mvRef<mvAppItem> getChildRef(mvUUID uuid);      // will return nullptr if not found

        // runtime modifications
        bool             addItem(mvRef<mvAppItem> item);
        bool             addRuntimeChild(mvUUID parent, mvUUID before, mvRef<mvAppItem> item);
        bool             addChildAfter(mvUUID prev, mvRef<mvAppItem> item);
        bool             deleteChild(mvUUID uuid);
        bool             moveChildUp(mvUUID uuid);
        bool             moveChildDown(mvUUID uuid);
        mvRef<mvAppItem> stealChild(mvUUID uuid); // steals a child (used for moving)
       
    public:

        mvUUID         _uuid = 0;
        std::string    _internalLabel; // label passed into imgui
        mvAppItem*     _parentPtr = nullptr;
        mvAppItemState _state;
        int            _location = -1;
        bool           _showDebug = false;
        
        // item pool info
        mvUUID _pool = 0;
        mvUUID _itemSet = 0;
        
        // next frame triggers
        bool _focusNextFrame = false;
        bool _triggerAlternativeAction = false;
        bool _shownLastFrame = false;
        bool _hiddenLastFrame = false;
        bool _enabledLastFrame = false;
        bool _disabledLastFrame = false;

        // previous frame cache
        ImVec2 _previousCursorPos = { 0.0f, 0.0f };

        // dirty flags
        bool _dirty_size = true;
        bool _dirtyPos = false;

        // slots
        //   * 0 : mvFileExtension, mvFontRangeHint, mvNodeLink, mvAnnotation
        //         mvDragLine, mvDragPoint, mvLegend, mvTableColumn
        //   * 1 : Most widgets
        //   * 2 : Draw Commands
        //   * 3 : mvDragPayload
        std::vector<mvRef<mvAppItem>> _children[4] = { {}, {}, {}, {} };

        // item handler registry
        mvRef<mvAppItem> _handlerRegistry = nullptr;

        // font
        mvRef<mvAppItem> _font = nullptr;

        // theme
        mvRef<mvAppItem> _theme = nullptr;
        mvRef<mvAppItem> _themeComponent = nullptr;

        // drag & drop
        PyObject* _dragCallback = nullptr;
        PyObject* _dropCallback = nullptr;
        std::string _payloadType = "$$DPG_PAYLOAD";

        // config
        mvUUID      _source = 0;
        std::string _specifiedLabel;
        mvUUID      _parent = 0;
        mvUUID      _before = 0;
        std::string _filter;
        int         _width = 0;
        int         _height = 0;
        float       _indent = -1.0f;
        bool        _show = true;
        bool        _enabled = true;
        PyObject*   _callback = nullptr;
        PyObject*   _user_data = nullptr;
        bool        _tracked = false;
        float       _trackOffset = 0.5f; // 0.0f:top, 0.5f:center, 1.0f:bottom
        bool        _searchLast = false;
        bool        _searchDelayed = false;
        bool        _useInternalLabel = true; // when false, will use specificed label
        std::string _alias;

    };

}
