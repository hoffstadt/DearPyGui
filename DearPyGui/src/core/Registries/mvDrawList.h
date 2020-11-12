#pragma once

#include <string>
#include <vector>
#include "mvCore.h"

namespace Marvel {

	class mvDrawCmd;

	class mvDrawList
	{

	public:

		~mvDrawList();

		void       draw         (ImDrawList* drawlist, float x, float y);
		void       clear        ();
		void       addCommand   (mvDrawCmd* command);
		void       deleteCommand(const std::string& tag);
		mvDrawCmd* getCommand   (const std::string& tag);

	private:

		std::vector<mvDrawCmd*> m_commands;

	};

}