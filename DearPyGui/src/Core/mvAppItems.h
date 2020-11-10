#pragma once

#include "Core/AppItems/mvInputText.h"
#include "Core/AppItems/mvContainerItems.h"
#include "Core/AppItems/mvBasicItems.h"
#include "Core/AppItems/mvInputItems.h"
#include "Core/AppItems/mvSliderItems.h"
#include "Core/AppItems/mvDragItems.h"
#include "Core/AppItems/mvTab.h"
#include "Core/AppItems/mvMenu.h"
#include "Core/AppItems/mvMiscItems.h"
#include "Core/AppItems/mvTooltip.h"
#include "Core/AppItems/mvColorItems.h"
#include "Core/AppItems/mvText.h"
#include "Core/AppItems/mvWindowAppItem.h"
#include "Core/AppItems/mvPopup.h"
#include "Core/AppItems/mvDrawing.h"
#include "Core/AppItems/mvImage.h"
#include "Core/AppItems/mvTable.h"
#include "Core/AppItems/mvTimePicker.h"
#include "Core/AppItems/mvDatePicker.h"
#include "Core/AppItems/mvColumns.h"
#include "Core/AppItems/mvLogger.h"
#include "Core/AppItems/mvAboutWindow.h"
#include "Core/AppItems/mvDocWindow.h"
#include "Core/AppItems/mvDebugWindow.h"
#include "Core/AppItems/mvMetricsWindow.h"
#include "Core/AppItems/mvStyleWindow.h"
#include "Core/AppItems/mvFileDialog.h"

#include "Core/PlotAppItems/mvPlot.h"
#include "Core/PlotAppItems/mvSimplePlot.h"
#include "Core/PlotAppItems/mvLineSeries.h"
#include "Core/PlotAppItems/mvBarSeries.h"
#include "Core/PlotAppItems/mvScatterSeries.h"
#include "Core/PlotAppItems/mvAreaSeries.h"
#include "Core/PlotAppItems/mvStemSeries.h"
#include "Core/PlotAppItems/mvLabelSeries.h"
#include "Core/PlotAppItems/mvPieSeries.h"
#include "Core/PlotAppItems/mvShadeSeries.h"
#include "Core/PlotAppItems/mvErrorSeries.h"
#include "Core/PlotAppItems/mvHeatSeries.h"
#include "Core/PlotAppItems/mvImageSeries.h"
#include "Core/PlotAppItems/mvStairSeries.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     - mvBasicItems.h
//         * mvSelectable
//         * mvButton
//         * mvCheckbox
//         * mvCombo
//         * mvListbox
//         * mvRadioButton
//         * mvProgressBar
//         * mvInputInt
//         * mvInputIntMulti
//         * mvInputFloat
//         * mvInputFloatMulti
//         * mvDragFloat
//         * mvDragInt
//         * mvSliderFloat
//         * mvSliderInt
//         * mvSliderFloatMulti
//         * mvSliderIntMulti
//
//     - mvColorItems.h
//         * mvColorEdit3
//         * mvColorEdit4
//         * mvColorPicker3
//         * mvColorPicker4
//
//     - mvContainerItems.h
//         * mvChild
//         * mvGroup
//         * mvCollapsingHeader
//         * mvTreeNode
//
//     - mvTimePicker.h
//         * mvTimePicker
//
//     - mvDatePicker.h
//         * mvDatePicker
//
//     - mvDrawing.h
//         * mvDrawing
//
//     - mvInputText.h
//         * mvInputText
//
//     - mvMenu.h
//         * mvMenuBar
//         * mvMenu
//         * mvMenuItem
//
//     - mvMiscItems.h
//         * mvSameLine
//         * mvSeparator
//         * mvSpacing
//         * mvIndent
//         * mvUnindent
//         * Dummy
//
//     - mvPlot.h
//         * mvPlot
//
//     - mvPieChart.h
//         * mvPieChart
//
//     - mvPopup.h
//         * mvPopup
//
//     - mvSimplePlot.h
//         * mvSimplePlot
//
//     - mvTab.h
//         * mvTabBar
//         * mvTab
//         * mvTabButton
//
//     - mvText.h
//         * mvText
//         * mvLabelText
//
//     - mvTooltip.h
//         * mvTooltip
//
//     - mvTable.h
//         * mvTable
//
//     - mvImage.h
//         * mvImage
//         * mvImageButton
//
//-----------------------------------------------------------------------------