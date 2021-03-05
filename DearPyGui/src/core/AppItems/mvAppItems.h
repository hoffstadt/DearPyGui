#pragma once

#include "basic/mvInputText.h"
#include "basic/mvRadioButton.h"
#include "basic/mvListbox.h"
#include "basic/mvCheckbox.h"
#include "basic/mvButton.h"
#include "basic/mvSelectable.h"
#include "basic/mvCombo.h"
#include "basic/mvInputItems.h"
#include "basic/mvSliderItems.h"
#include "basic/mvDragItems.h"
#include "basic/mvColorButton.h"
#include "basic/mvColorEdit.h"
#include "basic/mvColorPicker.h"
#include "basic/mvText.h"
#include "basic/mvTabButton.h"
#include "basic/mvMenuItem.h"
#include "basic/mvImage.h"
#include "basic/mvImageButton.h"

#include "misc/mvProgressBar.h"
#include "misc/mvDummy.h"
#include "misc/mvIndent.h"
#include "misc/mvUnindent.h"
#include "misc/mvSameLine.h"
#include "misc/mvSpacing.h"
#include "misc/mvSeparator.h"

#include "containers/mvTreeNode.h"
#include "containers/mvChild.h"
#include "containers/mvGroup.h"
#include "containers/mvCollapsingHeader.h"
#include "containers/mvTab.h"
#include "containers/mvTabBar.h"
#include "containers/mvMenu.h"
#include "containers/mvMenuBar.h"
#include "containers/mvTooltip.h"
#include "containers/mvColumns.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvPopup.h"

#include "custom/mvDrawing.h"
#include "custom/mvTimePicker.h"
#include "custom/mvDatePicker.h"

#include "composite/mvTable.h"
#include "composite/mvLogger.h"
#include "composite/mvAboutWindow.h"
#include "composite/mvDocWindow.h"
#include "composite/mvDebugWindow.h"
#include "composite/mvMetricsWindow.h"
#include "composite/mvStyleWindow.h"
#include "composite/mvFileDialog.h"

#include "plots/mvPlot.h"
#include "plots/mvSimplePlot.h"
#include "plots/mvLineSeries.h"
#include "plots/mvBarSeries.h"
#include "plots/mvScatterSeries.h"
#include "plots/mvAreaSeries.h"
#include "plots/mvStemSeries.h"
#include "plots/mvLabelSeries.h"
#include "plots/mvPieSeries.h"
#include "plots/mvShadeSeries.h"
#include "plots/mvErrorSeries.h"
#include "plots/mvHeatSeries.h"
#include "plots/mvImageSeries.h"
#include "plots/mvStairSeries.h"
#include "plots/mvCandleSeries.h"
#include "plots/mvInfiniteLineSeries.h"

#include "nodes/mvNodeEditor.h"
#include "nodes/mvNode.h"
#include "nodes/mvNodeAttribute.h"

namespace Marvel{

