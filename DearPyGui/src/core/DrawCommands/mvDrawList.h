#pragma once

#include <string>
#include <vector>
#include <map>
#include "mvPythonParser.h"
#include "mvCore.h"
#include "mvPython.h"

namespace Marvel {

	class mvDrawCmd;

	PyObject* modify_draw_command        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_draw_command           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* bring_draw_command_forward (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* bring_draw_command_to_front(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* send_draw_command_back     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* send_draw_command_to_back  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_draw_command        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_drawing              (PyObject* self, PyObject* args, PyObject* kwargs);

	class mvDrawList
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		~mvDrawList();

		void             draw         (ImDrawList* drawlist, float x, float y);
		void             clear        ();
		void             addCommand   (mvRef<mvDrawCmd> command);
		void             deleteCommand(const std::string& tag);
		void             bringForward (const std::string& tag);
		void             bringToFront (const std::string& tag);
		void             sendBack     (const std::string& tag);
		void             sendToBack   (const std::string& tag);
		mvRef<mvDrawCmd> getCommand   (const std::string& tag);

	private:

		std::vector<mvRef<mvDrawCmd>> m_commands;

	};

}