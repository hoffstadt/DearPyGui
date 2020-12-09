#pragma once

#include <utility>
#include "mvAppItem.h"

namespace Marvel {

	class mvImage : public mvAppItem, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt, "add_image")

		mvImage(const std::string& name, std::string default_value);

		bool onEvent(mvEvent& event) override;
		bool onTextureDeleted(mvEvent& event);

		~mvImage() override;

		void               draw              ()               override;
		void               setExtraConfigDict(PyObject* dict) override;
		void               getExtraConfigDict(PyObject* dict) override;
		void               setValue          (const std::string& value);
		const std::string& getValue          () const;


	private:

		std::string m_value;
		mvVec2	    m_uv_min = {0.0f, 0.0f};
		mvVec2	    m_uv_max = {1.0f, 1.0f};
		mvColor     m_tintColor = {255, 255, 255, 255, true};
		mvColor     m_borderColor = {0, 0, 0, 0, true};
		void*       m_texture = nullptr;
		bool        m_dirty = false;

	};

}