#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvButton : public mvAppItem
	{

	public:

		mvButton(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::Button; }

		virtual void draw() override;

	};

}