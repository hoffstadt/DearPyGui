#pragma once

#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvItemRegistry.h"
#include "mvCore.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlot, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlot, add_plot)
		MV_DEFAULT_PARENTS

		MV_CREATE_COMMAND(is_plot_queried);
		MV_CREATE_COMMAND(get_plot_query_area);

		//-----------------------------------------------------------------------------
		// Plot Marker Specifications
		//-----------------------------------------------------------------------------
		MV_CREATE_CONSTANT(mvPlotMarker_None    , -1L);  // no marker
		MV_CREATE_CONSTANT(mvPlotMarker_Circle  ,  0L);  // a circle marker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Square  ,  1L);  // a square maker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Diamond ,  2L);  // a diamond marker will be rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Up      ,  3L);  // an upward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Down    ,  4L);  // an downward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Left    ,  5L);  // an leftward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Right   ,  6L);  // an rightward-pointing triangle marker will up rendered at each point
		MV_CREATE_CONSTANT(mvPlotMarker_Cross   ,  7L);  // a cross marker will be rendered at each point (not filled)
		MV_CREATE_CONSTANT(mvPlotMarker_Plus    ,  8L);  // a plus marker will be rendered at each point (not filled)
		MV_CREATE_CONSTANT(mvPlotMarker_Asterisk,  9L); // a asterisk marker will be rendered at each point (not filled)

		MV_START_COMMANDS
			MV_ADD_COMMAND(is_plot_queried);
			MV_ADD_COMMAND(get_plot_query_area);
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvPlotMarker_None),
			MV_ADD_CONSTANT(mvPlotMarker_Circle),
			MV_ADD_CONSTANT(mvPlotMarker_Square),
			MV_ADD_CONSTANT(mvPlotMarker_Diamond),
			MV_ADD_CONSTANT(mvPlotMarker_Up),
			MV_ADD_CONSTANT(mvPlotMarker_Down),
			MV_ADD_CONSTANT(mvPlotMarker_Left),
			MV_ADD_CONSTANT(mvPlotMarker_Right),
			MV_ADD_CONSTANT(mvPlotMarker_Cross),
			MV_ADD_CONSTANT(mvPlotMarker_Plus),
			MV_ADD_CONSTANT(mvPlotMarker_Asterisk),
		MV_END_CONSTANTS

		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvPlotLegend),
			MV_ADD_CHILD(mvAppItemType::mvPlotAxis),
			MV_ADD_CHILD(mvAppItemType::mvDragPoint),
			MV_ADD_CHILD(mvAppItemType::mvDragLine),
			MV_ADD_CHILD(mvAppItemType::mvAnnotation),
			MV_ADD_CHILD(mvAppItemType::mvDrawLine),
			MV_ADD_CHILD(mvAppItemType::mvDrawArrow),
			MV_ADD_CHILD(mvAppItemType::mvDrawTriangle),
			MV_ADD_CHILD(mvAppItemType::mvDrawCircle),
			MV_ADD_CHILD(mvAppItemType::mvDrawEllipse),
			MV_ADD_CHILD(mvAppItemType::mvDrawBezierCubic),
			MV_ADD_CHILD(mvAppItemType::mvDrawBezierQuadratic),
			MV_ADD_CHILD(mvAppItemType::mvDrawQuad),
			MV_ADD_CHILD(mvAppItemType::mvDrawRect),
			MV_ADD_CHILD(mvAppItemType::mvDrawText),
			MV_ADD_CHILD(mvAppItemType::mvDrawPolygon),
			MV_ADD_CHILD(mvAppItemType::mvDrawPolyline),
			MV_ADD_CHILD(mvAppItemType::mvDrawImage),
			MV_ADD_CHILD(mvAppItemType::mvDrawLayer),
			MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
			MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
			MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
			MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
			MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
			MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
			MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
			MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
			MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
			MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
			MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
		MV_END_CHILDREN

	public:

		mvPlot(mvUUID uuid);

		void updateFlags();
		void updateAxesNames();

		// settings
		void SetColorMap    (ImPlotColormap colormap);
		void draw           (ImDrawList* drawlist, float x, float y) override;

		void addFlag         (ImPlotFlags flag);
		void removeFlag      (ImPlotFlags flag);

		[[nodiscard]] bool isPlotQueried() const;
		double* getPlotQueryArea();
		
		ImPlotFlags        getFlags         () const { return _flags; }

		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void postDraw() override;

	private:

		std::string     _xaxisName;
		std::string     _y1axisName;
		std::string     _y2axisName;
		std::string     _y3axisName;
		ImPlotFlags     _flags    = ImPlotFlags_NoLegend;
		ImPlotAxisFlags _xflags  = 0;
		ImPlotAxisFlags _yflags  = 0;
		ImPlotAxisFlags _y1flags  = 0;
		ImPlotAxisFlags _y2flags  = 0;			    
		bool            _newColorMap = false; // to bust color cache
		bool            _useColorMap = false;
		ImPlotColormap  _colormap = ImPlotColormap_Deep;			    
		bool            _equalAspectRatios = false;			    
		bool            _queried = false;
		double          _queryArea[4] = {0.0, 0.0, 0.0, 0.0};
		bool            _fitDirty = false;
		bool            _axisfitDirty[4] = { false, false, false, false };

		friend class mvPlotAxis;

		// custom input mapping
		ImPlotInputMap _originalMap = ImPlotInputMap();

		int _pan_button = 0;
		int _pan_mod = 0;
		int _fit_button = 0;
		int _context_menu_button = 0;
		int _box_select_button = 0;
		int _box_select_mod = 0;
		int _box_select_cancel_button = 0;
		int _query_button = 0;
		int _query_mod = 0;
		int _query_toggle_mod = 0;
		int _horizontal_mod = 0;
		int _vertical_mod = 0;
	
	};

}
