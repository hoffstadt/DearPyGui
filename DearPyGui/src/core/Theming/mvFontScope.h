#pragma once

namespace Marvel {

	class mvAppItem;

	class mvFontScope
	{

	public:

		mvFontScope(mvAppItem* item);

		~mvFontScope();

		void cleanup();

	private:

		bool m_valid = false;

	};
}