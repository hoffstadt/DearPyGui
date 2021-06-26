#pragma once

#include "mvTypeBases.h"
#include <implot.h>
#include <implot_internal.h>
#include <map>
#include <utility>
#include "mvCore.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlot, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvPlot : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlot, add_plot)

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

		//-----------------------------------------------------------------------------
		// Built-in ImPlot Color maps
		//-----------------------------------------------------------------------------
		MV_CREATE_CONSTANT(mvPlotColormap_Default,   0L); // ImPlot default colormap         (n=10)
		MV_CREATE_CONSTANT(mvPlotColormap_Deep   ,   1L); // a.k.a. matplotlib "Set1"        (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Dark   ,   2L); // a.k.a. matplotlib "Set1"        (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Pastel ,   3L); // a.k.a. matplotlib "Pastel1"     (n=9)
		MV_CREATE_CONSTANT(mvPlotColormap_Paired ,   4L); // a.k.a. matplotlib "Paired"      (n=12)
		MV_CREATE_CONSTANT(mvPlotColormap_Viridis,   5L); // a.k.a. matplotlib "viridis"     (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Plasma ,   6L); // a.k.a. matplotlib "plasma"      (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Hot    ,   7L); // a.k.a. matplotlib/MATLAB "hot"  (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Cool   ,   8L); // a.k.a. matplotlib/MATLAB "cool" (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Pink   ,   9L); // a.k.a. matplotlib/MATLAB "pink" (n=11)
		MV_CREATE_CONSTANT(mvPlotColormap_Jet    ,  10L); // a.k.a. MATLAB "jet"             (n=11)

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
		MV_ADD_CONSTANT(mvPlotColormap_Default),
		MV_ADD_CONSTANT(mvPlotColormap_Deep),
		MV_ADD_CONSTANT(mvPlotColormap_Dark),
		MV_ADD_CONSTANT(mvPlotColormap_Pastel),
		MV_ADD_CONSTANT(mvPlotColormap_Paired),
		MV_ADD_CONSTANT(mvPlotColormap_Viridis),
		MV_ADD_CONSTANT(mvPlotColormap_Plasma),
		MV_ADD_CONSTANT(mvPlotColormap_Hot),
		MV_ADD_CONSTANT(mvPlotColormap_Cool),
		MV_ADD_CONSTANT(mvPlotColormap_Pink),
		MV_ADD_CONSTANT(mvPlotColormap_Jet),
		MV_END_CONSTANTS

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
		
		ImPlotFlags        getFlags         () const { return m_flags; }

		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool canChildBeAdded(mvAppItemType type) override;
		void postDraw() override;

	private:

		std::string                   m_xaxisName;
		std::string                   m_y1axisName;
		std::string                   m_y2axisName;
		std::string                   m_y3axisName;
		ImPlotFlags                   m_flags    = ImPlotFlags_NoLegend;
		ImPlotAxisFlags               m_xflags  = 0;
		ImPlotAxisFlags               m_yflags  = 0;
		ImPlotAxisFlags               m_y1flags  = 0;
		ImPlotAxisFlags               m_y2flags  = 0;

		ImPlotColormap                m_colormap = ImPlotColormap_Deep;

		bool                          m_equalAspectRatios = false;

		bool                          m_queried = false;
		double                        m_queryArea[4] = {0.0, 0.0, 0.0, 0.0};
		bool                          m_dirty = false;

		bool m_fitDirty = false;
		bool m_axisfitDirty[4] = { false, false, false, false };

		friend class mvPlotAxis;
	
	};

}
