#include "mvFilterSet.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	mvFilterSet::mvFilterSet(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvFilterSet::getPyValue()
	{
		return ToPyString(std::string(_imguiFilter.InputBuf));
	}

	void mvFilterSet::setPyValue(PyObject* value)
	{
		auto str_value = ToString(value);

		int i = 0;
		for (auto& character : str_value)
		{
			if (i > 254)
				break;

			_imguiFilter.InputBuf[i] = character;

			++i;
		}
		_imguiFilter.InputBuf[i] = 0;
		_imguiFilter.Build();
	}

	void mvFilterSet::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_width != 0)
			ImGui::PushItemWidth((float)_width);

		if (_imguiFilter.IsActive())
		{
			for (auto& childset : _children)
			{
				for (auto& child : childset)
				{
					if (!_imguiFilter.PassFilter(child->_filter.c_str()))
						continue;

					child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
				}
			}

		}
		else
		{

			for (auto& childset : _children)
			{
				for (auto& child : childset)
					child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			}
		}

		if (_width != 0)
			ImGui::PopItemWidth();
	}

}