#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvGroup : public mvAppItem
	{

	public:

		mvGroup(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::Group; }

		virtual void draw() override;

	};

	class mvEndGroup : public mvAppItem
	{

	public:

		mvEndGroup(const std::string& parent)
			: mvAppItem(parent, "Endgroup")
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::EndGroup; }

		virtual void draw() override;

	};

}