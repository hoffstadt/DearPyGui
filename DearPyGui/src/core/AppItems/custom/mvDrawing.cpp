#include "mvDrawing.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvDrawList.h"
#include "mvDrawCmd.h"
#include "mvWindow.h"
#include "mvAppItems.h"

namespace Marvel {

	void mvDrawing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Bool, "show","Attempt to render", "True"},
		}, "Adds a drawing widget.", "None", "Drawing") });

		parsers->insert({ "modify_draw_command", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
			{mvPythonDataType::Kwargs, "**Kwargs"},
		}, "Configures an drawing command.", "None", "Drawing") });

		parsers->insert({ "bring_draw_command_forward", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Brings draw command forward.", "None", "Drawing") });

		parsers->insert({ "bring_draw_command_to_front", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Brings draw command to front.", "None", "Drawing") });

		parsers->insert({ "send_draw_command_back", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Sends draw command back.", "None", "Drawing") });

		parsers->insert({ "send_draw_command_to_back", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Sends draw command to the back.", "None", "Drawing") });

		parsers->insert({ "get_draw_command", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Returns an draw commands information", "dict", "Drawing") });

		parsers->insert({ "delete_draw_command", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "tag"},
		}, "Deletes a drawing item.", "None", "Drawing") });

		parsers->insert({ "draw_image", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::String, "file"},
			{mvPythonDataType::FloatList, "pmin", "top left coordinate"},
			{mvPythonDataType::FloatList, "pmax", "bottom right coordinate"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "uv_min", "normalized texture coordinates", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "uv_max", "normalized texture coordinates", "(1.0, 1.0)"},
			{mvPythonDataType::IntList, "color", "", "(255, 255, 255, 255)"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, ("Draws an image on a drawing. p_min (bottom-left) and p_max (upper-right) represent corners of the rectangle the image will be drawn to."
			"Setting the p_min equal to the p_max will sraw the image to with 1:1 scale."
			"uv_min and uv_max represent the normalized texture coordinates of the original image that will be shown. Using (0,0)->(1,1) texture"
			"coordinates will generally display the entire texture."), "None", "Drawing") });

		parsers->insert({ "draw_line", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a line on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_arrow", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws an arrow on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_triangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a triangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_rectangle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pmin", "top left coordinate"},
			{mvPythonDataType::FloatList, "pmax", "bottom right coordinate"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "rounding", "", "0.0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a rectangle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_quad", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::FloatList, "p4"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a quad on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_text", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "pos"},
			{mvPythonDataType::String, "text"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Integer, "size", "", "10"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws text on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_circle", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "center"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "segments", "", "0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a circle on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polyline", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::ListFloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closed", "", "False"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws lines on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_polygon", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::ListFloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a polygon on a drawing.", "None", "Drawing") });

		parsers->insert({ "draw_bezier_curve", mvPythonParser({
			{mvPythonDataType::String, "drawing"},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::FloatList, "p3"},
			{mvPythonDataType::FloatList, "p4"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::Integer, "segments", "", "0"},
			{mvPythonDataType::String, "tag", "", "''"},
		}, "Draws a bezier curve on a drawing.", "None", "Drawing") });

		parsers->insert({ "clear_drawing", mvPythonParser({
			{mvPythonDataType::String, "draw"}
		}, "Clears a drawing.", "None", "Drawing") });
	}

	mvDrawing::mvDrawing(const std::string& name)
		: mvAppItem(name)
	{
		m_drawList = CreateRef<mvDrawList>();
	}

	void mvDrawing::draw()
	{
		mvFontScope fscope(this);

		m_startx = (float)ImGui::GetCursorScreenPos().x;
		m_starty = (float)ImGui::GetCursorScreenPos().y;

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_core_config.width, m_starty + (float)m_core_config.height }, true);

		m_drawList->draw(ImGui::GetWindowDrawList(), m_startx, m_starty);

		ImGui::PopClipRect();
		ImGui::Dummy(ImVec2((float)m_core_config.width, (float)m_core_config.height));


		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			mvInput::setDrawingMousePosition((float)mousepos.x- m_startx, (float)mousepos.y - m_starty);
		}
	}

	mvRef<mvDrawList> mvDrawing::getDrawList()
	{
		return m_drawList;
	}

	PyObject* mvDrawing::add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;
		float originx = 0.0f;
		float originy = 0.0f;
		float scalex = 1.0f;
		float scaley = 1.0f;

		if (!(mvApp::GetApp()->getParsers())["add_drawing"].parse(args, kwargs, __FUNCTION__,
			&name, &parent, &before, &width, &height, &show, &originx, &originy, &scalex, &scaley))
			return ToPyBool(false);

		auto item = CreateRef<mvDrawing>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (!item)
			return ToPyBool(false);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}

	PyObject* mvDrawing::modify_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::string drawing = ToString(PyTuple_GetItem(args, 0));
		std::string tag = ToString(PyTuple_GetItem(args, 1));

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing.c_str());

		if (drawlist)
			drawlist->getCommand(tag)->setConfigDict(kwargs);
		else
			ThrowPythonException(tag + std::string(" tag was not found"));

		return GetPyNone();
	}

	PyObject* mvDrawing::get_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["get_draw_command"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);

		if (drawlist)
		{

			if (auto command = drawlist->getCommand(tag))
			{
				PyObject* pdict = PyDict_New();
				command->getConfigDict(pdict);
				return pdict;
			}
			else
				ThrowPythonException(tag + std::string(" tag was not found"));
		}
		return GetPyNone();
	}

	PyObject* mvDrawing::bring_draw_command_forward(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["bring_draw_command_forward"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);

		if (drawlist)
			drawlist->bringForward(tag);

		return GetPyNone();
	}

	PyObject* mvDrawing::bring_draw_command_to_front(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["bring_draw_command_to_front"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->bringToFront(tag);

		return GetPyNone();
	}

	PyObject* mvDrawing::send_draw_command_back(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["send_draw_command_back"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->sendBack(tag);

		return GetPyNone();
	}

	PyObject* mvDrawing::send_draw_command_to_back(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["send_draw_command_to_back"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->sendToBack(tag);

		return GetPyNone();
	}

	PyObject* mvDrawing::delete_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(mvApp::GetApp()->getParsers())["delete_draw_command"].parse(args, kwargs, __FUNCTION__, &drawing, &tag))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);

		if (drawlist)
			drawlist->deleteCommand(tag);

		return GetPyNone();
	}

	PyObject* mvDrawing::clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!(mvApp::GetApp()->getParsers())["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);

		if (drawlist)
			drawlist->clear();

		return GetPyNone();
	}
}
