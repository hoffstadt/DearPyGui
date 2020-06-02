#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvChild : public mvAppItem
	{

	public:

		mvChild(const std::string& parent, const std::string& name, int width, int height)
			: mvAppItem(parent, name), m_value(false)
		{
			m_width = width;
			m_height = height;
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::Child; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;
		int m_height;

	};

	class mvEndChild : public mvAppItem
	{

	public:

		mvEndChild(const std::string& parent)
			: mvAppItem(parent, "Endgroup")
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::EndChild; }

		virtual void draw() override;

	};

}