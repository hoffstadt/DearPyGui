#include "mvTextureItems.h"
#include "mvPyUtils.h"
#include "mvUtilities.h"

mvTextureRegistry::mvTextureRegistry(mvUUID uuid)
	:
	mvAppItem(uuid)
{
	config.show = false;
}

void mvTextureRegistry::draw(ImDrawList* drawlist, float x, float y)
{

	for (auto& item : childslots[1])
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

	if (config.show)
		show_debugger();

}

void mvTextureRegistry::show_debugger()
{
	ImGui::PushID(this);

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(info.internalLabel.c_str(), &config.show))
	{

		ImGui::Text("Textures");

		ImGui::BeginChild("##TextureStorageChild", ImVec2(400, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		int index = 0;
		for (auto& texture : childslots[1])
		{
			bool status = false;
			void* textureRaw = nullptr;
			if (texture->type == mvAppItemType::mvStaticTexture)
				textureRaw = static_cast<mvStaticTexture*>(texture.get())->_texture;
			else
				textureRaw = static_cast<mvDynamicTexture*>(texture.get())->_texture;

			ImGui::Image(textureRaw, ImVec2(25, 25));
			ImGui::SameLine();
			if (ImGui::Selectable(texture->info.internalLabel.c_str(), &status))
				_selection = index;

			++index;
		}
		ImGui::EndChild();

		if (_selection != -1)
		{

			if (_selection < childslots[1].size())
			{

				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::BeginGroup();
				ImGui::Text("Width: %d", childslots[1][_selection]->config.width);
				ImGui::Text("Height: %d", childslots[1][_selection]->config.height);
				ImGui::Text("Type: %s", childslots[1][_selection]->type == mvAppItemType::mvStaticTexture ? "static" : "dynamic");
				ImGui::EndGroup();

				ImGui::SameLine();

				void* textureRaw = nullptr;
				if (childslots[1][_selection]->type == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(childslots[1][_selection].get())->_texture;
				else
					textureRaw = static_cast<mvDynamicTexture*>(childslots[1][_selection].get())->_texture;

				ImGui::Image(textureRaw, ImVec2((float)childslots[1][_selection]->config.width, (float)childslots[1][_selection]->config.height));

				ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				if (ImPlot::BeginPlot("##texture plot", ImVec2(-1, -1),
					ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_Equal))
				{
					ImPlot::PlotImage(childslots[1][_selection]->info.internalLabel.c_str(), textureRaw, ImPlotPoint(0.0, 0.0),
						ImPlotPoint(childslots[1][_selection]->config.width, childslots[1][_selection]->config.height));
					ImPlot::EndPlot();
				}
				ImPlot::PopStyleColor();


				ImGui::EndGroup();
			}
			else
				_selection = -1;
		}


	}

	ImGui::End();

	ImGui::PopID();
}

mvDynamicTexture::~mvDynamicTexture()
{
	FreeTexture(_texture);
}

PyObject* mvDynamicTexture::getPyValue()
{
	return ToPyList(*_value);
}

void mvDynamicTexture::setPyValue(PyObject* value)
{
	*_value = ToFloatVect(value);
}

void mvDynamicTexture::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
}

void mvDynamicTexture::draw(ImDrawList* drawlist, float x, float y)
{
	if (_dirty)
	{

		_texture = LoadTextureFromArrayDynamic(_permWidth, _permHeight, _value->data());

		if (_texture == nullptr)
			state.ok = false;

		_dirty = false;
		return;
	}

	UpdateTexture(_texture, _permWidth, _permHeight, *_value);

}

void mvDynamicTexture::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_permWidth = ToInt(PyTuple_GetItem(dict, 0));
	config.width = _permWidth;
	_permHeight = ToInt(PyTuple_GetItem(dict, 1));
	config.height = _permHeight;
	*_value = ToFloatVect(PyTuple_GetItem(dict, 2));
}

void mvDynamicTexture::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

}

void mvDynamicTexture::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

PyObject* mvRawTexture::getPyValue()
{
	return GetPyNone();
}

void mvRawTexture::setPyValue(PyObject* value)
{

	if (value == nullptr)
		return;

	if (PyObject_CheckBuffer(value))
	{
		Py_buffer buffer_info;

		if (!PyObject_GetBuffer(value, &buffer_info,
			PyBUF_CONTIG_RO | PyBUF_FORMAT))
		{
			_value = buffer_info.buf;
			if (_value == nullptr)
			{
				mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture data not valid", this);
			}
		}
		PyBuffer_Release(&buffer_info);
		if (_buffer)
			Py_XDECREF(_buffer);
		Py_XINCREF(value);
		_buffer = value;
	}
}

mvRawTexture::~mvRawTexture()
{
	FreeTexture(_texture);

	mvGlobalIntepreterLock gil;
	Py_XDECREF(_buffer);
}

void mvRawTexture::draw(ImDrawList* drawlist, float x, float y)
{
	if (_dirty)
	{

		if (_value == nullptr)
			return;

		if (_componentType == ComponentType::MV_FLOAT_COMPONENT)
			_texture = LoadTextureFromArrayRaw(_permWidth, _permHeight, (float*)_value, _components);

		if (_texture == nullptr)
			state.ok = false;

		_dirty = false;
		return;
	}

	if (_componentType == ComponentType::MV_FLOAT_COMPONENT)
		UpdateRawTexture(_texture, _permWidth, _permHeight, (float*)_value, _components);

}

void mvRawTexture::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
	{
		mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture data not valid", this);
		return;
	}

	_permWidth = ToInt(PyTuple_GetItem(dict, 0));
	config.width = _permWidth;
	_permHeight = ToInt(PyTuple_GetItem(dict, 1));
	config.height = _permHeight;
	setPyValue(PyTuple_GetItem(dict, 2));
}

void mvRawTexture::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "format"))
	{
		int format = ToInt(item);
		if (format == 0)
		{
			_components = 4;
			_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
		}
		else if (format == 1)
		{
			_components = 3;
			_componentType = mvRawTexture::ComponentType::MV_FLOAT_COMPONENT;
		}
	}
}

void mvRawTexture::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

mvStaticTexture::~mvStaticTexture()
{
	if (uuid == MV_ATLAS_UUID)
		return;
	//UnloadTexture(_name);
	FreeTexture(_texture);
}

void mvStaticTexture::draw(ImDrawList* drawlist, float x, float y)
{
	if (!_dirty)
		return;

	if (!state.ok)
		return;

	if (uuid == MV_ATLAS_UUID)
	{
		_texture = ImGui::GetIO().Fonts->TexID;
		config.width = ImGui::GetIO().Fonts->TexWidth;
		config.height = ImGui::GetIO().Fonts->TexHeight;
	}
	else
		_texture = LoadTextureFromArray(_permWidth, _permHeight, _value->data());

	if (_texture == nullptr)
	{
		state.ok = false;
		mvThrowPythonError(mvErrorCode::mvItemNotFound, "add_static_texture",
			"Texture data can not be found.", this);
	}


	_dirty = false;
}

void mvStaticTexture::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_permWidth = ToInt(PyTuple_GetItem(dict, 0));
	config.width = _permWidth;
	_permHeight = ToInt(PyTuple_GetItem(dict, 1));
	config.height = _permHeight;
	*_value = ToFloatVect(PyTuple_GetItem(dict, 2));
}

PyObject* mvStaticTexture::getPyValue()
{
	return ToPyList(*_value);
}

void mvStaticTexture::setPyValue(PyObject* value)
{
	*_value = ToFloatVect(value);
}

void mvStaticTexture::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
}