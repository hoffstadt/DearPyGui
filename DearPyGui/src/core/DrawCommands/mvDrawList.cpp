#include "mvDrawList.h"
#include "mvDrawCmd.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

namespace Marvel {

	mvDrawList::~mvDrawList()
	{
		clear();
	}

	void mvDrawList::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto& command : m_commands)
			command->draw(drawlist, x, y);
	}

	void mvDrawList::clear()
	{
		m_commands.clear();
	}

	void mvDrawList::addCommand(mvRef<mvDrawCmd> command)
	{
		mvRef<mvDrawCmd> oldcommand = getCommand(command->tag);

		if (oldcommand.get())
			oldcommand = command;

		else
			m_commands.push_back(command);
		
	}

	void mvDrawList::bringForward(const std::string& tag)
	{
		if (tag.empty())
			return;

		for (size_t i = 0; i < m_commands.size(); i++)
		{
			if (m_commands[i]->tag == tag)
			{
				if (i == m_commands.size() - 1)
					return;
				std::swap(m_commands[i], m_commands[i + 1]);
				return;
			}
		}
	}

	void mvDrawList::sendBack(const std::string& tag)
	{
		if (tag.empty())
			return;

		for (size_t i = 0; i < m_commands.size(); i++)
		{
			if (m_commands[i]->tag == tag)
			{
				if (i == 0)
					return;
				std::swap(m_commands[i], m_commands[i - 1]);
			}
		}
	}

	void mvDrawList::bringToFront(const std::string& tag)
	{
		if (tag.empty())
			return;

		for (size_t i = 0; i < m_commands.size(); i++)
		{
			if (m_commands[i]->tag == tag)
			{
				if (i == m_commands.size() - 1)
					return;
				std::swap(m_commands[i], m_commands[m_commands.size() - 1]);
			}
		}
	}

	void mvDrawList::sendToBack(const std::string& tag)
	{
		if (tag.empty())
			return;

		for (size_t i = 0; i < m_commands.size(); i++)
		{
			if (m_commands[i]->tag == tag)
			{
				if (i == 0)
					return;
				std::swap(m_commands[i], m_commands[0]);
			}
		}
	}

	void mvDrawList::deleteCommand(const std::string& tag)
	{
		bool tagFound = false;
		size_t index = 0;

		for (index = 0; index < m_commands.size(); index++)
		{
			if (m_commands[index]->tag == tag)
			{
				tagFound = true;
				m_commands[index] = nullptr;
				break;
			}
		}

		if (!tagFound)
			return;

		std::vector<mvRef<mvDrawCmd>> oldCommands = std::move(m_commands);
		m_commands.clear();

		for (size_t i = 0; i < oldCommands.size(); i++)
		{
			if (i != index)
				m_commands.push_back(oldCommands[i]);

		}
	}

	mvRef<mvDrawCmd> mvDrawList::getCommand(const std::string& tag)
	{

		if (tag.empty())
			return nullptr;

		for (auto& cmd : m_commands)
		{
			if (cmd->tag == tag)
				return cmd;
		}

		return nullptr;
	}

	PyObject* modify_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvGlobalIntepreterLock gil;

		std::string drawing = ToString(PyTuple_GetItem(args, 0));
		std::string tag = ToString(PyTuple_GetItem(args, 1));

		mvDrawList* drawlist = GetDrawListFromTarget(drawing.c_str());
		if (drawlist)
		{

			if (auto command = drawlist->getCommand(tag))
				command->setConfigDict(kwargs);
			else
				ThrowPythonException(tag + std::string(" tag was not found"));
		}
		return GetPyNone();
	}

	PyObject* get_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["get_draw_command"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
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

	PyObject* bring_draw_command_forward(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["bring_draw_command_forward"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->bringForward(tag);

		return GetPyNone();
	}

	PyObject* bring_draw_command_to_front(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["bring_draw_command_to_front"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->bringToFront(tag);

		return GetPyNone();
	}

	PyObject* send_draw_command_back(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["send_draw_command_back"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->sendBack(tag);

		return GetPyNone();
	}

	PyObject* send_draw_command_to_back(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["send_draw_command_to_back"].parse(args, kwargs, __FUNCTION__,
			&drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->sendToBack(tag);

		return GetPyNone();
	}

	PyObject* delete_draw_command(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		const char* tag;

		if (!(*mvApp::GetApp()->getParsers())["delete_draw_command"].parse(args, kwargs, __FUNCTION__, &drawing, &tag))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->deleteCommand(tag);

		return GetPyNone();
	}

	PyObject* clear_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;

		if (!(*mvApp::GetApp()->getParsers())["clear_drawing"].parse(args, kwargs, __FUNCTION__, &drawing))
			return GetPyNone();

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->clear();

		return GetPyNone();
	}
}