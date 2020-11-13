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
		void       bringForward (const std::string& tag);
		void       bringToFront (const std::string& tag);
		void       sendBack     (const std::string& tag);
		void       sendToBack   (const std::string& tag);
		mvDrawCmd* getCommand   (const std::string& tag);

	private:

		std::vector<mvDrawCmd*> m_commands;

	};

}