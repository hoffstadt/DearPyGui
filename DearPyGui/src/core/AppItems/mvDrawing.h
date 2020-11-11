#pragma once

#include <utility>

#include "core/AppItems/mvTypeBases.h"
#include "core/mvTextureStorage.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDrawing
//
//-----------------------------------------------------------------------------

#define MV_DRAWCOMMAND_TYPE(x) virtual mvDrawingCommandType getType() const override { return x; }\
virtual void draw(mvDrawing* draw, ImDrawList* draw_list) override;

namespace Marvel {

	class mvDrawing;

	//-----------------------------------------------------------------------------
	// Enums
	//-----------------------------------------------------------------------------
	enum class mvDrawingCommandType
	{
		DrawLine, DrawTriangle, DrawCircle, DrawText, DrawRect, DrawQuad,
		DrawPolyline, DrawBezierCurve, DrawPolygon, DrawImage, DrawArrow
	};

	//-----------------------------------------------------------------------------
	// mvDrawingCommand
	//-----------------------------------------------------------------------------
	class mvDrawingCommand
	{

	public:

		virtual ~mvDrawingCommand() = default;
		virtual void draw(mvDrawing* drawing, ImDrawList* draw_list) = 0;
		[[nodiscard]] virtual mvDrawingCommandType getType() const = 0;

		std::string tag;

	};

