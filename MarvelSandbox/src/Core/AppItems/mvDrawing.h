#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvDrawing;

	//-----------------------------------------------------------------------------
	// Enums
	//-----------------------------------------------------------------------------
	enum class mvDrawingCommandType
	{
		DrawLine
	};

	//-----------------------------------------------------------------------------
	// mvDrawingCommand
	//-----------------------------------------------------------------------------
	class mvDrawingCommand
	{

	public:

		virtual void draw(mvDrawing* drawing, ImDrawList* draw_list) = 0;
		virtual mvDrawingCommandType getType() const = 0;

	};

	//-----------------------------------------------------------------------------
	// mvDrawLineCommand
	//-----------------------------------------------------------------------------
	class mvDrawLineCommand : public mvDrawingCommand
	{

	public:

		mvDrawLineCommand(int x1, int y1, int x2, int y2, const mvColor& color, float thickness = 1.0f)
			: mvDrawingCommand(), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_color(color), m_thickness(thickness)
		{
		}

		virtual mvDrawingCommandType getType() const override { return mvDrawingCommandType::DrawLine; }

		virtual void draw(mvDrawing* draw, ImDrawList* draw_list) override;

	private:

		int m_x1;
		int m_y1;
		int m_y2;
		int m_x2;
		mvColor m_color;
		float m_thickness;

	};

	//-----------------------------------------------------------------------------
	// mvDrawing
	//-----------------------------------------------------------------------------
	class mvDrawing : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Drawing)

			mvDrawing(const std::string& parent, const std::string& name, int width, int height)
			: mvNoneItemBase(parent, name)
		{
			m_width = width;
			m_height = height;
		}

		virtual void draw() override;

		inline float getStartX() const { return m_startx; }
		inline float getStartY() const { return m_starty; }

		void drawLine(int x1, int y1, int x2, int y2, const mvColor& color, float thickness = 1);

	private:

		std::vector<mvDrawingCommand*> m_commands;
		ImDrawList* draw_list = nullptr;
		float       m_height;
		float       m_startx;
		float       m_starty;

	};
}