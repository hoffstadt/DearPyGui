#pragma once

#include "Core/mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvMenuBar
	//-----------------------------------------------------------------------------
	class mvMenuBar : public mvAppItem
	{

	public:

		mvMenuBar(const std::string& name)
			: mvAppItem("", name)
		{
			m_value = true;
		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::MenuBar; }

		virtual void draw() override;

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTabBar
	//-----------------------------------------------------------------------------
	class mvEndMenuBar : public mvAppItem
	{

	public:

		mvEndMenuBar(const std::string& parent)
			: mvAppItem(parent, "temporary")
		{
			static int i = 0;
			i++;

			m_name = "endMenuBar" + std::to_string(i);

		}

		virtual PyObject* getPyValue() override { return nullptr; }
		virtual mvAppItemType getType() const override { return mvAppItemType::EndMenuBar; }

		virtual void draw() override;


	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvMenu : public mvAppItem
	{

	public:

		mvMenu(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::Menu; }

		virtual void draw() override;

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvEndTab
	//-----------------------------------------------------------------------------
	class mvEndMenu : public mvAppItem
	{

	public:

		mvEndMenu(const std::string& parent)
			: mvAppItem(parent, "temporary"), m_value(false)
		{
			static int i = 0;
			i++;

			m_name = "endTab" + std::to_string(i);

		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::EndMenu; }

		virtual void draw() override;

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvMenuItem : public mvAppItem
	{

	public:

		mvMenuItem(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name), m_value(false)
		{
		}

		virtual PyObject* getPyValue() override;
		virtual mvAppItemType getType() const override { return mvAppItemType::MenuItem; }

		virtual void draw() override;

		inline void setValue(bool value) { m_value = value; }
		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}