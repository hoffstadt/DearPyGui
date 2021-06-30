#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvRawTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvRawTexture : public mvAppItem
	{

		enum class ComponentType {
			MV_FLOAT_COMPONENT,
			MV_INT_COMPONENT,
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRawTexture, add_raw_texture)

			MV_CREATE_CONSTANT(mvFormat_Float_rgba, 0);
			MV_CREATE_CONSTANT(mvFormat_Float_rgb, 1);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvFormat_Float_rgba),
			MV_ADD_CONSTANT(mvFormat_Float_rgb),
		MV_END_CONSTANTS

	public:

		mvRawTexture(mvUUID uuid);
		~mvRawTexture();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void setWidth(int width) override {}
		void setHeight(int height) override {}


		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;

		void* getRawTexture() { return m_texture; }

	private:

		PyObject*     m_buffer = nullptr;
		void*         m_value = nullptr;
		void*         m_texture = nullptr;
		bool          m_dirty = true;
		ComponentType m_componentType = ComponentType::MV_FLOAT_COMPONENT;
		int           m_components = 4;

	};

}
