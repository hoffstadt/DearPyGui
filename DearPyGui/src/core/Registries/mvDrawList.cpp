#include "mvDrawList.h"
#include "DrawCommands/mvDrawCmd.h"

namespace Marvel {

	mvDrawList::~mvDrawList()
	{
		clear();
	}

	void mvDrawList::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto command : m_commands)
			command->draw(drawlist, x, y);
	}

	void mvDrawList::clear()
	{
		for (mvDrawCmd* item : m_commands)
		{
			delete item;
			item = nullptr;
		}

		m_commands.clear();
	}

	void mvDrawList::addCommand(mvDrawCmd* command)
	{
		m_commands.push_back(command);
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
				delete m_commands[index];
				m_commands[index] = nullptr;
				break;
			}
		}

		if (!tagFound)
			return;

		std::vector<mvDrawCmd*> oldCommands = m_commands;
		m_commands.clear();

		for (size_t i = 0; i < oldCommands.size(); i++)
		{
			if (i != index)
				m_commands.push_back(oldCommands[i]);

		}
	}

	mvDrawCmd* mvDrawList::getCommand(const std::string& tag)
	{

		if (tag.empty())
			return nullptr;

		for (mvDrawCmd* cmd : m_commands)
		{
			if (cmd->tag == tag)
				return cmd;
		}

		return nullptr;
	}
}