#include "mvDrawingInterface.h"
#include "mvDrawCmdCommon.h"
#include "mvDrawList.h"
#include "mvEvents.h"
#include "mvWindow.h"

namespace Marvel {

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers)
	{

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
			{mvPythonDataType::FloatList, "pmin"},
			{mvPythonDataType::FloatList, "pmax", ""},
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
			{mvPythonDataType::FloatList, "pmin"},
			{mvPythonDataType::FloatList, "pmax"},
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

}