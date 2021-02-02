#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvButtonConfig : public mvAppItemConfig
	{
		ImGuiDir direction = ImGuiDir_Up;
		bool small_button = false;
		bool arrow = false;

	};

	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs);
	void mv_add_button(const char* name, const mvButtonConfig& config = {});

	class mvButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button, "add_button")

		mvButton(const std::string& name);
		mvButton(const std::string& name, const mvButtonConfig& config);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		mvButtonConfig m_config;

	};

}
