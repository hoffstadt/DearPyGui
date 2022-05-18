#pragma once

// still WIP

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct mvAppItemInfo;
struct mvAppItemConfig;
struct mvAppItemDrawInfo;
struct mvAppItemState;
struct ScopedID;
struct mvItemRegistry;
struct mvCallbackJob;
struct mvCallbackRegistry;
struct mvIO;
struct mvInput;
struct mvContext;

//-----------------------------------------------------------------------------
// forward declarations - item configs
//-----------------------------------------------------------------------------
struct mvSimplePlotConfig;
struct mvButtonConfig;
struct mvCheckboxConfig;
struct mvComboConfig;
struct mvDragFloatConfig;
struct mvDragIntConfig;
struct mvDragDoubleConfig;
struct mvDragFloatMultiConfig;
struct mvDragDoubleMultiConfig;
struct mvDragIntMultiConfig;
struct mvSliderIntConfig;
struct mvSliderFloatConfig;
struct mvSliderDoubleConfig;
struct mvSliderDoubleMultiConfig;
struct mvSliderFloatMultiConfig;
struct mvSliderIntMultiConfig;
struct mvListboxConfig;
struct mvRadioButtonConfig;
struct mvInputTextConfig;
struct mvInputIntConfig;
struct mvInputFloatConfig;
struct mvInputDoubleConfig;
struct mvInputFloatMultiConfig;
struct mvInputDoubleMultiConfig;
struct mvInputIntMultiConfig;
struct mvTextConfig;
struct mvSelectableConfig;
struct mvTabButtonConfig;
struct mvMenuItemConfig;
struct mvProgressBarConfig;
struct mvColorButtonConfig;
struct mvImageConfig;
struct mvImageButtonConfig;
struct mvColorEditConfig;
struct mvColorPickerConfig;
struct mvFilterSetConfig;
struct mvTooltipConfig;
struct mvColorMapConfig;
struct mvColorMapButtonConfig;
struct mvColorMapScaleConfig;
struct mvColorMapSliderConfig;

// plots
struct mvPlotConfig;
struct mvPlotAxisConfig;
struct mvAnnotationConfig;
struct mvSubPlotsConfig;
struct mvPlotLegendConfig;
struct mvDragLineConfig;
struct mvDragPointConfig;
struct mvBasicSeriesConfig;
struct mvBarSeriesConfig;
struct mv2dHistogramSeriesConfig;
struct mvHistogramSeriesConfig;
struct mvErrorSeriesConfig;
struct mvHeatSeriesConfig;
struct mvPieSeriesConfig;
struct mvLabelSeriesConfig;
struct mvImageSeriesConfig;
struct mvAreaSeriesConfig;
struct mvCandleSeriesConfig;
struct mvCustomSeriesConfig;