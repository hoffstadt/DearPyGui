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
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvFileDialog)
		MV_END_PARENTS

	public:

		mvFileExtension(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

		const std::string& getFilter() const { return _extension; }

	private:

		std::string _extension;
		std::string _customText;
		mvColor     _color = {-1.0f, 0.0f, 0.0f, 1.0f};
	};

}