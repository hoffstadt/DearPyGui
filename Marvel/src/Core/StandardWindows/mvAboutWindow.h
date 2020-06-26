#pragma once

namespace Marvel {

	class mvAboutWindow
	{

	public:

		static mvAboutWindow* GetWindow();

		void render(bool& show);

	private:

		mvAboutWindow() = default;

	private:

		static mvAboutWindow* s_instance;

	};

}