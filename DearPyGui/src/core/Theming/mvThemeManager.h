#pragma once

#include <unordered_map>
#include "mvEvents.h"

namespace Marvel {

	class mvAppItem;
	enum class mvAppItemType;

	class mvThemeManager : public mvEventHandler
	{

	public:

		static void decodeType(long encoded_constant, mvAppItemType* type);
		static void decodelibID(long encoded_constant, int* libID);
		static int decodeIndex(long encoded_constant);

	public:

		mvThemeManager();
		~mvThemeManager();

	private:

		bool onEvent(mvEvent& event) override;
		bool add_color(mvEvent& event);
		bool add_style(mvEvent& event);

	};

	class mvImGuiThemeScope
	{
	public:
		mvImGuiThemeScope(mvAppItem* item);
		~mvImGuiThemeScope();

		void cleanup();

	private:
		int libIDCount = 0;
		int StyleIDCount = 0;
	};

}