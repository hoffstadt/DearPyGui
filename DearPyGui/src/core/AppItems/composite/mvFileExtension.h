#pragma once

#include "mvAppItem.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvFileExtension, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvFileExtension : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileExtension, add_file_extension)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvFileExtension(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		const std::string& getFilter() const { return m_extension; }

	private:

		std::string m_extension;
		std::string m_customText;
		mvColor     m_color = {-1.0f, 0.0f, 0.0f, 1.0f};
	};

}