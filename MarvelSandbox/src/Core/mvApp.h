#pragma once

#include <vector>
#include <string>
#include "AppItems/mvAppItem.h"

namespace Marvel {

	class mvApp final
	{

	public: // public static methods

		static mvApp* GetApp();

	public: // public methods

		mvAppItem* addInputText(const std::string& parent, const std::string& name, const std::string& hint = "");

		void render();

	private: // private methods

		mvApp() = default;

		mvApp(const mvApp& other) = delete;
		mvApp(mvApp&& other) = delete;
		mvApp operator=(const mvApp& other) = delete;
		mvApp operator=(mvApp&& other) = delete;

	private: // private static members

		static mvApp* s_instance;

	private: // private members

		std::vector<mvAppItem*> m_items;

	};

}