	//-----------------------------------------------------------------------------
	// mvDrawImageCommand
	//-----------------------------------------------------------------------------
	class mvDrawImageCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawImage)

		mvDrawImageCommand(std::string file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min = { 0, 0 },
				const mvVec2& uv_max = { 1, 1 }, const mvColor& color = { 255, 255, 255, 255 })
			: mvDrawingCommand(), m_file(std::move(file)), m_pmax(pmax), m_pmin(pmin), m_pmaxo(pmax), m_pmino(pmin), m_uv_min(uv_min), m_uv_max(uv_max),
			m_color(color)
		{
			if (m_pmax.x == m_pmin.x || m_pmax.y == m_pmin.y)
				m_autosize = true;
		}

		~mvDrawImageCommand() override;

	public:

		std::string m_file;
		mvVec2		m_pmax;
		mvVec2		m_pmin;
		mvVec2		m_pmaxo; // original, no scaling
		mvVec2		m_pmino; // original, no scaling
		mvVec2		m_uv_min;
		mvVec2		m_uv_max;
		mvColor		m_color;
		void*       m_texture = nullptr;
		int         m_width = 0;
		int         m_height = 0;
		bool        m_autosize = false;

	};

	//-----------------------------------------------------------------------------
	// mvDrawLineCommand
	//-----------------------------------------------------------------------------
	class mvDrawLineCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawLine)

		mvDrawLineCommand(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness)
			: mvDrawingCommand(), m_p1(p1), m_p2(p2), m_p1o(p1), m_p2o(p2), m_color(color), m_thickness(thickness)
		{
		}

	public:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p1o; // original, no scaling
		mvVec2  m_p2o; // original, no scaling
		mvColor m_color;
		float   m_thickness;

	};

	//-----------------------------------------------------------------------------
	// mvDrawArrowCommand
	//-----------------------------------------------------------------------------
	class mvDrawArrowCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawArrow)

		mvDrawArrowCommand(const mvVec2& p1, const mvVec2& p2, const std::vector<mvVec2>& points, const mvColor& color, float thickness, float size)
			: mvDrawingCommand(), m_p1(p1), m_p2(p2), m_p1o(p1), m_p2o(p2), m_points(points), m_pointso(points), m_color(color), m_thickness(thickness),
			m_size(size)
		{
		}

	public:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p1o; // original, no scaling
		mvVec2  m_p2o; // original, no scaling
		std::vector<mvVec2> m_points;
		std::vector<mvVec2> m_pointso; // original, no scaling
		mvColor m_color;
		float   m_thickness;
		float   m_size;

	};

	//-----------------------------------------------------------------------------
	// mvDrawTriangleCommand
	//-----------------------------------------------------------------------------
	class mvDrawTriangleCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawTriangle)

		mvDrawTriangleCommand(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvColor& color, float thickness, const mvColor& fill)
			: mvDrawingCommand(), m_p1(p1), m_p2(p2), m_p3(p3), m_p1o(p1), m_p2o(p2), m_p3o(p3), m_color(color), m_fill(fill), m_thickness(thickness)
		{
		}

	public:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p3;
		mvVec2  m_p1o; // original, no scaling
		mvVec2  m_p2o; // original, no scaling
		mvVec2  m_p3o; // original, no scaling
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

	//-----------------------------------------------------------------------------
	// mvDrawCircleCommand
	//-----------------------------------------------------------------------------
	class mvDrawCircleCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawCircle)

		mvDrawCircleCommand(const mvVec2& center, float radius, const mvColor& color, int segments, float thickness, const mvColor& fill)
			: mvDrawingCommand(), m_center(center), m_radius(radius), m_centero(center), m_radiuso(radius), m_segments(segments), m_color(color), m_fill(fill), m_thickness(thickness)
		{
		}

	public:

		mvVec2  m_center;
		float   m_radius;
		mvVec2  m_centero; // original, no scaling
		float   m_radiuso; // original, no scaling
		int     m_segments;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;

	};

	//-----------------------------------------------------------------------------
	// mvDrawTextCommand
	//-----------------------------------------------------------------------------
	class mvDrawTextCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawText)

		mvDrawTextCommand(const mvVec2& pos, std::string  text, const mvColor& color, int size)
			: mvDrawingCommand(), m_pos(pos), m_poso(pos), m_text(std::move(text)), m_color(color), m_size(size)
		{
		}

	public:

		mvVec2      m_pos;
		mvVec2      m_poso; // original, no scaling
		std::string m_text;
		mvColor     m_color;
		int         m_size;


	};

	//-----------------------------------------------------------------------------
	// mvDrawRectCommand
	//-----------------------------------------------------------------------------
	class mvDrawRectCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawRect)

		mvDrawRectCommand(const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, const mvColor& fill, float rounding, float thickness)
			: mvDrawingCommand(), m_pmin(pmin), m_pmax(pmax), m_pmino(pmin), m_pmaxo(pmax), m_rounding(rounding), m_color(color),m_fill(fill), m_thickness(thickness)
		{
		}

	public:

		mvVec2  m_pmin;
		mvVec2  m_pmax;
		mvVec2  m_pmino; // original, no scaling
		mvVec2  m_pmaxo; // original, no scaling
		float   m_rounding;
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;


	};

	//-----------------------------------------------------------------------------
	// mvDrawQuadCommand
	//-----------------------------------------------------------------------------
	class mvDrawQuadCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawQuad)

		mvDrawQuadCommand(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4,const mvColor& color, const mvColor& fill, float thickness)
			: mvDrawingCommand(), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p1o(p1), m_p2o(p2), m_p3o(p3), m_p4o(p4), m_color(color),m_fill(fill), m_thickness(thickness)
		{
		}

	public:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p3;
		mvVec2  m_p4;
		mvVec2  m_p1o; // original, no scaling
		mvVec2  m_p2o; // original, no scaling
		mvVec2  m_p3o; // original, no scaling
		mvVec2  m_p4o; // original, no scaling
		mvColor m_color;
		mvColor m_fill;
		float   m_thickness;


	};

	//-----------------------------------------------------------------------------
	// mvDrawPolylineCommand
	//-----------------------------------------------------------------------------
	class mvDrawPolylineCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawPolyline)

		mvDrawPolylineCommand(const std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness)
			: mvDrawingCommand(), m_points(points), m_pointso(points), m_color(color), m_closed(closed),m_thickness(thickness)
		{
		}

	public:

		std::vector<mvVec2> m_points;
		std::vector<mvVec2> m_pointso; // original, no scaling
		mvColor             m_color;
		bool                m_closed;
		float               m_thickness;


	};

	//-----------------------------------------------------------------------------
	// mvDrawPolygonCommand
	//-----------------------------------------------------------------------------
	class mvDrawPolygonCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawPolygon)

		mvDrawPolygonCommand(const std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness)
			: mvDrawingCommand(), m_points(points), m_pointso(points), m_color(color), m_fill(fill), m_thickness(thickness)
		{
		}

	public:

		std::vector<mvVec2> m_points;
		std::vector<mvVec2> m_pointso; // original, no scaling
		mvColor             m_color;
		mvColor             m_fill;
		float               m_thickness;


	};

	//-----------------------------------------------------------------------------
	// mvDrawBezierCurveCommand
	//-----------------------------------------------------------------------------
	class mvDrawBezierCurveCommand : public mvDrawingCommand
	{

	public:

		MV_DRAWCOMMAND_TYPE(mvDrawingCommandType::DrawBezierCurve)

		mvDrawBezierCurveCommand(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4,const mvColor& color, float thickness, int segments)
			: mvDrawingCommand(), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p1o(p1), m_p2o(p2), m_p3o(p3), m_p4o(p4), m_color(color),m_thickness(thickness), m_segments(segments)
		{
		}

	public:

		mvVec2  m_p1;
		mvVec2  m_p2;
		mvVec2  m_p3;
		mvVec2  m_p4;
		mvVec2  m_p1o; // original, no scaling
		mvVec2  m_p2o; // original, no scaling
		mvVec2  m_p3o; // original, no scaling
		mvVec2  m_p4o; // original, no scaling
		mvColor m_color;
		float   m_thickness;
		int     m_segments;
	};

	//-----------------------------------------------------------------------------
	// mvDrawing
	//-----------------------------------------------------------------------------
	class mvDrawing : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Drawing, "add_drawing")

		mvDrawing(const std::string& name)
			: mvAppItem(name)
		{
		}

		~mvDrawing()  override
		{
			clear();
		}

		void draw() override;
		void clear();

		[[nodiscard]] float  getStartX() const { return m_startx; }
		[[nodiscard]] float  getStartY() const { return m_starty; }
		[[nodiscard]] mvVec2 getStart () const { return { m_startx, m_starty }; }

		void drawLine       (const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness, const std::string& tag = "");
		void drawArrow      (const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness, float size, const std::string& tag = "");
		void drawTriangle   (const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvColor& color, const mvColor& fill, float thickness, const std::string& tag = "");
		void drawRectangle  (const mvVec2& pmin, const mvVec2& pmax, const mvColor& color,const mvColor& fill, float rounding, float thickness, const std::string& tag = "");
		void drawQuad       (const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4,const mvColor& color, const mvColor& fill, float thickness, const std::string& tag = "");
		void drawText       (const mvVec2& pos, const std::string& text, const mvColor& color, int size, const std::string& tag = "");
		void drawCircle     (const mvVec2& center, float radius, const mvColor& color,int segments, float thickness, const mvColor& fill, const std::string& tag = "");
		void drawPolyline   (std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness, const std::string& tag = "");
		void drawPolygon    (std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness, const std::string& tag = "");
		void drawBezierCurve(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, float thickness, int segments, const std::string& tag = "");
		void drawImage      (const std::string& file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min, const mvVec2& uv_max, const mvColor& color, const std::string& tag = "");

		void setScale(float xscale, float yscale);
		void setOrigin(float x, float y);

		[[nodiscard]] ImVec2 getScale() const { return { m_scalex, m_scaley }; }
		[[nodiscard]] ImVec2 getOrigin() const { return { m_originx, m_originy }; }

		void deleteCommand(const std::string& tag);

		mvVec2 convertToModelSpace(const mvVec2& point);
		void convertToModelSpace(std::vector<mvVec2>& points, const std::vector<mvVec2>& pointso);

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "originx")) { m_originx = ToFloat(item); m_dirty = true; }
			if (PyObject* item = PyDict_GetItemString(dict, "originy")) { m_originy = ToFloat(item); m_dirty = true; }
			if (PyObject* item = PyDict_GetItemString(dict, "scalex")) { m_scalex = ToFloat(item); m_dirty = true; }
			if (PyObject* item = PyDict_GetItemString(dict, "scaley")) { m_scaley = ToFloat(item); m_dirty = true; }
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "originx", ToPyFloat(m_originx));
			PyDict_SetItemString(dict, "originy", ToPyFloat(m_originy));
			PyDict_SetItemString(dict, "scalex", ToPyFloat(m_scalex));
			PyDict_SetItemString(dict, "scaley", ToPyFloat(m_scaley));
		}

	private:

		void updateCommands();

	private:

		std::vector<mvDrawingCommand*> m_commands;
		ImDrawList* draw_list = nullptr;
		float       m_startx = 0.0f;
		float       m_starty = 0.0f;
		float       m_originx = 0.0f;
		float       m_originy = 0.0f;
		float       m_scalex = 1.0f;
		float       m_scaley = 1.0f;
		bool        m_dirty = true;

	};
}