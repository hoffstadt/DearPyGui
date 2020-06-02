#pragma once

#include "mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTabBar
	//-----------------------------------------------------------------------------
	class mvTabBar : public mvAppItem
	{

	public:

		mvTabBar(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
			m_value = new std::string();
		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::TabBar; }

		virtual void draw() override;

		inline void setValue(const std::string& value) { *m_value = value; }
		inline const std::string& getValue() const { return *m_value; }

	private:

		std::string* m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTabBar
	//-----------------------------------------------------------------------------
	class mvEndTabBar : public mvAppItem
	{

	public:

		mvEndTabBar(const std::string& parent)
			: mvAppItem(parent, "temporary"), m_value(false)
		{
			static int i = 0;
			i++;

			m_name = "endTabBar" + std::to_string(i);

		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::EndTabBar; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvTab : public mvAppItem
	{

	public:

		mvTab(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::TabItem; }

		virtual void draw() override;

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTab
	//-----------------------------------------------------------------------------
	class mvEndTab : public mvAppItem
	{

	public:

		mvEndTab(const std::string& parent)
			: mvAppItem(parent, "temporary"), m_value(false)
		{
			static int i = 0;
			i++;

			m_name = "endTab" + std::to_string(i);

		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::EndTabItem; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}