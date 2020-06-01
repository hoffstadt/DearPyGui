#pragma once

#include <string>
#include <vector>

namespace Marvel {

	enum class mvAppItemType
	{
		None = 0, SampleWidget,
		InputText, InputFloat, InputFloat3, InputInt, Button, Checkbox, RadioButtons, Combo,
		Listbox, CollapsingHeader, TabBar, TabItem, EndTabItem, EndTabBar, Group, EndGroup,
		MenuBar, Menu, EndMenu, MenuItem, EndMenuBar, Spacing, Separator, SameLine,
		Tooltip, EndTooltip, Text, LabelText, ProgressBar, Child, EndChild, Custom, Image,
		DragInt, ColorEdit4, SliderInt, FileOpen, FileSave
	};

	struct mvAppItemProps
	{
		std::string name;
		std::string label;
		std::string tip;
		bool show;
	};

	class mvAppItem
	{

	public:

		mvAppItem(const mvAppItemProps& props)
			: m_props(props)
		{

		}

		~mvAppItem() = default;

		mvAppItem(const mvAppItem& other) = delete;
		mvAppItem(mvAppItem&& other) = delete;
		mvAppItem operator=(const mvAppItem& other) = delete;
		mvAppItem operator=(mvAppItem&& other) = delete;

		virtual mvAppItemType getType() const = 0;

		virtual void draw() = 0;

	protected:

		mvAppItemProps          m_props;
		mvAppItem*              m_parent = nullptr;
		std::vector<mvAppItem*> m_children;

	};

}