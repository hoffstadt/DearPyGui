#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvTooltip : public mvAppItem
	{

	public:

		mvTooltip(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
			// has to be showed that way it can check for hovering
			// otherwise it will never show
			m_show = true;
		}

		virtual PyObject* getPyValue() override;

		virtual mvAppItemType getType() const override { return mvAppItemType::Tooltip; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	class mvEndTooltip : public mvAppItem
	{

	public:

		mvEndTooltip(const std::string& parent)
			: mvAppItem(parent, "EndTooltip")
		{
		}

		virtual PyObject* getPyValue() override { return nullptr; }

		virtual mvAppItemType getType() const override { return mvAppItemType::EndTooltip; }

		virtual void draw() override;

	};

}