    template<> struct mvItemType<(int)mvAppItemType::Spacing>          { typedef mvSpacing type; };
    template<> struct mvItemType<(int)mvAppItemType::SameLine>         { typedef mvSameLine type; };
    template<> struct mvItemType<(int)mvAppItemType::InputText>        { typedef mvInputText type; };
    template<> struct mvItemType<(int)mvAppItemType::Button>           { typedef mvButton type; };
    template<> struct mvItemType<(int)mvAppItemType::RadioButtons>     { typedef mvRadioButton type; };
    template<> struct mvItemType<(int)mvAppItemType::TabBar>           { typedef mvTabBar type; };
    template<> struct mvItemType<(int)mvAppItemType::TabItem>          { typedef mvTab type; };
    template<> struct mvItemType<(int)mvAppItemType::Image>            { typedef mvImage type; };
    template<> struct mvItemType<(int)mvAppItemType::MenuBar>          { typedef mvMenuBar type; };
    template<> struct mvItemType<(int)mvAppItemType::Menu>             { typedef mvMenu type; };
    template<> struct mvItemType<(int)mvAppItemType::MenuItem>         { typedef mvMenuItem type; };
    template<> struct mvItemType<(int)mvAppItemType::Group>            { typedef mvGroup type; };
    template<> struct mvItemType<(int)mvAppItemType::Child>            { typedef mvChild type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderFloat>      { typedef mvSliderFloat type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderFloat2>     { typedef mvSliderFloat2 type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderFloat3>     { typedef mvSliderFloat3 type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderFloat4>     { typedef mvSliderFloat4 type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderInt>        { typedef mvSliderInt type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderInt2>       { typedef mvSliderInt2 type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderInt3>       { typedef mvSliderInt3 type; };
    template<> struct mvItemType<(int)mvAppItemType::SliderInt4>       { typedef mvSliderInt4 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragFloat>        { typedef mvDragFloat type; };
    template<> struct mvItemType<(int)mvAppItemType::DragFloat2>       { typedef mvDragFloat2 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragFloat3>       { typedef mvDragFloat3 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragFloat4>       { typedef mvDragFloat4 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragInt>          { typedef mvDragInt type; };
    template<> struct mvItemType<(int)mvAppItemType::DragInt2>         { typedef mvDragInt2 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragInt3>         { typedef mvDragInt3 type; };
    template<> struct mvItemType<(int)mvAppItemType::DragInt4>         { typedef mvDragInt4 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputFloat>       { typedef mvInputFloat type; };
    template<> struct mvItemType<(int)mvAppItemType::InputFloat2>      { typedef mvInputFloat2 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputFloat3>      { typedef mvInputFloat3 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputFloat4>      { typedef mvInputFloat4 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputInt>         { typedef mvInputInt type; };
    template<> struct mvItemType<(int)mvAppItemType::InputInt2>        { typedef mvInputInt2 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputInt3>        { typedef mvInputInt3 type; };
    template<> struct mvItemType<(int)mvAppItemType::InputInt4>        { typedef mvInputInt4 type; };
    template<> struct mvItemType<(int)mvAppItemType::ColorEdit3>       { typedef mvColorEdit3 type; };
    template<> struct mvItemType<(int)mvAppItemType::ColorEdit4>       { typedef mvColorEdit4 type; };
    template<> struct mvItemType<(int)mvAppItemType::ColorPicker3>     { typedef mvColorPicker3 type; };
    template<> struct mvItemType<(int)mvAppItemType::ColorPicker4>     { typedef mvColorPicker4 type; };
    template<> struct mvItemType<(int)mvAppItemType::Tooltip>          { typedef mvTooltip type; };
    template<> struct mvItemType<(int)mvAppItemType::CollapsingHeader> { typedef mvCollapsingHeader type; };
    template<> struct mvItemType<(int)mvAppItemType::Separator>        { typedef mvSeparator type; };
    template<> struct mvItemType<(int)mvAppItemType::Checkbox>         { typedef mvCheckbox type; };
    template<> struct mvItemType<(int)mvAppItemType::Listbox>          { typedef mvListbox type; };
    template<> struct mvItemType<(int)mvAppItemType::Text>             { typedef mvText type; };
    template<> struct mvItemType<(int)mvAppItemType::LabelText>        { typedef mvLabelText type; };
    template<> struct mvItemType<(int)mvAppItemType::Combo>            { typedef mvCombo type; };
    template<> struct mvItemType<(int)mvAppItemType::Plot>             { typedef mvPlot type; };
    template<> struct mvItemType<(int)mvAppItemType::SimplePlot>       { typedef mvSimplePlot type; };
    template<> struct mvItemType<(int)mvAppItemType::Indent>           { typedef mvIndent type; };
    template<> struct mvItemType<(int)mvAppItemType::Unindent>         { typedef mvUnindent type; };
    template<> struct mvItemType<(int)mvAppItemType::Drawing>          { typedef mvDrawing type; };
    template<> struct mvItemType<(int)mvAppItemType::Window>           { typedef mvWindowAppItem type; };
    template<> struct mvItemType<(int)mvAppItemType::Popup>            { typedef mvPopup type; };
    template<> struct mvItemType<(int)mvAppItemType::Selectable>       { typedef mvSelectable type; };
    template<> struct mvItemType<(int)mvAppItemType::TreeNode>         { typedef mvTreeNode type; };
    template<> struct mvItemType<(int)mvAppItemType::ProgressBar>      { typedef mvProgressBar type; };
    template<> struct mvItemType<(int)mvAppItemType::Table>            { typedef mvTable type; };
    template<> struct mvItemType<(int)mvAppItemType::Dummy>            { typedef mvDummy type; };
    template<> struct mvItemType<(int)mvAppItemType::ImageButton>      { typedef mvImageButton type; };
    template<> struct mvItemType<(int)mvAppItemType::TimePicker>       { typedef mvTimePicker type; };
    template<> struct mvItemType<(int)mvAppItemType::DatePicker>       { typedef mvDatePicker type; };
    template<> struct mvItemType<(int)mvAppItemType::ColorButton>      { typedef mvColorButton type; };
    template<> struct mvItemType<(int)mvAppItemType::ManagedColumns>   { typedef mvManagedColumns type; };
    template<> struct mvItemType<(int)mvAppItemType::ColumnSet>        { typedef mvColumn type; };
    template<> struct mvItemType<(int)mvAppItemType::NextColumn>       { typedef mvNextColumn type; };
    template<> struct mvItemType<(int)mvAppItemType::Logger>           { typedef mvLoggerItem type; };
    template<> struct mvItemType<(int)mvAppItemType::AboutWindow>      { typedef mvAboutWindow type; };
    template<> struct mvItemType<(int)mvAppItemType::DocWindow>        { typedef mvDocWindow type; };
    template<> struct mvItemType<(int)mvAppItemType::DebugWindow>      { typedef mvDebugWindow type; };
    template<> struct mvItemType<(int)mvAppItemType::MetricsWindow>    { typedef mvMetricsWindow type; };
    template<> struct mvItemType<(int)mvAppItemType::StyleWindow>      { typedef mvStyleWindow type; };
    template<> struct mvItemType<(int)mvAppItemType::FileDialog>       { typedef mvFileDialog type; };
    template<> struct mvItemType<(int)mvAppItemType::TabButton>        { typedef mvTabButton type; };
    template<> struct mvItemType<(int)mvAppItemType::NodeEditor>       { typedef mvNodeEditor type; };
    template<> struct mvItemType<(int)mvAppItemType::Node>             { typedef mvNode type; };
    template<> struct mvItemType<(int)mvAppItemType::NodeAttribute>    { typedef mvNodeAttribute type; };
 }