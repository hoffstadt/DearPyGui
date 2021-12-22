#include "mvNodeAttribute.h"
#include <imnodes.h>
#include "mvContext.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvNodeEditor.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"

namespace Marvel {

	mvNodeAttribute::mvNodeAttribute(mvUUID uuid)
		: mvAppItem(uuid)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		_id = (int)reduced_address;
	}

	void mvNodeAttribute::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvNodeAttribute*>(item);
		_attrType = titem->_attrType;
		_shape = titem->_shape;
		_category = titem->_category;
	}

	void mvNodeAttribute::draw(ImDrawList* drawlist, float x, float y)
	{
		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!config.show)
			return;

		// set item width
		if (config.width != 0)
			ImGui::SetNextItemWidth((float)config.width);

		// indent (for children
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

			if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
				imnodes::BeginStaticAttribute((int)_id);
			else if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
				imnodes::BeginOutputAttribute((int)_id, _shape);
			else
				imnodes::BeginInputAttribute((int)_id, _shape);

			for (auto& item : childslots[1])
			{
				// skip item if it's not shown
				if (!item->config.show)
					continue;

				// set item width
				if (item->config.width != 0)
					ImGui::SetNextItemWidth((float)item->config.width);

				if (item->info.focusNextFrame)
				{
					ImGui::SetKeyboardFocusHere();
					item->info.focusNextFrame = false;
				}

				auto oldCursorPos = ImGui::GetCursorPos();
				if (item->info.dirtyPos)
					ImGui::SetCursorPos(item->state.pos);

				item->state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

				item->draw(drawlist, x, y);

				if (item->info.dirtyPos)
					ImGui::SetCursorPos(oldCursorPos);

				UpdateAppItemState(item->state);
			}

			if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Static)
				imnodes::EndStaticAttribute();
			else if (_attrType == mvNodeAttribute::AttributeType::mvAttr_Output)
				imnodes::EndOutputAttribute();
			else
				imnodes::EndInputAttribute();

		}

		// undo indents
		if (config.indent > 0.0f)
			ImGui::Unindent(config.indent);

		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);
	}

	void mvNodeAttribute::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "category")) _category = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "attribute_type")) _attrType = (mvNodeAttribute::AttributeType)ToUUID(item);
		if (PyObject* item = PyDict_GetItemString(dict, "shape"))
		{
			_shape = (imnodes::PinShape)ToInt(item);
		}
	}

	void mvNodeAttribute::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "attribute_type", mvPyObject(ToPyLong((long)_attrType)));
		PyDict_SetItemString(dict, "shape", mvPyObject(ToPyInt((int)_shape)));
		PyDict_SetItemString(dict, "category", mvPyObject(ToPyString(_category)));
	}

}