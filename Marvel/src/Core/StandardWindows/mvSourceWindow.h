#pragma once

namespace Marvel {

	class mvSourceWindow
	{

	public:

		static mvSourceWindow* GetWindow();

		void render(bool& show);

	private:

		mvSourceWindow() = default;

	private:

		static mvSourceWindow* s_instance;

	};

}